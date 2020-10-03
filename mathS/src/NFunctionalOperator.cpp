#include <NFunctionalOperator.h>

using namespace mathS;
using namespace mathS::NMath;

Ptr<NMathObject> mathS::NMath::Sum(NFuncParamsList f, NParamsList i)
{
	if (f.size() != 1) {
		return New<NMathError>("NMath::Sum: Expected 1 function parameter.");
	}
	if (i.size() == 0) {
		return New<NMathError>("NMath::Sum: At least given 1 argument.");
	}
	if (i.size() != 1) {
		return New<NMathError>("NMath::Sum: More than 1 argument is not supported yet.");
	}
	auto sumbound = i[0];	// {START, END, [STEP]}
	if(sumbound->GetType()!=NMathObject::LIST)
		return New<NMathError>("NMath::Sum: More than 1 argument is not supported yet.");
	auto v = Dynamic_cast<NList, NMathObject>(sumbound);
	if (v->components.size() == 2) {
		if (v->components[0]->IsAtom() && v->components[1]->IsAtom()) {
			double start = v->components[0]->GetValue();
			double end = v->components[1]->GetValue();
			auto ret = f[0]({ New<NAtom>(start) });
			for (double k = start + 1.0; k <= end; k += 1.0) {
				ret = Plus({ ret, f[0]({ New<NAtom>(k) }) });
				if (ret->IsError()) return ret;
			}
			return ret;
		}
		else {
			return New<NMathError>("NMath::Sum: Parameters in {START, END, [STEP]} must be atom value.");
		}
	}
	else if (v->components.size() == 3) {
		if (v->components[0]->IsAtom() && v->components[1]->IsAtom()) {
			double start = v->components[0]->GetValue();
			double end = v->components[1]->GetValue();
			double step = v->components[2]->GetValue();
			auto ret = f[0]({ New<NAtom>(start) });
			for (double k = start + step; k <= end; k += step) {
				ret = Plus({ ret, f[0]({ New<NAtom>(k) }) });
				if (ret->IsError()) return ret;
			}
			return ret;
		}
		else {
			return New<NMathError>("NMath::Sum: Parameters in {START, END, [STEP]} must be atom value.");
		}
	}
	else {
		return New<NMathError>("NMath::Sum: Please follow {START, END, [STEP]}");
	}
}
