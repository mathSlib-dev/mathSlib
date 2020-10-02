#include <Compute.h>
#include <MathParser.h>
#include <RuleLib.h>

using namespace mathS;

bool mathS::Compute(Ptr<Item> input, Ptr<MathObject>& result)
{
	// 先拷贝一份，以便对Item的factors递归地做Compute
	Ptr<Item> s = Dynamic_cast<Item>(input->DeepCopy());
	// 先计算子表达式
	for (auto it : input->factors) {
		auto s1 = it->DeepCopy();
		while (Compute(s1, s1));
		s->push_back(s1);
	}
	result = Dynamic_cast<MathObject>(s);
	// 化简优先
	// 同底数幂合并（包括约分）
	while (RuleLib::Power_reduction_1(result, result));
	while (RuleLib::Power_reduction_2(result, result));
	while (RuleLib::Power_reduction_3(result, result));
	while (RuleLib::Power_reduction_4(result, result));
	while (RuleLib::Power_reduction_5(result, result));
	while (RuleLib::Power_reduction_6(result, result));
	// 常数项计算 TODO
	// 常数分子分母约分 TODO
	// 再应用 Item 级的 Rule，一旦规则成功应用就返回
	if (RuleLib::ExpandDistributive(result, result))
		return true;
	if (RuleLib::VectorMultiply(result, result))
		return true;
	return false;
}

bool mathS::Compute(Ptr<MathObject> input, Ptr<MathObject>& result) {
	switch (input->GetType())
	{
	case MathObject::ATOM:
		return false;
	case MathObject::ITEM:
		return Compute(Dynamic_cast<Item>(input), result);
	default:
		return false;
		break;
	}
}

bool mathS::Compute(Ptr<Vector> input, Ptr<MathObject>& result) {
	// 先拷贝一份，以便对Item的factors递归地做Compute
	Ptr<Vector> s1 = Dynamic_cast<Vector>(input->DeepCopy());
	// 计算子表达式
	for (int i = 0; i < s1->components.size(); i++) {
		while (Compute(s1->components[i], s1->components[i]));
	}
	return false; 
}