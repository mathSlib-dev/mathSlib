#pragma once
#include <MathObject.h>
#include <map>

namespace mathS {

	namespace Rule {
		
		/// <summary>
		/// ∆•≈‰ pattern ∫Õ obj£¨
		/// </summary>
		/// <param name="pattern"></param>
		/// <param name="obj"></param>
		/// <param name="table"></param>
		bool Match(const Ptr<MathObject> pattern, const Ptr<MathObject> obj, std::map<std::string, Ptr<MathObject>>& table);
	};
}