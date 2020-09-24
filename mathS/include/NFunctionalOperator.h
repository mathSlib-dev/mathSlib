#pragma once

#include <NMathObject.h>
#include <NFunction.h>

namespace mathS {

	namespace NMath {

		using NFuncParamsList = std::vector<NFunction>;
		using NFuncOperator = std::function<Ptr<NMathObject>(NFuncParamsList, NParamsList)>;
		
		Ptr<NMathObject> Sum(NFuncParamsList f, NParamsList i);
	}
}