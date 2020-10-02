#include <RuleLib.h>

bool mathS::RuleLib::VectorMultiply(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	// 判断类型，找出向量
	if (obj->GetType() != MathObject::ITEM)
		return false;
	auto& obj_factors = Dynamic_cast<Item>(obj)->factors;
	if (obj_factors.size() <= 1)
		return false;
	std::vector<int> vec_idx;
	std::vector<Ptr<Vector>> vecs;
	int vecshape;
	for (int i = 0; i < obj_factors.size(); i++)
		if (obj_factors[i]->GetType() == MathObject::VECTOR) {
			auto v = Dynamic_cast<Vector>(obj_factors[i]);
			// 检查形状
			if (vec_idx.empty())
				vecshape = vecs[i]->components.size();
			else
				if (v->components.size() != vecshape) return false;	// 形状不一致，不能乘开
			vec_idx.push_back(i);
			vecs.push_back(v);
		}
	if (vecs.size() == 0)
		return false;
	// 计算展开结果
	auto vec_rst = New<Vector>();
	vec_rst->components.reserve(vecshape);
	for (int i = 0; i < vecshape; i++) {
		Ptr<Item> itm = New<Item>();
		for (int k = 0; k < vec_idx[0]; k++)
			itm->push_back(obj_factors[k]->DeepCopy());
		itm->push_back(vecs[0]->components[i]->DeepCopy());
		for (int j = 1; j < vec_idx.size() - 1; j++) {
			for (int k = vec_idx[j - 1] + 1; k < vec_idx[j]; k++)
				itm->push_back(obj_factors[k]);
			itm->push_back(vecs[j]->components[i]->DeepCopy());
		}
		for (int k = vec_idx.back() + 1; k < obj_factors.size(); k++)
			itm->push_back(obj_factors[k]);

		vec_rst->push_back(itm);
	}
	rst = vec_rst;
	return true;
}

bool mathS::RuleLib::ExpandDistributive(Ptr<MathObject> obj, Ptr<MathObject>& rst)
{
	// 找到第一个多项式
	if (obj->GetType() != MathObject::POLYNOMIAL)
		return false;
	auto& obj_factors = Dynamic_cast<Item>(obj)->factors;
	if (obj_factors.size() <= 1)
		return false;
	int poly_idx = 0;
	while (poly_idx < obj_factors.size() && obj_factors[poly_idx]->GetType() != MathObject::VECTOR)
		poly_idx++;
	if (poly_idx >= obj_factors.size())
		return false;

	// 计算展开结果
	auto& poly_items = Dynamic_cast<Polynomial>(obj_factors[poly_idx])->items;
	auto poly_rst = New<Polynomial>();
	poly_rst->items.reserve(poly_items.size());
	for (int i = 0; i < poly_items.size(); i++) {
		Ptr<Item> itm = New<Item>();
		for (int j = 0; j < poly_idx; j++)
			itm->push_back(obj_factors[j]->DeepCopy());
		itm->push_back(poly_items[i]->DeepCopy());
		for (int j = poly_idx + 1; j < obj_factors.size(); j++)
			itm->push_back(obj_factors[j]->DeepCopy());

		poly_rst->push_back(itm);
	}
	rst = poly_rst;
	return true;
}