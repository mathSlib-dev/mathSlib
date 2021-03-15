#include <LBAssembler.h>

using namespace mathS;
using namespace mathS::NMath;

void mathS::Assembler::InitializeSTDLIB()
{
	function_table["Sin"] = Sin;
	function_table["Cos"] = Cos;
	function_table["Log"] = Log;
	function_table["Tan"] = Tan;
	function_table["Exp"] = Exp;
	function_table["ASin"] = ASin;
	function_table["ACos"] = ACos;
	function_table["ATan"] = ATan;
	function_table["Floor"] = Floor;

	constant_table["PI"] = NFunctionAtom(3.141592653589793);
	constant_table["E"] = NFunctionAtom(2.7182818284590452);

	fop_table["Sum"] = Sum;
	fop_table["Product"] = Product;
	// fop_table["NDerivative"] = NDerivative;
}

NMath::NFunction mathS::Assembler::Assemble(Ptr<MathObject> expr, std::vector<std::string>& paramsstr)
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
			for (; i < paramsstr.size(); i++)
				if (paramsstr[i] == atomexpr->str) break;
			if (i >= paramsstr.size())
				return NMath::NFunctionError("Assemble: Variable " + atomexpr->str + " is not specified as a parameter in its context");
			return [i](const NParamsList& params) {
				if (i >= params.size())
					return Dynamic_cast<NMathObject>(New<NMathError>("NFunction: Missing parameter."));
				return params[i];
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
		std::vector<NFunction> fpara;	// funcexpr->parameter 对于变量的函数
		for (auto& it : funcexpr->parameter) 
			fpara.push_back(Assemble(it, paramsstr));
		
		return [ffunc, fpara](const NParamsList& params) {
			NParamsList args;
			args.reserve(fpara.size());
			for (auto& it : fpara) args.push_back(it(params));
			return ffunc(args);
		};

		break;
	}
	case MathObject::FUNCOPERATOR: {
		Ptr<FunctionalOperator> fopexpr = Dynamic_cast<FunctionalOperator, MathObject>(expr);
		auto itfop = fop_table.find(fopexpr->function->GetString());
		if (itfop == fop_table.end())
			return NFunctionError("Assemble: No such functional operator as " + fopexpr->function->GetString());
		// 内部变量与外部变量名称冲突检查
		// TODO
		// 
		NFuncOperator nfop = itfop->second;

		std::vector<std::string> paramsstr2;
		paramsstr2.reserve(fopexpr->variables.size() + paramsstr.size());
		for (auto it : fopexpr->variables)
			paramsstr2.push_back(it->GetString());
		paramsstr2.insert(paramsstr2.end(), paramsstr.begin(), paramsstr.end());	// params2 是   variable & params

		std::vector<NFunction> ffparas;					// fparameter对于变量和内部变量的函数
		ffparas.reserve(fopexpr->fparameter.size());
		for (auto& it : fopexpr->fparameter)
			ffparas.push_back(Assemble(it, paramsstr2));

		std::vector<NFunction> fparas;					// parameter对于变量的函数
		fparas.reserve(fopexpr->parameter.size());
		for (auto& it : fopexpr->parameter)
			fparas.push_back(Assemble(it, paramsstr));

		return [nfop, ffparas, fparas](const NParamsList& params) {
			// ffparas 是对 外部变量parameters和内部variables的函数
			// fs 是 给定了parameter时， ffparas对variables的函数
			NFuncParamsList fs;
			fs.reserve(ffparas.size());
			for(auto& it:ffparas)
				fs.push_back([it, params](const NParamsList& vars){
					// 将 params 和 vars 合并成ffparas的参数
					NParamsList paramsvars;
					paramsvars.reserve(params.size() + vars.size());
					paramsvars.insert(paramsvars.end(), vars.begin(), vars.end());
					paramsvars.insert(paramsvars.begin(), params.begin(), params.end());
					return it(paramsvars);
				});
			NParamsList args;
			args.reserve(fparas.size());
			for (auto& it : fparas) args.push_back(it(params));
			return nfop(fs, args);						// 调用 fop 
		};
		break;
	}
	case MathObject::VECTOR: {
		Ptr<Vector> vecexpr = Dynamic_cast<Vector, MathObject>(expr);
		std::vector<NFunction> elemfuncs;
		for (auto& it : vecexpr->components) {
			elemfuncs.push_back(Assemble(it, paramsstr));
		}
		return [elemfuncs](const NParamsList& params) {
			Ptr<NList> ret = New<NList>();
			for (auto& it : elemfuncs) {
				ret->components.push_back(it(params));
			}
			return Dynamic_cast<NMathObject, NList>(ret);
		};
		break;
	}
	case MathObject::POWER: {
		Ptr<Power> powexpr = Dynamic_cast<Power, MathObject>(expr);
		NFunction fbase = Assemble(powexpr->base, paramsstr);
		NFunction fexp = Assemble(powexpr->exponent, paramsstr);
		return [fbase, fexp](const NParamsList& params) {
			return NMath::Power({ fbase(params), fexp(params) });
		};
		break;
	}	
	case MathObject::INVERSE: {
		Ptr<Inverse> invexpr = Dynamic_cast<Inverse, MathObject>(expr);
		NFunction t = Assemble(invexpr->component, paramsstr);
		return [t](const NParamsList& params) {
			return Divide({ New<NAtom>(1.), t(params) });
		};
		break;
	}
	case MathObject::ITEM: {
		Ptr<Item> itmexpr = Dynamic_cast<Item, MathObject>(expr);
		std::vector<NFunction> fctfuncs;
		for (auto& it : itmexpr->factors) {
			fctfuncs.push_back(Assemble(it, paramsstr));
		}
		return [fctfuncs](const NParamsList& params) {
			Ptr<NMathObject> ret = fctfuncs[0](params);
			for (int i = 1; i < fctfuncs.size(); i++) {
				ret = Multiply({ ret, fctfuncs[i](params) });
			}
			return ret;
		};
		break;
	}
	case MathObject::OPPOSITE:{
		Ptr<Opposite> oppoexpr = Dynamic_cast<Opposite, MathObject>(expr);
		NFunction t = Assemble(oppoexpr->component, paramsstr);
		return [t](const NParamsList& params) {
			return Subtract({ New<NAtom>(0.), t(params) });
		};
		break;
	}
	case MathObject::POLYNOMIAL: {
		Ptr<Polynomial> polyexpr = Dynamic_cast<Polynomial, MathObject>(expr);
		std::vector<NFunction> itmfuncs;
		for (auto& it : polyexpr->items) {
			itmfuncs.push_back(Assemble(it, paramsstr));
		}
		return [itmfuncs](const NParamsList& params) {
			Ptr<NMathObject> ret = itmfuncs[0](params);
			for (int i = 1; i < itmfuncs.size(); i++) {
				ret = Plus({ ret, itmfuncs[i](params) });
			}
			return ret;
		};
		break;
	}
	case MathObject::COMPARE: {
		Ptr<Compare> cmpexpr = Dynamic_cast<Compare, MathObject>(expr);
		NFunction fleft = Assemble(cmpexpr->left, paramsstr);
		NFunction fright = Assemble(cmpexpr->right, paramsstr);
		if (cmpexpr->op == "<") {
			return [fleft, fright](const NParamsList& params) {
				return NMath::Less({ fleft(params), fright(params) });
			};
		}
		else if(cmpexpr->op == ">") {
			return [fleft, fright](const NParamsList& params) {
				return NMath::Greater({ fleft(params), fright(params) });
			};
		}
		else if (cmpexpr->op == "<=") {
			return [fleft, fright](const NParamsList& params) {
				return NMath::Lesseq({ fleft(params), fright(params) });
			};
		}
		else if (cmpexpr->op == ">=") {
			return [fleft, fright](const NParamsList& params) {
				return NMath::Greatereq({ fleft(params), fright(params) });
			};
		}
		else {
			return NFunctionError("Assemble: Unknown compare operator");
		}
		break;
	}
	default:
		return NFunctionError("Assemble: Not supported yet");
		break;
	}
}
