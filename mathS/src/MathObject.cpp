#include <MathObject.h>

std::string mathS::Vector::GetString() const
{
	return "{" + list->GetString() + "}";
}

mathS::MathObject* mathS::Vector::DeepCopy() const
{
	Vector* ret= new Vector();
	ret->list = dynamic_cast<ListObject*>(list->DeepCopy());
	return ret;
}

std::string mathS::Function::GetString() const
{
	if (function->Level() <= LEVEL_FUNCTION)
		return function->GetString() + "(" + parameter->GetString() + ")";
	else
		return "(" + function->GetString() + ")" + "(" + parameter->GetString() + ")";
}

mathS::MathObject* mathS::Function::DeepCopy() const
{
	Function* ret = new Function();
	ret->function = function->DeepCopy();
	ret->parameter = parameter->DeepCopy();
	return ret;
}

std::string mathS::Locate::GetString() const
{
	if (object->Level() <= LEVEL_LOCATE)
		return object->GetString() + "[" + location->GetString() + "]";
	else
		return "(" + object->GetString() + ")" + "[" + object->GetString() + "]";
}

mathS::MathObject* mathS::Locate::DeepCopy() const
{
	Locate* ret = new Locate();
	ret->object = object->DeepCopy();
	ret->location = location->DeepCopy();
	return ret;
}

std::string mathS::Power::GetString() const
{
	return
		(base->Level() < LEVEL_POWER ? base->GetString() : "(" + base->GetString() + ")") + "^" +
		(exponent->Level() <= LEVEL_POWER ? exponent->GetString() : "(" + exponent->GetString() + ")");
}

mathS::MathObject* mathS::Power::DeepCopy() const
{
	Power* ret = new Power();
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

mathS::MathObject* mathS::Inverse::DeepCopy() const
{
	Inverse* ret = new Inverse();
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

mathS::MathObject* mathS::Item::DeepCopy() const
{
	Item* ret = new Item();
	ret->factors.reserve(factors.size());
	for (auto it : factors)
		ret->factors.push_back(it->DeepCopy());
	return ret;
}

std::string mathS::Opposite::GetString() const
{
	if (component->Level() <= LEVEL_OPPOSITE)
		return "-" + component->GetString();
	else
		return "-(" + component->GetString() + ")";
}

mathS::MathObject* mathS::Opposite::DeepCopy() const
{
	Opposite* ret = new Opposite();
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

mathS::MathObject* mathS::Polynomial::DeepCopy() const
{
	Polynomial* ret = new Polynomial();
	ret->items.reserve(items.size());
	for (auto it : items)
		ret->items.push_back(it->DeepCopy());
	return nullptr;
}

std::string mathS::Map::GetString() const
{
	return
		(key->Level() < LEVEL_MAP ? key->GetString() : "(" + key->GetString() + ")") + "->" +
		(value->Level() < LEVEL_MAP ? value->GetString() : "(" + value->GetString() + ")");
}

mathS::MathObject* mathS::Map::DeepCopy() const
{
	Map* ret = new Map;
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

mathS::MathObject* mathS::Compare::DeepCopy() const
{
	Compare* ret = new Compare();
	ret->left = left->DeepCopy();
	ret->right = right->DeepCopy();
	return ret;
}

std::string mathS::ListObject::GetString() const
{
	std::string ret;
	if (components.size() == 0)
		return "";
	if (components[0]->Level() < LEVEL_LIST)
		ret += components[0]->GetString();
	else
		ret += "(" + components[0]->GetString() + ")";
	for (int i = 1; i < components.size(); i++)
		if (components[i]->Level() < LEVEL_LIST)
			ret += "," + components[i]->GetString();
		else
			ret += ",(" + components[i]->GetString() + ")";
	return ret;
}

mathS::MathObject* mathS::ListObject::DeepCopy() const
{
	ListObject* ret = new ListObject();
	ret->components.reserve(components.size());
	for (auto it : components)
		ret->components.push_back(it->DeepCopy());
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

mathS::MathObject* mathS::Atom::DeepCopy() const
{
	return new Atom(str);
}

mathS::MathObject* mathS::ErrorObject::DeepCopy() const
{
	return new ErrorObject(info);
}

mathS::MathObject* mathS::EmptyObject::DeepCopy() const
{
	return new EmptyObject();
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
	ret += "|" + fparameter->GetString() + ">>(" + parameter->GetString() + ")";
	return ret;
}

mathS::MathObject* mathS::FunctionalOperator::DeepCopy() const
{
	FunctionalOperator* ret = new FunctionalOperator;
	ret->function = function->DeepCopy();
	for (auto itv : variables) 
		ret->variables.push_back(dynamic_cast<Atom*>(itv->DeepCopy()));
		
	ret->fparameter = fparameter->DeepCopy();
	ret->parameter = parameter->DeepCopy();
	return ret;
}
