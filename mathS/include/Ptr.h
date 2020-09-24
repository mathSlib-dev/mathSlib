#pragma once
#include <memory>

namespace mathS {
	// 使用智能指针，防止内存泄漏
	template<class T>
	using Ptr = std::shared_ptr<T>;  

#define New std::make_shared
	// 强制内存转换，基类与派生类之间相互转换
	template<class T1, class T2>
    Ptr<T1> Dynamic_cast(Ptr<T2> _Arg) {
        return std::dynamic_pointer_cast<T1, T2>(_Arg);
    } 
}
