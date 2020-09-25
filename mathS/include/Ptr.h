#pragma once
#include <memory>

namespace mathS {
	// 使用智能指针，防止内存泄漏
	template<class T>
	using Ptr = std::shared_ptr<T>;  

#define New std::make_shared
	// 强制内存转换，基类与派生类之间相互转换
#define Dynamic_cast std::dynamic_pointer_cast
}
