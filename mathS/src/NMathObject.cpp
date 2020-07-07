#include <NMathObject.h>
#include <string>

using namespace mathS::NMath;

std::string mathS::NMath::NAtom::GetString() const
{
	return std::to_string(value);
}

NMathObject* mathS::NMath::NList::PartLocate(const std::vector<int>& loc) const
{
	if (loc.size() == 0)
		return new NMathError("NList::PartLocate: Empty part specification. ");
	NList* pl;
	if(loc[0]>=components.size())
		return new NMathError("NList::PartLocate: Part " + std::to_string(loc[0]) + " of " + GetString() + " is out of range.");
	NMathObject* p = components[loc[0]];
	for (int i = 1; i < loc.size(); i++)
	{
		if (p->GetType() == NMathObject::Type::LIST)
		{
			pl = dynamic_cast<NList*>(p);
			if (loc[i] >= pl->components.size())
				return new NMathError("NList::PartLocate: Part " + std::to_string(loc[i]) + " of " + p->GetString() + " is out of range.");
			p = pl->components[loc[i]];
		}
		else
		{
			if (p->IsAtom())
				return new NMathError("NList::PartLocate: Part specification is longer than depth of object. ");
			else if (p->IsError())
				return new NMathError("An error has existed: "+p->GetString());
		}	
	}
	return p;
}

std::string mathS::NMath::NList::GetString() const
{
	std::string ret("{");
	ret += components[0]->GetString();
	for (int i = 1; i < components.size(); i++)
		ret += ","+components[i]->GetString();
	ret += "}";
	return ret;
}



NMathObject* mathS::NMath::PartLocate(NMathObject* obj, const int loc)
{
	if (obj->GetType() == NMathObject::LIST)
	{
		NList* pl = dynamic_cast<NList*>(obj);
		if(0<=loc && loc < pl->components.size())
			return pl->components[loc];
		else
			return new NMathError("PartLocate: Part " + std::to_string(loc) + " of " + obj->GetString() + " is out of range.");
	}
}

NMathObject*& mathS::NMath::PartLocate_ref(NMathObject* obj, const std::vector<int>& loc)
{
	NList* pl;
	NMathObject* p = obj;
	// Locate the first size-1 indices
	for (int i = 1; i < loc.size() - 1; i++)
	{
		pl = dynamic_cast<NList*>(p);
		p = pl->components[loc[i]];
	}
	// return the reference of last index
	pl = dynamic_cast<NList*>(p);
	return pl->components[loc[loc.size() - 1]];
}

NMathObject* mathS::NMath::PartLocate(NMathObject* obj, const std::vector<int>& loc)
{
	NList* pl;
	NMathObject* p = obj;
	for (int i = 0; i < loc.size(); i++)
	{
		if (p->GetType() == NMathObject::Type::LIST)
		{
			pl = dynamic_cast<NList*>(p);
			if (loc[i] >= pl->components.size())
				return new NMathError("PartLocate: Part " + std::to_string(loc[i]) + " of " + p->GetString() + " is out of range.");
			p = pl->components[loc[i]];
		}
		else
		{
			if (p->IsAtom())
				return new NMathError("PartLocate: Part specification is longer than depth of object. ");
			else if (p->IsError())
				return new NMathError("An error has existed: " + p->GetString());
		}
	}
	return p;
}