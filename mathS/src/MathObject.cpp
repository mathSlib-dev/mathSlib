#include <MathObject.h>

std::string mathS::Vector::GetString() const
{
	return "{" + list->GetString() + "}";
}

mathS::MathObject* mathS::Vector::DeepCopy()
{
	Vector* ret= new Vector();
	ret->list = dynamic_cast<ListObject*>(list->DeepCopy());
	return ret;
}

std::string mathS::Function::GetString() const
{
	if (function->Level() <= LEVEL_FUNCTION)
		return function->GetString() + "[" + parameter->GetString() + "]";
	else
		return "(" + function->GetString() + ")" + "[" + parameter->GetString() + "]";
}

mathS::MathObject* mathS::Function::DeepCopy()
{
	Function* ret = new Function();
	ret->function = function->DeepCopy();
	ret->parameter = parameter->DeepCopy();
	return ret;
}

std::string mathS::Locate::GetString() const
{
	if (object->Level() <= LEVEL_LOCATE)
		return object->GetString() + "[[" + location->GetString() + "]]";
	else
		return "(" + object->GetString() + ")" + "[[" + object->GetString() + "]]";
}

mathS::MathObject* mathS::Locate::DeepCopy()
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
		(exponent->Level() <= LEVEL_POWER ? base->GetString() : "(" + exponent->GetString() + ")");
}

mathS::MathObject* mathS::Power::DeepCopy()
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

mathS::MathObject* mathS::Inverse::DeepCopy()
{
	Inverse* ret = new Inverse();
	ret->component = component->DeepCopy();
	return ret;
}

std::string mathS::Item::GetString() const
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

mathS::MathObject* mathS::Item::DeepCopy()
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

mathS::MathObject* mathS::Opposite::DeepCopy()
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

mathS::MathObject* mathS::Polynomial::DeepCopy()
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

mathS::MathObject* mathS::Map::DeepCopy()
{
	Map* ret = new Map();
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

mathS::MathObject* mathS::Compare::DeepCopy()
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

mathS::MathObject* mathS::ListObject::DeepCopy()
{
	ListObject* ret = new ListObject();
	ret->components.reserve(components.size());
	for (auto it : components)
		ret->components.push_back(it->DeepCopy());
	return ret;
}

std::string mathS::Atom::GetString() const
{
	return str;
}

mathS::MathObject* mathS::Atom::DeepCopy()
{
	return new Atom(str);
}

mathS::MathObject* mathS::ErrorObject::DeepCopy()
{
	return new ErrorObject(info);
}

mathS::MathObject* mathS::EmptyObject::DeepCopy()
{
	return new EmptyObject();
}
