#include <LBAssembler.h>

using namespace mathS;

NMath::NFunction mathS::Assembler::Assemble(MathObject* const expr, const std::vector<std::string>& params)
{
	// TODO

	switch (expr->GetType())
	{
	case MathObject::ATOM:
	{
		Atom* atomexpr = dynamic_cast<Atom*>(expr);
		switch (atomexpr->AtomType())
		{
		case MathObject::NUMBER: // Number 直接返回值
			return NMath::NFunctionAtom(atomexpr->NumberValue());
			break;
		case MathObject::VARIABLE:	// Variable 对应参数列表
		{
			int i = 0;
			for (; i < params.size(); i++)
				if (params[i] == atomexpr->str) break;
			if (i >= params.size())
				return NMath::NFunctionError("Assembler::Assemble: Variable " + atomexpr->str + " is not specified as a parameter.");
			return [i](NMath::NMathObject* param) {
				return NMath::PartLocate(param, i);
			};
		}
			break;
		case MathObject::STRING:	// String 不支持
			return NMath::NFunctionError("Assembler::Assemle: String type object cannot converted to NMathObject.");
			break;
		default:
			return NMath::NFunctionError("MathObject Error. ");
			break;
		}
	}
		break;
	case MathObject::FUNCTION:
	{

	}
		break;
	case MathObject::VECTOR:

		break;
	case MathObject::POWER:

		break;
	default:
		break;
	}
	return NMath::NFunction();
}
