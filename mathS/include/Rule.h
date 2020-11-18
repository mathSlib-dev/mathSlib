#pragma once
#include <MathObject.h>
#include <map>
#include <list>
#include <MathParser.h>

namespace mathS {
	// Rule的第一个参数是输入obj，第二个引用是返回值。Rule引用不会改变输入obj指向的内容，引用返回的涉及到obj中对应内容都是拷贝。
	// 如果返回false，说明匹配失败，引用参数不会改变
	using Rule = std::function<bool(Ptr<MathObject>, Ptr<MathObject>&)>;
	using Match = std::function<bool(Ptr<MathObject>)>;

	/// <summary>
	/// 对于给定的Pattern，生成一个匹配
	/// </summary>
	/// <param name="pattern"></param>
	/// <returns></returns>
	Match MakeMatch(Ptr<MathObject> pattern);
	/// <summary>
	/// 对于给定的source pattern, target pattern，生成一个匹配-替换规则
	/// </summary>
	/// <param name="src_pattern"></param>
	/// <param name="tar_pattern"></param>
	/// <returns></returns>
	Rule MakeRule(const Ptr<MathObject>& src_pattern, Ptr<MathObject> tar_pattern);
	/// <summary>
	/// 对于给定的source pattern, target function，生成一个匹配-替换规则
	/// </summary>
	/// <param name="src_pattern"></param>
	/// <param name="do_replace"></param>
	/// <returns></returns>
	Rule MakeRule(const Ptr<MathObject>& src_pattern, const std::function<Ptr<MathObject>(std::map<std::string, Ptr<MathObject>>&, bool&)>&);// 这里回调的bool参数是用来应答是否拒绝应用此规则的flag，若为true则为拒绝，将不再调用

	/// <summary>
	/// 匹配。对给定的pattern和obj的形式，并返回匹配表
	/// </summary>
	/// <param name="pattern"></param>
	/// <param name="obj"></param>
	/// <param name="table"></param>
	/// <returns></returns>
	bool DoMatch(Ptr<MathObject> pattern, Ptr<MathObject> obj, std::map<std::string, Ptr<MathObject>>& table, std::list<std::string>& table_list);
	
	/// <summary>
	/// 替换。对于给定的table，返回将pattern中对应地替换的结果（是拷贝，不影响pattern）
	/// </summary>
	/// <param name="pattern"></param>
	/// <param name="table"></param>
	/// <returns></returns>
	Ptr<MathObject> DoReplace(Ptr<MathObject> pattern, std::map<std::string, Ptr<MathObject>>& table);

	/// <summary>
	/// 全匹配，当两个两个表达式完全相同返回true。(为什么不比较GetString()？因为GetString()一定会遍历整个表达式，效率很低。)
	/// 将来为了提高Compare效率，可以对MathObject维护高度属性
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	bool FullCompare(Ptr<MathObject> a, Ptr<MathObject> b);



}