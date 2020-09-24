#include <MathObject.h>

using namespace mathS;

std::string mathS::Vector::GetString() const
{
	return "{" + ListGetString(components) + "}";
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

mathS::Ptr<MathObject> mathS::Inverse::DeepCopy() const
{
	Ptr<Inverse> ret = New<Inverse>();
	ret->component = component->DeepCopy();
	return ret;
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

mathS::Ptr<MathObject> mathS::Polynomial::DeepCopy() const
{
	Ptr<Polynomial> ret = New<Polynomial>();
	ret->items.reserve(items.size());
	for (auto it : items)
		ret->items.push_back(it->DeepCopy());
	return ret;
}

std::string mathS::Map::GetString() const
{
	return
		(key->Level() < LEVEL_MAP ? key->GetString() : "(" + key->GetString() + ")") + "->" +
		(value->Level() < LEVEL_MAP ? value->GetString() : "(" + value->GetString() + ")");
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

std::vector<Ptr<MathObject>> mathS::ListDeepCopy(const std::vector<Ptr<MathObject>>& lst)
{
	std::vector<Ptr<MathObject>> ret;
	ret.reserve(lst.size());
	for (auto it : lst)
		ret.push_back(it->DeepCopy());
	return ret;
}
