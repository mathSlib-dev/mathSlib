#pragma once

#include <MathObject.h>
#include <string>

/*
	In this file, we introduce methods that parse text input of expressions 
	and transform them into MathObject.
*/
namespace mathS
{
	class Parser
	{
	public:

		//
		MathObject* Parse(const std::string& str);

	private:
		static bool inline is_figure(char c);
		static bool inline is_op(char c);
		static bool is_varchar(char c);

		MathObject* Parse(const std::string& str, int& i, int lv);

		MathObject* parse_number(const std::string& str, int& i);
		MathObject* parse_variable(const std::string& str, int& i);
		MathObject* parse_string(const std::string& str, int& i);
		MathObject* parse_vector(const std::string& str, int& i);
		MathObject* parse_function(const std::string& str, int& i);
		MathObject* parse_list(const std::string& str, int& i);

		MathObject* parse_list_forced(const std::string& str, int& i);
	};
}