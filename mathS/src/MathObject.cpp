#include <MathObject.h>

using namespace mathS;

void mathS::Vector::push_back(Ptr<MathObject> const f)
{
	components.push_back(f);
}

std::string mathS::Vector::GetString() const
{
	return "{" + ListGetString(components) + "}";
}

std::string mathS::Vector::GetLaTeXString() const
{
	return "\\left\\{" + ListGetLaTeXString(components) + "\\right\\}";
}

Ptr<MathObject> mathS::Vector::DeepCopy() const
{
	Ptr<Vector> ret = New<Vector>();
	ret->components = ListDeepCopy(components);
	return ret;
}


std::string mathS::Function::GetString() const
{
	if (function->Level() <= LEVEL_FUNCTION)
		return function->GetString() + "(" + ListGetString(parameter) + ")";
	else
		return "(" + function->GetString() + ")" + "(" + ListGetString(parameter) + ")";
}

std::string mathS::Function::GetLaTeXString() const
{
	static std::unordered_map<std::string, std::string> str2LaTeXstr = {
		{"Exp", "\\exp"}, {"Sin", "\\sin"}, {"Cos", "\\cos"}, {"Tan", "\\tan"}, 
		{"Log", "\\log"}, {"Ln", "\\ln"}, {"Lg", "\\lg"}, {"Cot", "\\cot"}, {"Csc", "\\csc"},
		{"Sec", "\\sec"}, {"ASin", "\\arcsin"}, {"ACos", "\\arccos"}, {"ATan", "\\arctan"}, 
		{"Sgn", "\\sgn"}, {"Det", "\\det"}, {"Sqrt", "\\sqrt"}
	};
	std::string funName = function->GetLaTeXString();
	if (function->Level() <= LEVEL_FUNCTION) {
		if (str2LaTeXstr.find(funName) != str2LaTeXstr.end()) {
			funName = str2LaTeXstr[funName];
			if(funName=="\\sqrt" || (parameter.size()==1 && parameter[0]->Level() <= MathObject::LEVEL_POWER))
				return funName + "{" + ListGetLaTeXString(parameter) + "}";
			else
				return funName + "{\\left(" + ListGetLaTeXString(parameter) + "\\right)}";
		}
		else
			return funName + "\\left(" + ListGetLaTeXString(parameter) + "\\right)";
	}
	else
		return "\\left(" + funName + "\\right)" + "\\left(" + ListGetLaTeXString(parameter) + "\\right)";
}

mathS::Ptr<MathObject> mathS::Function::DeepCopy() const
{
	Ptr<Function> ret = New<Function>();
	ret->function = function->DeepCopy();
	ret->parameter = ListDeepCopy(parameter);
	return ret;
}

std::string mathS::Locate::GetString() const
{
	if (object->Level() <= LEVEL_LOCATE)
		return object->GetString() + "[" + ListGetString(location) + "]";
	else
		return "(" + object->GetString() + ")" + "[" + ListGetString(location) + "]";
}

std::string mathS::Locate::GetLaTeXString() const
{
	if (object->Level() <= LEVEL_LOCATE)
		return object->GetLaTeXString() + "\\left[" + ListGetLaTeXString(location) + "\\right]";
	else
		return "\\left(" + object->GetLaTeXString() + "\\right)" + "\\left[" + ListGetLaTeXString(location) + "\\right]";
}

mathS::Ptr<MathObject> mathS::Locate::DeepCopy() const
{
	Ptr<Locate> ret = New<Locate>();
	ret->object = object->DeepCopy();
	ret->location = ListDeepCopy(location);
	return ret;
}

std::string mathS::Power::GetString() const
{
	return
		(base->Level() < LEVEL_POWER ? base->GetString() : "(" + base->GetString() + ")") + "^" +
		(exponent->Level() <= LEVEL_POWER ? exponent->GetString() : "(" + exponent->GetString() + ")");
}

std::string mathS::Power::GetLaTeXString() const
{
	return
		(base->Level() < LEVEL_FUNCTION ? base->GetLaTeXString() : "\\left(" + base->GetLaTeXString() + "\\right)") + "^" +
		("{" + exponent->GetLaTeXString() + "}");
}

mathS::Ptr<MathObject> mathS::Power::DeepCopy() const
{
	Ptr<Power> ret = New<Power>();
	ret->base = base->DeepCopy();
	ret->exponent = exponent->DeepCopy();
	return ret;
}

std::string mathS::Inverse::GetString() const
{
	if (component->Level() <= LEVEL_INVERSE)
		return "/" + component->GetString();
	else
		return "/(" + component->GetString() + ")";
}

std::string mathS::Inverse::GetLaTeXString() const
{
	return component->GetLaTeXString();
}

mathS::Ptr<MathObject> mathS::Inverse::DeepCopy() const
{
	Ptr<Inverse> ret = New<Inverse>();
	ret->component = component->DeepCopy();
	return ret;
}

void mathS::Item::push_back(Ptr<MathObject> const f)
{
	if (f->GetType() == Type::ITEM) {
		auto itm = Dynamic_cast<Item>(f);
		for (auto& it : itm->factors) {
			factors.push_back(it);
		}
	}
	else {
		factors.push_back(f);
	}
}

std::string mathS::Item::GetString() const
{
	std::string ret;
	if (factors[0]->Level() < LEVEL_ITEM){
		if (factors[0]->GetType() == Type::INVERSE)
			ret += "1";
		ret += factors[0]->GetString();
	}
	else{
		ret += "(" + factors[0]->GetString() + ")";
	}

	for (int i = 1; i < factors.size(); i++){
		if (factors[i]->Level() < LEVEL_ITEM){
			if (factors[i]->GetType() != Type::INVERSE)
				ret += "*";
			ret += factors[i]->GetString();
		}
		else{
			ret += "*(" + factors[i]->GetString() + ")";
		}
	}
	return ret;
}

std::string mathS::Item::GetLaTeXString() const
{
	std::string ret;
	std::string temp = "";
	int invflag = 0, braceflag = 0;
	if (factors[0]->Level() < LEVEL_ITEM) {
		if (factors[0]->GetType() == Type::INVERSE) {
			temp = "\\frac{1}{" + factors[0]->GetLaTeXString() + "}";
			invflag = 1;
		}
		else
			temp = factors[0]->GetLaTeXString();
	}
	else {
		temp = "\\left(" + factors[0]->GetLaTeXString() + "\\right)";
		braceflag = 1;
	}
	for (int i = 1; i < factors.size(); i++) {
		if (factors[i]->GetType() == Type::INVERSE) {
			temp = "\\frac{" + temp + "}{" + factors[i]->GetLaTeXString() + "}";
			invflag = 1;
		}
		else {
			if (!invflag) {
				if (factors[i]->Level() >= LEVEL_ITEM) {
					temp += "\\left(" + factors[i]->GetLaTeXString() + "\\right)";
					braceflag = 1;
				}
				else {
					if (braceflag)
						temp += factors[i]->GetLaTeXString();
					else {
						if (factors[i]->GetType()==Type::ATOM && Dynamic_cast<Atom>(factors[i])->AtomType()==Type::NUMBER)
							temp += "\\times " + factors[i]->GetLaTeXString();
						else
							temp += "\\, " + factors[i]->GetLaTeXString();
					}
					braceflag = 0;
				}
			}
			else {
				ret += temp + "\\times ";
				temp = "";
				invflag = 0;
				if (factors[i]->Level() < LEVEL_ITEM) {
					temp += factors[i]->GetLaTeXString();
					braceflag = 0;
				}
				else {
					temp += "\\left(" + factors[i]->GetLaTeXString() + "\\right)";
					braceflag = 1;
				}
			}
		}
	}
	ret += temp;
	return ret;
}

mathS::Ptr<MathObject> mathS::Item::DeepCopy() const
{
	Ptr<Item> ret = New<Item>();
	ret->factors.reserve(factors.size());
	for (auto it : factors)
		ret->factors.push_back(it->DeepCopy());
	return ret;
}

std::string mathS::Opposite::GetString() const
{
	if (component->Level() < LEVEL_OPPOSITE)
		return "-" + component->GetString();
	else
		return "-(" + component->GetString() + ")";
}

std::string mathS::Opposite::GetLaTeXString() const
{
	if (component->Level() < LEVEL_OPPOSITE)
		return "-" + component->GetLaTeXString();
	else
		return "-\\left(" + component->GetLaTeXString() + "\\right)";
}

mathS::Ptr<MathObject> mathS::Opposite::DeepCopy() const
{
	Ptr<Opposite> ret = New<Opposite>();
	ret->component = component->DeepCopy();
	return ret;
}

std::string mathS::Polynomial::GetString() const
{
	std::string ret;

	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->Level() < LEVEL_POLYNOMIAL)
		{
			if (items[i]->GetType() != Type::OPPOSITE && i != 0)
				ret += "+";
			ret += items[i]->GetString();
		}
		else
		{
			ret += "+(" + items[i]->GetString() + ")";
		}
	}
	return ret;
}

std::string mathS::Polynomial::GetLaTeXString() const
{
	std::string ret;

	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->Level() < LEVEL_POLYNOMIAL)
		{
			if (items[i]->GetType() != Type::OPPOSITE && i != 0)
				ret += "+";
			ret += items[i]->GetLaTeXString();
		}
		else
		{
			ret += "+\\left(" + items[i]->GetLaTeXString() + "\\right)";
		}
	}
	return ret;
}

mathS::Ptr<MathObject> mathS::Polynomial::DeepCopy() const
{
	Ptr<Polynomial> ret = New<Polynomial>();
	ret->items.reserve(items.size());
	for (auto it : items)
		ret->items.push_back(it->DeepCopy());
	return ret;
}

void mathS::Polynomial::push_back(Ptr<MathObject> const f)
{
	if (f->GetType() == Type::POLYNOMIAL) {
		auto poly = Dynamic_cast<Polynomial>(f);
		for (auto& it : poly->items) {
			items.push_back(it);
		}
	}
	else {
		items.push_back(f);
	}
}

std::string mathS::Map::GetString() const
{
	return
		(key->Level() < LEVEL_MAP ? key->GetString() : "(" + key->GetString() + ")") + "->" +
		(value->Level() < LEVEL_MAP ? value->GetString() : "(" + value->GetString() + ")");
}

std::string mathS::Map::GetLaTeXString() const
{
	return
		(key->Level() < LEVEL_MAP ? key->GetLaTeXString() : "\\left(" + key->GetLaTeXString() + "\\right)") + "\\rightarrow " +
		(value->Level() < LEVEL_MAP ? value->GetLaTeXString() : "\\left(" + value->GetLaTeXString() + "\\right)");
}

mathS::Ptr<MathObject> mathS::Map::DeepCopy() const
{
	Ptr<Map> ret = New<Map>();
	ret->key = key->DeepCopy();
	ret->value = value->DeepCopy();
	return ret;
}

std::string mathS::Compare::GetString() const
{
	return
		(left->Level() < LEVEL_COMPARE ? left->GetString() : "(" + left->GetString() + ")") + op +
		(right->Level() < LEVEL_COMPARE ? right->GetString() : "(" + right->GetString() + ")");
}

std::string mathS::Compare::GetLaTeXString() const
{
	std::string opLaTeX = op;
	if (op == ">=")
		opLaTeX = "\\ge ";
	if (op == "<=")
		opLaTeX = "\\le ";
	return
		(left->Level() < LEVEL_COMPARE ? left->GetLaTeXString() : "\\left(" + left->GetLaTeXString() + "\\right)") + opLaTeX +
		(right->Level() < LEVEL_COMPARE ? right->GetLaTeXString() : "\\left(" + right->GetLaTeXString() + "\\right)");
}

mathS::Ptr<MathObject> mathS::Compare::DeepCopy() const
{
	Ptr<Compare> ret = New<Compare>();
	ret->op = op;
	ret->left = left->DeepCopy();
	ret->right = right->DeepCopy();
	return ret;
}


mathS::MathObject::Type mathS::Atom::AtomType() const
{
	if ('0' <= str[0] && str[0] <= '9')
		return Type::NUMBER;
	else if (('a' <= str[0] && str[0] <= 'z') || ('A' <= str[0] && str[0] <= 'Z') || str[0] == '_' || str[0] == '$')
		return Type::VARIABLE;
	else if ('\"' == str[0] && '\"' == str[str.length() - 1])
		return Type::STRING;
	else
		return Type::ERROR;
}

double mathS::Atom::NumberValue() const
{
	return std::stod(str);
}

std::string mathS::Atom::GetString() const
{
	return str;
}

std::string mathS::Atom::GetLaTeXString() const
{
	std::string LaTeXstr;
	if (str == "Pi")
		return "{\\pi}";
	if (str == "E")
		return "\\mathrm{e}";
	for (auto c : str) {
		if (c == '_' || c == '$' || c == '#' || c == '&')
			LaTeXstr += "\\";
		if (c == '~')
			LaTeXstr += "\\~{}";
		LaTeXstr += c;
	}
	return LaTeXstr;
}

mathS::Ptr<MathObject> mathS::Atom::DeepCopy() const
{
	return New<Atom>(str);
}

mathS::Ptr<MathObject> mathS::ErrorObject::DeepCopy() const
{
	return New<ErrorObject>(info);
}

mathS::Ptr<MathObject> mathS::EmptyObject::DeepCopy() const
{
	return New<EmptyObject>();
}

std::string mathS::FunctionalOperator::GetString() const
{
	std::string ret;
	if (function->Level() <= LEVEL_FUNCOPERATOR)
		ret = function->GetString() + "<<";
	else
		ret = "(" + function->GetString() + ")<<";
	if(!variables.empty())
		ret += variables[0]->GetString();
	for (int i = 1; i < variables.size(); i++) 
		ret += ","+variables[i]->GetString();
	ret += "|" + ListGetString(fparameter) + ">>(" + ListGetString(parameter) + ")";
	return ret;
}

std::string mathS::FunctionalOperator::GetLaTeXString() const
{
	std::string ret;
    if (function->GetString() == "Sum"){
        for (int i = 0; i < variables.size(); i++) {
            ret += "\\sum_{" + variables[i]->GetLaTeXString() + "}" +
                   "^{" + parameter[i]->GetLaTeXString() + "}";
        }
        if (fparameter.size() == 1)
            if (fparameter[0]->Level() <= MathObject::LEVEL_POWER)
                ret += fparameter[0]->GetLaTeXString();
            else
                ret += "\\left(" + fparameter[0]->GetLaTeXString() + "\\right)";
        else
            ret += "\\left\\{" + ListGetLaTeXString(fparameter) + "\\right\\}";
        return ret;
    } else if (function->GetString() == "Product"){
        for (int i = 0; i < variables.size(); i++) {
            ret += "\\prod_{" + variables[i]->GetLaTeXString() + "}" +
                   "^{" + parameter[i]->GetLaTeXString() + "}";
        }
        if (fparameter.size() == 1)
            if (fparameter[0]->Level() <= MathObject::LEVEL_POWER)
                ret += fparameter[0]->GetLaTeXString();
            else
                ret += "\\left(" + fparameter[0]->GetLaTeXString() + "\\right)";
        else
            ret += "\\left\\{" + ListGetLaTeXString(fparameter) + "\\right\\}";
        return ret;
    }

	return ret;
}

mathS::Ptr<MathObject> mathS::FunctionalOperator::DeepCopy() const
{
	Ptr<FunctionalOperator> ret = New<FunctionalOperator>();
	ret->function = function->DeepCopy();
	for (auto itv : variables) 
		ret->variables.push_back(Dynamic_cast<Atom, MathObject>(itv->DeepCopy()));
		
	ret->fparameter = ListDeepCopy(fparameter);
	ret->parameter = ListDeepCopy(parameter);
	return ret;
}

std::string mathS::ListGetString(const std::vector<Ptr<MathObject>>& lst)
{
	std::string ret;
	if (lst.size() == 0)
		return "";
	if (lst[0]->Level() < MathObject::LEVEL_LIST)
		ret += lst[0]->GetString();
	else
		ret += "(" + lst[0]->GetString() + ")";
	for (int i = 1; i < lst.size(); i++)
		if (lst[i]->Level() < MathObject::LEVEL_LIST)
			ret += "," + lst[i]->GetString();
		else
			ret += ",(" + lst[i]->GetString() + ")";
	return ret;
}

std::string mathS::ListGetLaTeXString(const std::vector<Ptr<MathObject>>& lst)
{
	std::string ret;
	if (lst.size() == 0)
		return "";
	if (lst[0]->Level() < MathObject::LEVEL_LIST)
		ret += lst[0]->GetLaTeXString();
	else
		ret += "\\left(" + lst[0]->GetLaTeXString() + "\\right)";
	for (int i = 1; i < lst.size(); i++)
		if (lst[i]->Level() < MathObject::LEVEL_LIST)
			ret += "," + lst[i]->GetLaTeXString();
		else
			ret += ",\\left(" + lst[i]->GetLaTeXString() + "\\right)";
	return ret;
}

std::vector<Ptr<MathObject>> mathS::ListDeepCopy(const std::vector<Ptr<MathObject>>& lst)
{
	std::vector<Ptr<MathObject>> ret;
	ret.reserve(lst.size());
	for (auto it : lst)
		ret.push_back(it->DeepCopy());
	return ret;
}
