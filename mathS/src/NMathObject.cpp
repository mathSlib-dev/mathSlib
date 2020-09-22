#include <NMathObject.h>
#include <string>

using namespace mathS;
using namespace mathS::NMath;

std::string mathS::NMath::NAtom::GetString() const
{
	return std::to_string(value);
}

Ptr<NMathObject> mathS::NMath::NList::PartLocate(const std::vector<int>& loc) const
{
	if (loc.size() == 0)
		return New<NMathError>("NList::PartLocate: Empty part specification. ");
	Ptr<NList> pl;
	if(loc[0]>=components.size())
		return New<NMathError>("NList::PartLocate: Part " + std::to_string(loc[0]) + " of " + GetString() + " is out of range.");
	Ptr<NMathObject> p = components[loc[0]];
	for (int i = 1; i < loc.size(); i++)
	{
		if (p->GetType() == NMathObject::Type::LIST)
		{
			pl = Dynamic_cast<NList, NMathObject>(p);
			if (loc[i] >= pl->components.size())
				return New<NMathError>("NList::PartLocate: Part " + std::to_string(loc[i]) + " of " + p->GetString() + " is out of range.");
			p = pl->components[loc[i]];
		}
		else
		{
			if (p->IsAtom())
				return New<NMathError>("NList::PartLocate: Part specification is longer than depth of object. ");
			else if (p->IsError())
				return New<NMathError>("An error has existed: "+p->GetString());
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



Ptr<NMathObject> mathS::NMath::PartLocate(Ptr<NMathObject> obj, const int loc)
{
	if (obj->GetType() == NMathObject::LIST)
	{
		Ptr<NList> pl = Dynamic_cast<NList, NMathObject>(obj);
		if(0<=loc && loc < pl->components.size())
			return pl->components[loc];
		else
			return New<NMathError>("PartLocate: Part " + std::to_string(loc) + " of " + obj->GetString() + " is out of range.");
	}
}

Ptr<NMathObject>& mathS::NMath::PartLocate_ref(Ptr<NMathObject> obj, const std::vector<int>& loc)
{
	Ptr<NList> pl;
	Ptr<NMathObject> p = obj;
	// Locate the first size-1 indices
	for (int i = 1; i < loc.size() - 1; i++)
	{
		pl = Dynamic_cast<NList, NMathObject>(p);
		p = pl->components[loc[i]];
	}
	// return the reference of last index
	pl = Dynamic_cast<NList, NMathObject>(p);
	return pl->components[loc[loc.size() - 1]];
}

Ptr<NMathObject> mathS::NMath::PartLocate(Ptr<NMathObject> obj, const std::vector<int>& loc)
{
	Ptr<NList> pl;
	Ptr<NMathObject> p = obj;
	for (int i = 0; i < loc.size(); i++)
	{
		if (p->GetType() == NMathObject::Type::LIST)
		{
			pl = Dynamic_cast<NList, NMathObject>(p);
			if (loc[i] >= pl->components.size())
				return New<NMathError>("PartLocate: Part " + std::to_string(loc[i]) + " of " + p->GetString() + " is out of range.");
			p = pl->components[loc[i]];
		}
		else
		{
			if (p->IsAtom())
				return New<NMathError>("PartLocate: Part specification is longer than depth of object. ");
			else if (p->IsError())
				return New<NMathError>("An error has existed: " + p->GetString());
		}
	}
	return p;
}