#pragma once


#include <MathObject.h>

namespace mathS {

	// MathFunction : MathObject -> MathObject
	// MathFunction 是对数学对象，运用一些规则来得到结果的
	using MathFunction = std::function<Ptr<MathObject>(std::vector<Ptr<MathObject>>)>;
	
}