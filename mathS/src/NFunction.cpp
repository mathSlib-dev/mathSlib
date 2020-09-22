#include<NFunction.h>

// NMath + - * /
using namespace mathS;
using namespace mathS::NMath;


// Define a shape wise NMathFunction with name o f FUNCNAME, based on operator OP
#define DEFINE_SHAPE_WISE_NMATHFUNC_OP(FUNCNAME, OP) \
		Ptr<NMathObject> mathS::NMath::FUNCNAME(Ptr<NMathObject> const a, Ptr<NMathObject> const b){\
			using NType = NMathObject::Type;\
			NType atype = a->GetType(), btype = b->GetType();\
			/*Case 1 (Basic case)*/\
			if (atype == NType::ATOM && btype == NType::ATOM)\
			{\
				return New<NAtom>(Dynamic_cast<NAtom, NMathObject>(a)->value OP Dynamic_cast<NAtom, NMathObject>(b)->value);\
			}\
			/*Case 2*/\
			if (atype == NType::ATOM && btype == NType::LIST)\
			{\
				Ptr<NList> ret = New<NList>();\
				Ptr<NList> blist = Dynamic_cast<NList, NMathObject>(b);\
				for (auto it : blist->components)\
					ret->components.push_back(FUNCNAME(a, it));\
				return ret;\
			}\
			/*Case 3*/\
			if (atype == NType::LIST && btype == NType::ATOM)\
			{\
				Ptr<NList> ret = New<NList>();\
				Ptr<NList> alist = Dynamic_cast<NList, NMathObject>(a);\
				for (auto it : alist->components)\
					ret->components.push_back(FUNCNAME(it, b));\
				return ret;\
			}\
			/*Case 4*/\
			if (atype == NType::LIST && btype == NType::LIST)\
			{\
				Ptr<NList> alist = Dynamic_cast<NList, NMathObject>(a), blist = Dynamic_cast<NList, NMathObject>(b);\
				int absize;\
				if ((absize = alist->Size()) != blist->Size())\
					return New<NMathError>("Shape-wise operation: Shapes of " + alist->GetString() + " and " + blist->GetString() + "do not match. ");\
					\
				Ptr<NList> ret = New<NList>();\
				Ptr<NMathObject> newnode;\
				for (int i = 0; i < absize; i++)\
				{\
					newnode = Plus(alist->components[i], blist->components[i]);\
					if (newnode->IsError())\
					{\
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
								return New<NMathError>\
								(\
									(atype == NType::ERROR ? Dynamic_cast<NMathError, NMathObject>(a)->info : "")\
									+ (btype == NType::ERROR ? Dynamic_cast<NMathError, NMathObject>(b)->info : "")\
									); \
							}\
		} \

// Pre-defined NMathFunction +, -, *, /
DEFINE_SHAPE_WISE_NMATHFUNC_OP(Plus, +);

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Subtract, -);

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Multiply, *);

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Divide, / );


#define DEFINE_SHAPE_WISE_NMATHFUNC_MONO(FUNCNAME, MONO) \
Ptr<NMathObject> mathS::NMath::FUNCNAME(Ptr<NMathObject> const a)\
{\
	using NType = NMathObject::Type;\
	NType atype = a->GetType();\
	/*Case 1 (Basic case)*/\
	if (atype == NType::ATOM)\
	{\
		return New<NAtom>(MONO(Dynamic_cast<NAtom, NMathObject>(a)->value));\
	}\
	/*Case 2*/\
	if (atype == NType::LIST)\
	{\
		Ptr<NList> ret = New<NList>();\
		Ptr<NMathObject> newnode;\
		for (auto it : Dynamic_cast<NList, NMathObject>(a)->components)\
		{\
			newnode = FUNCNAME(it);\
			if (newnode->IsError())\
			{\
				return newnode;\
			}\
			ret->components.push_back(FUNCNAME(it));\
		}\
		return ret;\
	}\
	/*Case 3*/\
	if (atype == NType::ERROR)\
	{\
		return New<NMathError>(a->GetString());\
	}\
	else\
	{\
		return New<NMathError>("Mono: Unkown type.");\
	}\
}\


DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Sin, sin);
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Cos, cos);
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Tan, tan);
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ArcSin, asin);
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ArcCos, acos);
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ArcTan, atan);
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Log, log);
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Exp, exp);
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Floor, floor);



						/*
						Prototype of DEFINE_SHAPE_WISE_NMATHFUNC_MONO

						Ptr<NMathObject> mathS::NMath::Sin(Ptr<NMathObject> const a)
						{
							using NType = NMathObject::Type;
							NType atype = a->GetType();
							// Case 1 (Basic case)
							if (atype == NType::ATOM)
							{
								return New<NAtom>(sin(dynamic_cast<NAtom*>(a)->value));
							}
							// Case 2
							if (atype == NType::LIST)
							{
								Ptr<NList> ret = New<NList>();
								Ptr<NMathObject> newnode;
								for (auto it : dynamic_cast<Ptr<NList>>(a)->components)
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
								return New<NMathError>(a->GetString());
							}
						}
						*/

NFunction mathS::NMath::NFunctionError(const std::string info)
{
	return [info](Ptr<NMathObject>) {
		return Dynamic_cast<NMathObject, NMathError>(New<NMathError>(info));
	};
}

NFunction mathS::NMath::NFunctionAtom(const NValueType v)
{
	return [v](Ptr<NMathObject>) {
		return Dynamic_cast<NMathObject, NAtom>(New<NAtom>(v));
	};
}

						