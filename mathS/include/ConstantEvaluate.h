#pragma once

#include <MathObject.h>

namespace mathS {
	
	namespace ConstantEvaluate {
		// 常量加法。调用前要检查是否为数字 Atom
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