#pragma once
#include <memory>

namespace mathS {

	template<class T>
	using Ptr = std::shared_ptr<T>;

#define New std::make_shared
	
	template<class T1, class T2>
    Ptr<T1> Dynamic_cast(Ptr<T2> _Arg) {
        return std::dynamic_pointer_cast<T1, T2>(_Arg);
    } 
}
