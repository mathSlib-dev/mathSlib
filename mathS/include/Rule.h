#pragma once
#include <MathObject.h>
#include <map>
#include <list>

namespace mathS {

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
	Rule MakeRule(Ptr<MathObject> src_pattern, Ptr<MathObject> tar_pattern);

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
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	bool FullCompare(Ptr<MathObject> a, Ptr<MathObject> b);


	// 规则库，在这里可以写包括直接生成的规则，或是必须要程序进行的规则
	namespace RuleLib {
		
		
		
	};
}