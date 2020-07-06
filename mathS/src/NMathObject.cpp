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

// NMath + - * /

// Define a shape wise NMathFunction with name o f FUNCNAME, based on operator OP
#define DEFINE_SHAPE_WISE_NMATHFUNC_OP(FUNCNAME, OP) \
		NMathObject* mathS::NMath::FUNCNAME(NMathObject* const a, NMathObject* const b){\
			using NType = NMathObject::Type;\
			NType atype = a->GetType(), btype = b->GetType();\
			/*Case 1 (Basic case)*/\
			if (atype == NType::ATOM && btype == NType::ATOM)\
			{\
				return new NAtom(dynamic_cast<NAtom*>(a)->value OP dynamic_cast<NAtom*>(b)->value);\
			}\
			/*Case 2*/\
			if (atype == NType::ATOM && btype == NType::LIST)\
			{\
				NList* ret = new NList;\
				NList* blist = dynamic_cast<NList*>(b);\
				for (auto it : blist->components)\
					ret->components.push_back(Plus(a, it));\
				return ret;\
			}\
			/*Case 3*/\
			if (atype == NType::LIST && btype == NType::ATOM)\
			{\
				NList* ret = new NList;\
				NList* alist = dynamic_cast<NList*>(a);\
				for (auto it : alist->components)\
					ret->components.push_back(Plus(it, b));\
				return ret;\
			}\
			/*Case 4*/\
			if (atype == NType::LIST && btype == NType::LIST)\
			{\
				NList* alist = dynamic_cast<NList*>(a), * blist = dynamic_cast<NList*>(b);\
				int absize;\
				if ((absize = alist->Size()) != blist->Size())\
					return new NMathError("Shape-wise operation: Shapes of " + alist->GetString() + " and " + blist->GetString() + "do not match. ");\
					\
				NList* ret = new NList;\
				NMathObject* newnode;\
				for (int i = 0; i < absize; i++)\
				{\
					newnode = Plus(alist->components[i], blist->components[i]);\
					if (newnode->IsError())\
					{\
						delete ret;\
						/*This is very important! When an error occurs, 
						temporary result mush be deleted before returning an error flag.*/\
						return newnode; \
					}\
					ret->components.push_back(newnode); \
				}\
				return ret; \
			}\
			/*Obvious Error Case*/\
			if (atype == NType::ERROR || btype == NType::ERROR)\
			{\
				return new NMathError\
				(\
					(atype == NType::ERROR ? dynamic_cast<NMathError*>(a)->info : "")\
					+ (btype == NType::ERROR ? dynamic_cast<NMathError*>(b)->info : "")\
					); \
			}\
		} \

// Pre-defined NMathFunction
DEFINE_SHAPE_WISE_NMATHFUNC_OP(Plus, +);

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Subtract, -);

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Multiply, *);

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Divide, / );


#define DEFINE_SHAPE_WISE_NMATHFUNC_MONO(FUNCNAME, MONO) \
NMathObject* mathS::NMath::FUNCNAME(NMathObject* const a)\
{\
	using NType = NMathObject::Type;\
	NType atype = a->GetType();\
	/*Case 1 (Basic case)*/\
	if (atype == NType::ATOM)\
	{\
		return new NAtom(MONO(dynamic_cast<NAtom*>(a)->value));\
	}\
	/*Case 2*/\
	if (atype == NType::LIST)\
	{\
		NList* ret = new NList;\
		NMathObject* newnode;\
		for (auto it : dynamic_cast<NList*>(a)->components)\
		{\
			newnode = FUNCNAME(it);\
			if (newnode->IsError())\
			{\
				delete ret;\
				return newnode;\
			}\
			ret->components.push_back(Sin(it));\
		}\
		return ret;\
	}\
	/*Case 3*/\
	if (atype == NType::ERROR)\
	{\
		return new NMathError(a->GetString());\
	}\
	else\
	{\
		return new NMathError("Mono: Unkown type.");\
	}\
}\

//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Sin, sin);
//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Cos, cos);
//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Tan, tan);
//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ArcSin, asin);
//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ArcCos, acos);
//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ArcTan, atan);
//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Log, log);
//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Exp, exp);
//DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Floor, floor);



/*
Prototype of DEFINE_SHAPE_WISE_NMATHFUNC_MONO

NMathObject* mathS::NMath::Sin(NMathObject* const a)
{
	using NType = NMathObject::Type;
	NType atype = a->GetType();
	// Case 1 (Basic case)
	if (atype == NType::ATOM)
	{
		return new NAtom(sin(dynamic_cast<NAtom*>(a)->value));
	}
	// Case 2
	if (atype == NType::LIST)
	{
		NList* ret = new NList;
		NMathObject* newnode;
		for (auto it : dynamic_cast<NList*>(a)->components)
		{
			newnode = Sin(it);
			if (newnode->IsError())
			{
				delete ret;
				return newnode;
			}
			ret->components.push_back(Sin(it));
		}
		return ret;
	}
	// Case 3
	if (atype == NType::ERROR)
	{
		return new NMathError(a->GetString());
	}
}
*/

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
				return new NMathError("NList::PartLocate: Part " + std::to_string(loc[i]) + " of " + p->GetString() + " is out of range.");
			p = pl->components[loc[i]];
		}
		else
		{
			if (p->IsAtom())
				return new NMathError("NList::PartLocate: Part specification is longer than depth of object. ");
			else if (p->IsError())
				return new NMathError("An error has existed: " + p->GetString());
		}
	}
	return p;
}
