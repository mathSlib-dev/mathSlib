#include <MathParser.h>
#include <stack>
#include <string-switch.h>

using namespace mathS;

// 解析表达式
MathObject *Parser::Parse() {
    struct Operator {
        Operator() {}
        Operator(const Operator& op) : priority(op.priority), text(op.text) {}
        Operator(short p, const std::string& t) : priority(p), text(t) { }
        Operator& operator=(const Operator& o) {
            priority = o.priority;
            text = o.text;
            return *this;
        }
        short priority;
        std::string text;
    };
    std::stack<MathObject*> objects;
    std::stack<Operator> operators;
    bool preIsNS(false);
    Token token;
    Operator op;
    short p;
    bool finish = false;
    while (token.type != END) {
        lexer.get(token);
        switch (token.type) {
            case NUMBER:
            case SYMBOL:
                objects.push(new Atom(token.text));
                if (preIsNS) operators.push(Operator(2, "*"));
                preIsNS = true;
                break;
            case OPERATOR:
                /*
                 * a
                 * b
                 * c
                 * /d
                 * */
                /*
                 * *
                 * *
                 * */
                p = priority(token.text);
                if (operators.empty()) goto FINISH_BUILD;
                op = operators.top();
                if (p < op.priority) {
                    LESS_P:
                    operators.pop();
                    if (op.priority == 2) {// 乘除法
                        auto term = new Item;
                        while (true) {
                            auto i = objects.top();
                            objects.pop();
                            if (op.text == "/") i = new Inverse(i);
                            term->push(i);
                            if (operators.empty()) break;
                            op = operators.top();
                            operators.pop();
                            if (op.priority != 2) break;
                        }
                        objects.push(term);
                    }
                }
                if (finish) return objects.top();
                FINISH_BUILD:
                operators.push(Operator(p, token.text));
                preIsNS = false;
                break;
        }
    }
    if (objects.size() > 1) {
        op = operators.top();
        finish = true;
        goto LESS_P;
    }
    return objects.top();
}

short Parser::priority(const std::string &c) {
    SWITCH(c.c_str()) {
        CASE(c, "^") return 3;
        CASE(c, "*") return 2;
        CASE(c, "/") return 2;
        CASE(c, "%") return 2;
        CASE(c, "+") return 1;
        CASE(c, "-") return 1;
        default: return 0;
    }
    return 0;
}
