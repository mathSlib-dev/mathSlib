#include<NFunction.h>
#include<ExtendedCMath.h>

using namespace mathS;
using namespace mathS::NMath;

NFunction mathS::NMath::NFunctionError(const std::string info)
{
	return [info](const NParamsList&) {
		return Dynamic_cast<NMathObject, NMathError>(New<NMathError>(info));
	};
}

NFunction mathS::NMath::NFunctionAtom(const NValueType v)
{
	return [v](const NParamsList&) {
		return Dynamic_cast<NMathObject, NAtom>(New<NAtom>(v));
	};
}



// Define a shape wise NMathFunction with name of FUNCNAME, based on operator OP
#define DEFINE_SHAPE_WISE_NMATHFUNC_OP(FUNCNAME, OP, FNAMESTRING) \
		Ptr<NMathObject> mathS::NMath::FUNCNAME(const NParamsList& params){\
			if (params.size() != 2)\
				return New<NMathError>(std::string("NMath::") + FNAMESTRING + ": Must take 2 arguments but " + std::to_string(params.size()) + " got instead. ");\
			auto& a = params[0];\
			auto& b = params[1]; \
			using NType = NMathObject::Type;\
			NType atype = a->GetType(), btype = b->GetType();\
			/*Case 1 (Basic case)*/\
			if (atype == NType::ATOM && btype == NType::ATOM)\
				return New<NAtom>(Dynamic_cast<NAtom, NMathObject>(a)->value OP Dynamic_cast<NAtom, NMathObject>(b)->value);\
			/*Case 2*/\
			if (atype == NType::ATOM && btype == NType::LIST){\
				Ptr<NList> ret = New<NList>();\
				Ptr<NList> blist = Dynamic_cast<NList, NMathObject>(b);\
				for (auto it : blist->components)\
					ret->components.push_back(FUNCNAME({a, it}));\
				return ret;\
			}\
			/*Case 3*/\
			if (atype == NType::LIST && btype == NType::ATOM){\
				Ptr<NList> ret = New<NList>();\
				Ptr<NList> alist = Dynamic_cast<NList, NMathObject>(a);\
				for (auto it : alist->components)\
					ret->components.push_back(FUNCNAME({it, b}));\
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
					newnode = FUNCNAME({alist->components[i], blist->components[i]});\
					if (newnode->IsError())\
							return newnode; \
					ret->components.push_back(newnode); \
				}\
				return ret; \
			}\
			/*Obvious Error Case*/\
			if (atype == NType::ERROR || btype == NType::ERROR) {\
				return New<NMathError>\
					(\
						(atype == NType::ERROR ? Dynamic_cast<NMathError, NMathObject>(a)->info : "")\
						+ (btype == NType::ERROR ? Dynamic_cast<NMathError, NMathObject>(b)->info : "")\
					); \
			}\
		} \

// Pre-defined NMathFunction +, -, *, /
DEFINE_SHAPE_WISE_NMATHFUNC_OP(Plus, +, "Plus");

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Subtract, -, "Subtract");

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Multiply, *, "Multiply");

DEFINE_SHAPE_WISE_NMATHFUNC_OP(Divide, / ,"Divide");


#define DEFINE_SHAPE_WISE_NMATHFUNC_MONO(FUNCNAME, MONO, FNAMESTRING) \
Ptr<NMathObject> mathS::NMath::FUNCNAME(const NParamsList& params)\
{\
	if(params.size()!=1)\
		return New<NMathError>(std::string("NMath::")+FNAMESTRING+": Must take 1 argument but got "+ std::to_string(params.size()));\
	auto& a = params[0];\
	using NType = NMathObject::Type;\
	NType atype = a->GetType();\
	/*Case 1 (Basic case)*/\
	if (atype == NType::ATOM)\
		return New<NAtom>(MONO(Dynamic_cast<NAtom, NMathObject>(a)->value));\
	/*Case 2*/\
	if (atype == NType::LIST)\
	{\
		Ptr<NList> ret = New<NList>();\
		Ptr<NMathObject> newnode;\
		for (auto it : Dynamic_cast<NList, NMathObject>(a)->components)\
		{\
			newnode = FUNCNAME({it});\
			if (newnode->IsError())\
				return newnode;\
			ret->components.push_back(FUNCNAME({it}));\
		}\
		return ret;\
	}\
	/*Case 3*/\
	if (atype == NType::ERROR)\
		return a;\
	else\
	{\
		return New<NMathError>("Mono: Unkown type.");\
	}\
}\


DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Sin, sin, "Sin");
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Cos, cos, "Cos");
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Tan, tan, "Tan");
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ASin, asin, "ASin");
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ACos, acos, "ACos");
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(ATan, atan, "ATan");
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Log, log, "Log");
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Exp, exp, "Exp");
DEFINE_SHAPE_WISE_NMATHFUNC_MONO(Floor, floor, "Floor");


// Define a shape wise NMathFunction with name o f FUNCNAME, based on operator OP
#define DEFINE_SHAPE_WISE_NMATHFUNC2(FUNCNAME, CFUNC, FNAMESTRING) \
		Ptr<NMathObject> mathS::NMath::FUNCNAME(const NParamsList& params){\
			if (params.size() != 2)\
				return New<NMathError>(std::string("NMath::") + FNAMESTRING + ": Must take 2 arguments but " + std::to_string(params.size()) + " got instead. ");\
			auto& a = params[0];\
			auto& b = params[1]; \
			using NType = NMathObject::Type;\
			NType atype = a->GetType(), btype = b->GetType();\
			/*Case 1 (Basic case)*/\
			if (atype == NType::ATOM && btype == NType::ATOM)\
				return New<NAtom>(CFUNC(Dynamic_cast<NAtom, NMathObject>(a)->value, Dynamic_cast<NAtom, NMathObject>(b)->value));\
			/*Case 2*/\
			if (atype == NType::ATOM && btype == NType::LIST){\
				Ptr<NList> ret = New<NList>();\
				Ptr<NList> blist = Dynamic_cast<NList, NMathObject>(b);\
				for (auto it : blist->components)\
					ret->components.push_back(FUNCNAME({a, it}));\
				return ret;\
			}\
			/*Case 3*/\
			if (atype == NType::LIST && btype == NType::ATOM){\
				Ptr<NList> ret = New<NList>();\
				Ptr<NList> alist = Dynamic_cast<NList, NMathObject>(a);\
				for (auto it : alist->components)\
					ret->components.push_back(FUNCNAME({it, b}));\
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
					newnode = FUNCNAME({alist->components[i], blist->components[i]});\
					if (newnode->IsError())\
							return newnode; \
					ret->components.push_back(newnode); \
				}\
				return ret; \
			}\
			/*Obvious Error Case*/\
			if (atype == NType::ERROR || btype == NType::ERROR) {\
				return New<NMathError>\
					(\
						(atype == NType::ERROR ? Dynamic_cast<NMathError, NMathObject>(a)->info : "")\
						+ (btype == NType::ERROR ? Dynamic_cast<NMathError, NMathObject>(b)->info : "")\
					); \
			}\
		} \



DEFINE_SHAPE_WISE_NMATHFUNC2(Less, double_less, "Less");
DEFINE_SHAPE_WISE_NMATHFUNC2(Greater, double_greater, "Greater");
DEFINE_SHAPE_WISE_NMATHFUNC2(Lesseq, double_lesseq, "LessOrEqual");
DEFINE_SHAPE_WISE_NMATHFUNC2(Greatereq, double_greatereq, "GreaterOrEqual");
DEFINE_SHAPE_WISE_NMATHFUNC2(Power, pow, "Power");