#include <Compute.h>

using namespace mathS;

Ptr<MathObject> mathS::Compute(const Ptr<Item>& input)
{
	// 先拷贝一份
	Ptr<Item> rst = Dynamic_cast<Item>(input->DeepCopy());
	return Ptr<MathObject>();
}
