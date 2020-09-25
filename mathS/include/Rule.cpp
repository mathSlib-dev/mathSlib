#include "Rule.h"

using namespace mathS;

bool Rule::Match(const Ptr<MathObject> pattern, const Ptr<MathObject> obj, std::map<std::string, Ptr<MathObject>>& table)
{
	MathObject::Type mtype = pattern->GetType();
	if (obj->GetType() != mtype)
		return false;
	switch (mtype)
	{
	case MathObject::ATOM: {
		Ptr<Atom> atom_pattern = Dynamic_cast<Atom>(pattern);
		break;
	}
	default:
		break;
	}
}
