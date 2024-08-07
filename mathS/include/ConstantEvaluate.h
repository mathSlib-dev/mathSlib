#pragma once

#include "MathObject.h"

namespace mathS {
	
	namespace ConstantEvaluate {
		// �����ӷ�������ǰҪ����Ƿ�Ϊ���� Atom
		Ptr<MathObject> Plus(Ptr<Atom> a, Ptr<Atom> b) {
			return New<Atom>(std::to_string(a->NumberValue() + b->NumberValue()));
		}
		Ptr<MathObject> Negative(Ptr<Atom> a) {
			return New<Atom>(std::to_string(-a->NumberValue()));
		}
		Ptr<MathObject> Multyply(Ptr<Atom> a, Ptr<Atom> b) {
			return New<Atom>(std::to_string(a->NumberValue() * b->NumberValue()));
		}
	}
}