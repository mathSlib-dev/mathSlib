#include <MathObject.h>

std::string mathS::Number::GetString()
{
	return number;
}

std::string mathS::String::GetString()
{
	return str;
}

std::string mathS::Variable::GetString()
{
	return name;
}

std::string mathS::Vector::GetString()
{
	return "{" + list->GetString() + "}";
}

std::string mathS::Function::GetString()
{
	if (function->Level() <= LEVEL_FUNCTION)
		return function->GetString() + "[" + parameter->GetString() + "]";
	else
		return "(" + function->GetString() + ")" + "[" + parameter->GetString() + "]";
}

std::string mathS::Locate::GetString()
{
	if (object->Level() <= LEVEL_LOCATE)
		return object->GetString() + "[[" + location->GetString() + "]]";
	else
		return "(" + object->GetString() + ")" + "[[" + object->GetString() + "]]";
}

std::string mathS::Power::GetString()
{
	return
		(base->Level() < LEVEL_POWER ? base->GetString() : "(" + base->GetString() + ")") + "^" +
		(exponent->Level() <= LEVEL_POWER ? base->GetString() : "(" + exponent->GetString() + ")");
}

std::string mathS::Inverse::GetString()
{
	if (component->Level() <= LEVEL_INVERSE)
		return "/" + component->GetString();
	else
		return "/(" + component->GetString() + ")";
}

std::string mathS::Item::GetString()
{
	std::string ret;
	if (factors[0]->Level() < LEVEL_ITEM)
	{
		if (factors[0]->GetType() == Type::INVERSE)
			ret += "1";
		ret += factors[0]->GetString();
	}
	else
	{
		ret += "(" + factors[0]->GetString() + ")";
	}

	for (int i = 1; i < factors.size(); i++)
	{
		if (factors[i]->Level() < LEVEL_ITEM)
		{
			if (factors[i]->GetType() != Type::INVERSE)
				ret += "*";
			ret += factors[i]->GetString();
		}
		else
		{
			ret += "*(" + factors[i]->GetString() + ")";
		}
	}
	return ret;
}

std::string mathS::Opposite::GetString()
{
	if (component->Level() <= LEVEL_OPPOSITE)
		return "-" + component->GetString();
	else
		return "-(" + component->GetString() + ")";
}

std::string mathS::Polynomial::GetString()
{
	std::string ret;

	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->Level() < LEVEL_POLYNOMIAL)
		{
			if (items[i]->GetType() != Type::OPPOSITE)
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

std::string mathS::Map::GetString()
{
	return
		(key->Level() < LEVEL_MAP ? key->GetString() : "(" + key->GetString() + ")") + "->" +
		(value->Level() < LEVEL_MAP ? value->GetString() : "(" + value->GetString() + ")");
}

std::string mathS::Equation::GetString()
{
	return
		(left->Level() < LEVEL_EQUATION ? left->GetString() : "(" + left->GetString() + ")") + op +
		(right->Level() < LEVEL_EQUATION ? right->GetString() : "(" + right->GetString() + ")");
}

std::string mathS::ListObject::GetString()
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

mathS::MathObject* mathS::MathObject::DeepCopy(MathObject* obj)
{
	// TODO
	return nullptr;
}
