#include <Evaluate.h>
#include <MathParser.h>
#include <RuleLib.h>

using namespace mathS;

bool mathS::Evaluate(Ptr<MathObject> input, Ptr<MathObject>& result) {
	switch (input->GetType())
	{
	case MathObject::ATOM:
		return false;
	case MathObject::VECTOR:
		return Evaluate(Dynamic_cast<Vector>(input), result);
	case MathObject::POWER:
		return Evaluate(Dynamic_cast<Power>(input), result);
	case MathObject::INVERSE:
		return Evaluate(Dynamic_cast<Inverse>(input), result);
	case MathObject::ITEM:
		return Evaluate(Dynamic_cast<Item>(input), result);
	case MathObject::OPPOSITE:
		return Evaluate(Dynamic_cast<Opposite>(input), result);
	case MathObject::POLYNOMIAL:
		return Evaluate(Dynamic_cast<Polynomial>(input), result);

	default:
		return false;
		break;
	}
}

bool mathS::Evaluate(Ptr<Item> input, Ptr<MathObject>& result)
{
	Ptr<Item> s = New<Item>();
	// 先计算子表达式
	for (auto it : input->factors) {
		bool flag = false;
		auto s1 = it;
		while (Evaluate(s1, s1)) flag = true;
		s->push_back(flag ? s1 : it->DeepCopy());
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
	// 去掉多余的1
	while (RuleLib::Drop_ones(result, result));
	// 常数项计算 TODO
	// 常数分子分母约分 TODO
	// 再应用 Item 级的 Rule，一旦规则成功应用就返回
	if (RuleLib::ExpandDistributive(result, result))
		return true;
	if (RuleLib::VectorMultiply(result, result))
		return true;
	return false;
}

bool mathS::Evaluate(Ptr<Vector> input, Ptr<MathObject>& result) {
	// 先拷贝一份，以便对Item的factors递归地做Evaluate
	Ptr<Vector> s1 = Dynamic_cast<Vector>(input->DeepCopy());
	// 计算子表达式
	for (int i = 0; i < s1->components.size(); i++) {
		while (Evaluate(s1->components[i], s1->components[i]));
	}
	result = s1;
	return false; 
}

bool mathS::Evaluate(Ptr<Power> input, Ptr<MathObject>& result) {
	auto base = input->base->DeepCopy();
	while (Evaluate(base, base));
	auto exponet = input->exponent->DeepCopy();
	while (Evaluate(exponet, exponet));

	result = New<Power>(base, exponet);
	// 常数计算 TODO
	// 多项式幂展开 TODO 
	// 展开项因子
	if (RuleLib::ExpandItemPower(result, result)) return true;
	// 指数简化
	if (RuleLib::Power_simplify(result, result)) return true;
	// 向量幂次展开
	if (RuleLib::VectorPower(result, result)) return true;
	return false;
}

bool mathS::Evaluate(Ptr<Inverse> input, Ptr<MathObject>& result) {
	auto c = input->component->DeepCopy();
	// 计算子表达式
	while (Evaluate(c, c));
	result = New<Inverse>(c);
	if (RuleLib::Double_inverse_1(result, result)) return true;
	if (RuleLib::Double_inverse_2(result, result)) return true;
	return false;
}

bool mathS::Evaluate(Ptr<Opposite> input, Ptr<MathObject>& result) {
	auto c = input->component->DeepCopy();
	// 计算子表达式
	while (Evaluate(c, c));
	result = New<Opposite>(c);
	// 消除常量负号
	if (RuleLib::ConstantNegative(result, result)) return true;
	// 消除成对
	if (RuleLib::Double_negative(result, result)) return true;
	return false;
}

bool mathS::Evaluate(Ptr<Polynomial> input, Ptr<MathObject>& result) {
	Ptr<Polynomial> s = New<Polynomial>();
	// 先计算子表达式
	for (auto it : input->items) {
		bool flag = false;
		auto s1 = it;
		while (Evaluate(s1, s1)) flag = true;
		s->push_back(flag ? s1 : it->DeepCopy());
	}
	// 基本的消项、合并同类项处理
	result = ReducePolynomial(s);
    while (RuleLib::ConstantPlus(result, result));
	while (RuleLib::Reduce_opposite_terms(result, result));
	while (RuleLib::Combining_similar_terms(result, result));
    while (RuleLib::Drop_zeros(result, result));
	return false;
}