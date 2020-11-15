#include <NFunctionalOperator.h>

using namespace mathS;
using namespace mathS::NMath;

Ptr<NMathObject> mathS::NMath::Product(NFuncParamsList f, NParamsList i)
{
    if (f.size() != 1) {
        return New<NMathError>("NMath::Product: Expected 1 function parameter.");
    }
    if (i.size() == 0) {
        return New<NMathError>("NMath::Product: At least given 1 argument.");
    }
    if (i.size() == 1) {
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
                    ret = Multiply({ ret, f[0]({ New<NAtom>(k) }) });
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
                    ret = Multiply({ ret, f[0]({ New<NAtom>(k) }) });
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
    double st[i.size()], ed[i.size()], sp[i.size()];
    NParamsList paramsList;
    for (int t = 0; t < i.size(); t++) {
        auto sumbound = i[t];	// {START, END, [STEP]}
        if (sumbound->GetType() != NMathObject::LIST) return New<NMathError>("NMath::Product: More than 1 argument is not supported yet.");
        auto v = Dynamic_cast<NList, NMathObject>(sumbound);
        double step = 1.0f;
        if (v->components.size() == 3) {
            if (v->components[2]->GetType() != NMathObject::ATOM) return New<NMathError>("NMath::Product: Parameters in {START, END, [STEP]} must be atom value.");
            step = v->components[2]->GetValue();
        } else if (v->components.size() != 2) return New<NMathError>("NMath::Product: Please follow {START, END, [STEP]}");
        if (v->components[0]->GetType() != NMathObject::ATOM || v->components[1]->GetType() != NMathObject::ATOM) return New<NMathError>("NMath::Sum: Parameters in {START, END, [STEP]} must be atom value.");
        paramsList.push_back(New<NAtom>(st[t] = v->components[0]->GetValue()));
        ed[t] = v->components[1]->GetValue();
        sp[t] = step;
    }
    auto lst = i.size() - 1;
    auto prelst = i.size() - 2;
    auto ret = Static_cast<NMathObject, NAtom>(New<NAtom>(1));
    auto step = sp[lst];
    auto prestep = sp[prelst];
    auto lstpAtomPtr = Dynamic_cast<NAtom, NMathObject>(paramsList[lst]);
    auto prelstpAtomPtr = Dynamic_cast<NAtom, NMathObject>(paramsList[prelst]);
    while (paramsList[0]->GetValue() <= ed[0]) {
        for (double t = st[lst]; t <= ed[lst]; t += step) {
            lstpAtomPtr->SetValue(t);
            ret = Multiply({ ret, f[0](paramsList) });
        }
        lstpAtomPtr->SetValue(st[lst]);
        prelstpAtomPtr->SetValue(prelstpAtomPtr->GetValue() + prestep);
        for (unsigned t = prelst; ~t; ) {
            if (paramsList[t]->GetValue() > ed[t]) {
                if (t == 0) {// 说明这是最后一轮回
                    if (paramsList[t]->GetValue() + sp[t] > ed[t]) return ret;
                    Dynamic_cast<NAtom, NMathObject>(paramsList[t])->SetValue(st[t]);
                    break;
                }
                Dynamic_cast<NAtom, NMathObject>(paramsList[t])->SetValue(st[t]);
                Dynamic_cast<NAtom, NMathObject>(paramsList[--t])->SetValue(paramsList[t]->GetValue() + sp[t]);
            } else break;
        }
    }
    return ret;
}
Ptr<NMathObject> mathS::NMath::Sum(NFuncParamsList f, NParamsList i)
{
	if (f.size() != 1) {
		return New<NMathError>("NMath::Sum: Expected 1 function parameter.");
	}
	if (i.size() == 0) {
		return New<NMathError>("NMath::Sum: At least given 1 argument.");
	}
    if (i.size() == 1) {
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
    double st[i.size()], ed[i.size()], sp[i.size()];
    NParamsList paramsList;
	for (int t = 0; t < i.size(); t++) {
        auto sumbound = i[t];	// {START, END, [STEP]}
        if (sumbound->GetType() != NMathObject::LIST) return New<NMathError>("NMath::Sum: More than 1 argument is not supported yet.");
        auto v = Dynamic_cast<NList, NMathObject>(sumbound);
        double step = 1.0f;
        if (v->components.size() == 3) {
            if (v->components[2]->GetType() != NMathObject::ATOM) return New<NMathError>("NMath::Sum: Parameters in {START, END, [STEP]} must be atom value.");
            step = v->components[2]->GetValue();
        } else if (v->components.size() != 2) return New<NMathError>("NMath::Sum: Please follow {START, END, [STEP]}");
        if (v->components[0]->GetType() != NMathObject::ATOM || v->components[1]->GetType() != NMathObject::ATOM) return New<NMathError>("NMath::Sum: Parameters in {START, END, [STEP]} must be atom value.");
        paramsList.push_back(New<NAtom>(st[t] = v->components[0]->GetValue()));
        ed[t] = v->components[1]->GetValue();
        sp[t] = step;
    }
	auto lst = i.size() - 1;
	auto prelst = i.size() - 2;
	auto ret = Static_cast<NMathObject, NAtom>(New<NAtom>(0));
	auto step = sp[lst];
	auto prestep = sp[prelst];
	auto lstpAtomPtr = Dynamic_cast<NAtom, NMathObject>(paramsList[lst]);
	auto prelstpAtomPtr = Dynamic_cast<NAtom, NMathObject>(paramsList[prelst]);
	while (paramsList[0]->GetValue() <= ed[0]) {
        for (double t = st[lst]; t <= ed[lst]; t += step) {
            lstpAtomPtr->SetValue(t);
            ret = Plus({ ret, f[0](paramsList) });
        }
        lstpAtomPtr->SetValue(st[lst]);
        prelstpAtomPtr->SetValue(prelstpAtomPtr->GetValue() + prestep);
        for (unsigned t = prelst; ~t; ) {
            if (paramsList[t]->GetValue() > ed[t]) {
                if (t == 0) {// 说明这是最后一轮回
                    if (paramsList[t]->GetValue() + sp[t] > ed[t]) return ret;
                    Dynamic_cast<NAtom, NMathObject>(paramsList[t])->SetValue(st[t]);
                    break;
                }
                Dynamic_cast<NAtom, NMathObject>(paramsList[t])->SetValue(st[t]);
                Dynamic_cast<NAtom, NMathObject>(paramsList[--t])->SetValue(paramsList[t]->GetValue() + sp[t]);
            } else break;
        }
	}
    return ret;
}

Ptr<NMathObject> mathS::NMath::NDerivative(NFuncParamsList f, NParamsList i) {
    if (f.size() != 1) return New<NMathError>("NMath::NDerivative: Expected 1 function");
    if (i.size() != 1) return New<NMathError>("NMath::NDerivative: Expected 1 variable");
    if (i[0]->GetType() != NMathObject::LIST) return New<NMathError>("NMath::NDerivative: More than 1 argument is not supported yet.");
    auto param = Dynamic_cast<NList, NMathObject>(i[0]);// {x, [∆x]}
    double x, delta_x = 0.0001f;
    if (param->components.size() == 1) x = param->components[0]->GetValue();
    else if (param->components.size() == 2) {
        x = param->components[0]->GetValue();
        delta_x = param->components[1]->GetValue();
    } else return New<NMathError>("NMath::NDerivative: Please follow {x, [∆x]}");
    NParamsList delta_f_param, f_param;
    delta_f_param.push_back(New<NAtom>(x + delta_x));
    f_param.push_back(New<NAtom>(x));
    return Divide({ Subtract({ f[0](delta_f_param), f[0](f_param) }), New<NAtom>(delta_x) });
}
