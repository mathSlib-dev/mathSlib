#include <LBAssembler.h>

using namespace mathS;

NMath::NMathFunction mathS::Assembler::Assemble(const MathObject* expr, const std::vector<std::string>& params)
{
	// TODO

	switch (expr->GetType())
	{
	case MathObject::ATOM:

		break;
	case MathObject::FUNCTION:

		break;
	case MathObject::VECTOR:

		break;
	case MathObject::POWER:

		break;
	default:
		break;
	}
	return NMath::NMathFunction();
}
