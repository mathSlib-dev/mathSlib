#include <LBAssembler.h>

using namespace mathS;
using namespace mathS::NMath;

void mathS::Assembler::InitializeSTDLIB()
{
	function_table["sin"] = Sin;
	function_table["cos"] = Cos;
	function_table["log"] = Log;
	function_table["tan"] = Tan;
	function_table["exp"] = Exp;
	function_table["arcsin"] = ArcSin;
	function_table["arccos"] = ArcCos;
	function_table["arctan"] = ArcTan;
	function_table["floor"] = Floor;

	constant_table["PI"] = NFunctionAtom(3.141592653589793);
	constant_table["E"] = NFunctionAtom(2.7182818284590452);
}

NMath::NFunction mathS::Assembler::Assemble(Ptr<MathObject> expr, std::vector<std::string>& params)
{
	// TODO

	switch (expr->GetType()){
	case MathObject::ATOM:{
		Ptr<Atom> atomexpr = Dynamic_cast<Atom, MathObject>(expr);
		switch (atomexpr->AtomType())
		{
		case MathObject::NUMBER: // Number 直接返回值
			return NFunctionAtom(atomexpr->NumberValue());
			break;
		case MathObject::VARIABLE:{	// Variable 对应参数列表
			int i = 0;
			auto it_c = constant_table.find(atomexpr->str);
			if (it_c != constant_table.end())
				return it_c->second;
			for (; i < params.size(); i++)
				if (params[i] == atomexpr->str) break;
			if (i >= params.size())
				return NMath::NFunctionError("Assemble: Variable " + atomexpr->str + " is not specified as a parameter.");
			return [i](Ptr<NMathObject> param) {
				return PartLocate(param, i);
			};
		}
			break;
		case MathObject::STRING:	// String 不支持
			return NMath::NFunctionError("Assemble: Cannot convert String type object to NMathObject.");
			break;
		default:
			return NMath::NFunctionError("MathObject Error. ");
			break;
		}
		break;
	}
		
	case MathObject::FUNCTION: {	// 函数组装
		Ptr<Function> funcexpr = Dynamic_cast<Function, MathObject>(expr);
		auto it_ffunc = function_table.find(funcexpr->function->GetString());
		if (it_ffunc == function_table.end()) 
			return NFunctionError("Assemble: No such function as " + funcexpr->function->GetString());

		NFunction ffunc = it_ffunc->second;
		NFunction fparafunc = Assemble(funcexpr->parameter, params);
		return [ffunc, fparafunc](Ptr<NMathObject> param) {
			return ffunc(fparafunc(param));
		};

		break;
	}
	case MathObject::FUNCOPERATOR: {
		return NFunctionError("Assemble: FunctionalOperator is not supported yet");
		break;
	}
	case MathObject::VECTOR: {
		Ptr<Vector> vecexpr = Dynamic_cast<Vector, MathObject>(expr);
		std::vector<NFunction> elemfuncs;
		for (auto& it : vecexpr->list->components) {
			elemfuncs.push_back(Assemble(it, params));
		}
		return [elemfuncs](Ptr<NMathObject> param) {
			Ptr<NList> ret = New<NList>();
			for (auto& it : elemfuncs) {
				ret->components.push_back(it(param));
			}
			return Dynamic_cast<NMathObject, NList>(ret);
		};
		break;
	}
	case MathObject::POWER: {
		Ptr<Power> powexpr = Dynamic_cast<Power, MathObject>(expr);
		NFunction fbase = Assemble(powexpr->base, params);
		NFunction fexp = Assemble(powexpr->exponent, params);
		return [fbase, fexp](Ptr<NMathObject> param) {
			return NMath::Power(fbase(param), fexp(param));
		};
		break;
	}	
	case MathObject::INVERSE: {
		Ptr<Inverse> invexpr = Dynamic_cast<Inverse, MathObject>(expr);
		NFunction t = Assemble(invexpr->component, params);
		return [t](Ptr<NMathObject> param) {
			return Divide(New<NAtom>(1.), t(param));
		};
		break;
	}
	case MathObject::ITEM: {
		Ptr<Item> itmexpr = Dynamic_cast<Item, MathObject>(expr);
		std::vector<NFunction> fctfuncs;
		for (auto& it : itmexpr->factors) {
			fctfuncs.push_back(Assemble(it, params));
		}
		return [fctfuncs](Ptr<NMathObject> param) {
			Ptr<NMathObject> ret = fctfuncs[0](param);
			for (int i = 1; i < fctfuncs.size(); i++) {
				ret = Multiply(ret, fctfuncs[i](param));
			}
			return ret;
		};
		break;
	}
	case MathObject::OPPOSITE:{
		Ptr<Opposite> oppoexpr = Dynamic_cast<Opposite, MathObject>(expr);
		NFunction t = Assemble(oppoexpr->component, params);
		return [t](Ptr<NMathObject> param) {
			return Subtract(New<NAtom>(0.), t(param));
		};
		break;
	}
	case MathObject::POLYNOMIAL: {
		Ptr<Polynomial> polyexpr = Dynamic_cast<Polynomial, MathObject>(expr);
		std::vector<NFunction> itmfuncs;
		for (auto& it : polyexpr->items) {
			itmfuncs.push_back(Assemble(it, params));
		}
		return [itmfuncs](Ptr<NMathObject> param) {
			Ptr<NMathObject> ret = itmfuncs[0](param);
			for (int i = 1; i < itmfuncs.size(); i++) {
				ret = Plus(ret, itmfuncs[i](param));
			}
			return ret;
		};
		break;
	}
	/*
	case MathObject::COMPARE: {
		// 返回0 1
		break;
	}
	case MathObject::LIST: {
		// 禁止不在Vector内的ListObject
		break;
	}*/
	default:
		return NFunctionError("Assemble: Not supported yet");
		break;
	}
}
