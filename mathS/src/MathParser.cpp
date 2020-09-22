#include <MathParser.h>
#include <stack>
#include <string-switch.h>

using namespace mathS;

// 解析表达式
MathObject *Parser::Parse() {

    std::vector<Token> tokens;
    Token tok;
    while (true) {
        lexer.get(tok);
        if (tok.type == Token::END) break;
        tokens.push_back(tok);
    }
    
    int i;
    auto obj = parseObject(tokens, 0, i);
    if (i < tokens.size()) {
        delete obj;
        return new ErrorObject("Parse: Syntax Error. Unexpected Symbol " + tokens[i].text);
    }
    
    return obj;
}

short Parser::level(const std::string &c) {
    SWITCH(c.c_str()) {
        CASE(c, "(") return MathObject::LEVEL_FUNCTION;
        CASE(c, "<<") return MathObject::LEVEL_FUNCOPERATOR;
        CASE(c, "[") return MathObject::LEVEL_LOCATE;
        CASE(c, "^") return MathObject::LEVEL_POWER;
        CASE(c, "/") return MathObject::LEVEL_ITEM;
        CASE(c, "*") return MathObject::LEVEL_ITEM;
        CASE(c, "-") return MathObject::LEVEL_POLYNOMIAL;
        CASE(c, "+") return MathObject::LEVEL_POLYNOMIAL;
        CASE(c, "==") return MathObject::LEVEL_COMPARE;
        CASE(c, "<") return MathObject::LEVEL_COMPARE;
        CASE(c, ">") return MathObject::LEVEL_COMPARE;
        CASE(c, "<=") return MathObject::LEVEL_COMPARE;
        CASE(c, ">=") return MathObject::LEVEL_COMPARE;
        CASE(c, ",") return MathObject::LEVEL_LIST;
        CASE(c, "->")return MathObject::LEVEL_MAP;

        CASE(c, ")") return MathObject::LEVEL_CLOSED;
        CASE(c, "|") return MathObject::LEVEL_CLOSED;
        CASE(c, ">>") return MathObject::LEVEL_CLOSED;
        CASE(c, "]") return MathObject::LEVEL_CLOSED;
        CASE(c, "}") return MathObject::LEVEL_CLOSED;

        default: return MathObject::LEVEL_ERROR;
    }
    return 0;
}

#define ERROR_CHECK(x) if (x->GetType() == MathObject::ERROR) return x
#define NULL_CHECK(x) if (!x) return nullptr
MathObject* mathS::Parser::parseObject(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    // EmptyObject
    if (i >= tokens.size() || level(tokens[i].text) == MathObject::LEVEL_CLOSED) return new EmptyObject;
    return parseList(tokens, i, i);
}
MathObject* mathS::Parser::parseAtom(const std::vector<Token>& tokens, const int start, int& i)
{
    MathObject* obj;
    i = start;
    if (i >= tokens.size())
        return new ErrorObject("Parse: Expression is incompleted.");
    if (tokens[i].type == Token::NUMORSYMB || tokens[i].type == Token::STRING) {
        obj = new Atom(tokens[i].text);
        i++;
        return obj;
    }
    else if (tokens[i].type == Token::OPERATOR) {
        // 如果是操作符，那么必须是(_OBJECT)或{_OBJECT}
        if (tokens[i].text == "(") {
            obj = parseObject(tokens, start + 1, i);
            ERROR_CHECK(obj);
            if (i < tokens.size() && tokens[i].text == ")") {
                i++;
                return obj;
            }
            else {
                delete obj;
                return new ErrorObject("Parser: Unmatched brace (");
            }
        }
        else if(tokens[i].text == "{")
        {
            auto lst = parseList_forced(tokens, start + 1, i);
            if (!lst) return new ErrorObject("Parser: Unmatched brace {");
            ERROR_CHECK(lst);

            Vector* vobj = new Vector;
            vobj->list = dynamic_cast<ListObject*>(lst);
            if (vobj == nullptr) {
                delete vobj;
                return new ErrorObject("Parse: (Unexpected Error) Not a list contained in {...}");
            }
            if (i < tokens.size() && tokens[i].text == "}") {
                i++;
                return vobj;
            }
            else {
                delete vobj;
                return new ErrorObject("Parser: Unmatched brace {");
            }
        }
    }
    return  new ErrorObject("Parse: Syntax Error. Unexpected Symbol. " + tokens[i].text);
}


MathObject* mathS::Parser::parseFunction(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;

    auto f = parseAtom(tokens, start, i);
    ERROR_CHECK(f);
    if (i >= tokens.size()) 
        return f;
    
    if (tokens[i].text == "(") {
        // ( ) 表示函数
        auto p = parseObject(tokens, i + 1, i);
        if (p->GetType() == MathObject::ERROR) {
            delete f;
            return p;
        }
        if (i < tokens.size() && tokens[i].text == ")") {
            i++;
            Function* fobj = new Function;
            fobj->function = f;
            fobj->parameter = p;
            return fobj;
        }
        else {
            delete f; delete p;
            return new ErrorObject("Parse: Syntax Error. Unmatched brace (");
        }
    }
    else if (tokens[i].text == "<<") {
        // << | >>( ) 表示泛函算子
        std::vector<Atom*> vars;
        while (true) {
            auto v = parseAtom(tokens, i + 1, i);
            if (v->GetType() == MathObject::ERROR) {
                delete f; for (auto itv : vars) delete itv;
                return v;
            }
            if (i >= tokens.size()) {
                delete f; for (auto itv : vars) delete itv; delete v;
            }
            auto vatom = dynamic_cast<Atom*>(v);
            if (!vatom) {
                delete f; for (auto itv : vars) delete itv; delete v;
                return new ErrorObject("Parse: Unexpected Error. Cannot convert "+ v->GetString()+" to atom.");
            }
            vars.push_back(vatom);
            if (tokens[i].text == "|") {
                break;
            }
            else if (tokens[i].text != ",") {
                return new ErrorObject("Parse: Syntax Error. Unmatched <<. ");
            }
        }
        auto fpara = parseObject(tokens, i + 1, i);
        if (fpara->GetType() == MathObject::ERROR) {
            delete f; for (auto itv : vars) delete itv;
            return fpara;
        }
        if (!(i < tokens.size() && tokens[i].text == ">>")) {
            delete f; for (auto itv : vars) delete itv; delete fpara;
            return new ErrorObject("Parser: Unmatched <<");
        }
        i++;
        if (!(i < tokens.size() && tokens[i].text == "(")) {
            delete f; for (auto itv : vars) delete itv; delete fpara;
            return new ErrorObject("Parser: Incompleted Functional Operator");
        }
        auto para = parseObject(tokens, i + 1, i);
        if (para->GetType() == MathObject::ERROR) {
            delete f; for (auto itv : vars) delete itv; delete fpara;
            return para;
        }
        if (!(i < tokens.size() && tokens[i].text == ")")) {
            delete f; for (auto itv : vars) delete itv; delete fpara; delete para;
            return new ErrorObject("Parser: Unmatched brace (");
        }
        FunctionalOperator* fop = new FunctionalOperator;

        fop->function = f;
        fop->variables = vars;
        fop->fparameter = fpara;
        fop->parameter = para;
        i++;
        return fop;
    }
    else if (level(tokens[i].text) > MathObject::LEVEL_FUNCTION) {
        // 后面已经截断，说明并不是函数形式，把 f 返回即可
        return f;
    }
    else {
        return new ErrorObject("Parse: Syntax Error. Unexpected symbol " + tokens[i].text);
    }

}

MathObject* mathS::Parser::parseLocate(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    auto obj = parseFunction(tokens, start, i);
    ERROR_CHECK(obj);
    if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_LOCATE) {
        return obj;
    }
    if (tokens[i].text != "[") {
        auto loc = parseObject(tokens, i + 1, i);
        if (loc->GetType() == MathObject::ERROR) {
            delete obj;
            return loc;
        }
        if (!(i < tokens.size() && tokens[i].text == "]")) {
            delete obj; delete loc;
            return new ErrorObject("Parse: Syntax Error. Unmatched brace [");
        }
        Locate* lc = new Locate;
        lc->object = obj;
        lc->location = loc;
        return lc;
    }
    else {
        return new ErrorObject("Parse: Syntax Error. Unexpected symbol " + tokens[i].text);
    }
       
}

MathObject* mathS::Parser::parsePower(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    auto b = parseLocate(tokens, start, i);
    ERROR_CHECK(b);
    // 是否是指数形式
    if (!(i < tokens.size() && tokens[i].text == "^")) {
        return b;
    }
    auto e = parsePower(tokens, i + 1, i);
    if (e->GetType() == MathObject::ERROR) {
        delete b;
        return e;
    }
    // parsePower 和 parseAtom 一定不会返回 Empty
    Power* pw = new Power;
    pw->base = b;
    pw->exponent = e;
    return pw;
}

MathObject* mathS::Parser::parseItem(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    auto fct = parsePower(tokens, start, i);
    ERROR_CHECK(fct);
    if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_ITEM) {
        // 如果项中只有一个factor，那么就直接返回这个factor，而不必再套一层item
        return fct;
    }

    Item* itm = new Item;
    itm->push_back(fct);
    while (true) {
        if (tokens[i].text == "*") {
            // 增加乘因子
            auto t = parsePower(tokens, i + 1, i);
            if (t->GetType() == MathObject::ERROR) {
                delete itm;
                return t;
            }
            itm->push_back(t);
            if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_ITEM)
                return itm;
        }
        else if (tokens[i].text == "/") {
            // 增加除因子
            auto t = parsePower(tokens, i + 1, i);
            if (t->GetType() == MathObject::ERROR) {
                delete itm;
                return t;
            } 
            itm->push_back(new Inverse(t));
            if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_ITEM)
                return itm;
        }
        else {
            delete itm;
            return new ErrorObject("Parse: Syntax Error. Unexpected Symbol " + tokens[i].text);
        }
        
    }
    
}

MathObject* mathS::Parser::parsePolynomial(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_POLYNOMIAL) {
        return new ErrorObject("Parse: Syntax Error. Incomplete expression.");
    }
    MathObject* itm;
    Polynomial* poly = new Polynomial;
    if (tokens[i].text != "+" || tokens[i].text != "-") {
        itm = parseItem(tokens, i, i);
        if (itm->GetType() == MathObject::ERROR) {
            delete poly;
            return itm;
        }
        poly->push_back(itm);
    }
    while (true) {
        if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_POLYNOMIAL) {
            // 若只有一项，简化表达式
            if (poly->items.size() > 1)
                return poly;
            else {
                auto tmp = poly->items[0];
                poly->items.clear();
                delete poly;
                return tmp;
            }
        }
        if (tokens[i].text == "+") {
            // 增加正项
            auto t = parseItem(tokens, i + 1, i);
            if (t->GetType() == MathObject::ERROR) {
                delete poly;
                return t;
            }
            poly->push_back(t);
        }
        else if (tokens[i].text == "-") {
            // 增加负项
            auto t = parseItem(tokens, i + 1, i);
            if (t->GetType() == MathObject::ERROR) {
                delete poly;
                return t;
            }
            poly->push_back(new Opposite(t));
        }
        else {
            // 多项式没有终止，而下一个字符无法识别有效，报错
            delete poly;
            return new ErrorObject("Parse: Syntax Error. Unexpected Symbol " + tokens[i].text);
        }
    }
    return nullptr;
}

MathObject* mathS::Parser::parseMap(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    auto a = parsePolynomial(tokens, i, i);
    ERROR_CHECK(a);
    if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_MAP)
        return a;
    if (tokens[i].text != "->") {
        delete a;
        return new ErrorObject("Parse: Syntax Error. Unexpected Symbol " + tokens[i].text);
    }
    auto b = parsePolynomial(tokens, i + 1, i);
    if (b->GetType() == MathObject::ERROR) {
        delete a;
        return b;
    }
    Map* mp = new Map(a, b);
    return mp;
}

MathObject* mathS::Parser::parseCompare(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    auto a = parseMap(tokens, i, i);
    ERROR_CHECK(a);
    if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_COMPARE)
        return a;
    if (level(tokens[i].text) != MathObject::LEVEL_COMPARE) {
         delete a;
        return new ErrorObject("Parse: Syntax Error. Unexpected Symbol " + tokens[i].text);
    }
    auto b = parseMap(tokens, i + 1, i);
    if (b->GetType() == MathObject::ERROR) {
        delete a;
        return b;
    }
    Compare* cmp = new Compare(a, b);
    return cmp;
}

MathObject* mathS::Parser::parseList(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    auto obj = parseCompare(tokens, start, i);
    ERROR_CHECK(obj);
    if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_LIST) {
        // 如果项中只有一个obj，那么就直接返回这个obj，而不必再套一层List
        return obj;
    }

    ListObject* lst = new ListObject;
    lst->push_back(obj);
    while (true) {
        if (tokens[i].text == ",") {
            auto t = parseCompare(tokens, i + 1, i);
            if (t->GetType() == MathObject::ERROR) {
                delete lst;
                return t;
            }
            lst->push_back(t);
            if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_LIST)
                return lst;
        }
        else {
            delete lst;
            return new ErrorObject("Parse: Syntax Error. Unexpected Symbol " + tokens[i].text);
        }
    }
}

MathObject* mathS::Parser::parseList_forced(const std::vector<Token>& tokens, const int start, int& i)
{
    i = start;
    auto obj = parseCompare(tokens, start, i);
    ERROR_CHECK(obj);

    ListObject* lst = new ListObject;
    lst->push_back(obj);
    while (true) {
        if (i >= tokens.size() || level(tokens[i].text) > MathObject::LEVEL_LIST)
            return lst;
        if (tokens[i].text == ",") {
            auto t = parseCompare(tokens, i + 1, i);
            if (t->GetType() == MathObject::ERROR) {
                delete lst;
                return t;
            }
            lst->push_back(t);
        }
        else {
            delete lst;
            return new ErrorObject("Parse: Syntax Error. Unexpected Symbol " + tokens[i].text);
        }
    }
}
