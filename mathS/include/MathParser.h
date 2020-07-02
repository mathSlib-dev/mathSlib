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


		// 解析整个字符串 str
		MathObject* Parse(const std::string& str);

	private:
		// 这是判断字符类型的工具函数
		static bool inline is_figure(char c);
		static bool inline is_op(char c);
		static bool is_varchar(char c);

		// 对于文本表达式 str， 从位置 i 开始， 解析并返回出一个级别不超过lv的一个对象.
		// 注意，i是引用。结束时， i 指向识别的对象之后的一个位置。这可以方便调用它的函数接着继续进行操作。
		MathObject* Parse(const std::string& str, int& i, int lv);

		// 识别特定的基本对象的函数. （通常是识别不超过特定级别的）
		MathObject* parse_number(const std::string& str, int& i);
		MathObject* parse_variable(const std::string& str, int& i);
		MathObject* parse_string(const std::string& str, int& i);
		MathObject* parse_vector(const std::string& str, int& i);
		MathObject* parse_function(const std::string& str, int& i);
		MathObject* parse_list(const std::string& str, int& i);

		MathObject* parse_list_forced(const std::string& str, int& i);


		/* 
		注意parser们何时会返回 nullptr，何时返回 MathErrorObject:

		* 发现非法语法时，parse返回的是MathErrorObject，包含了语法错误的信息；如果发现了MathErrorObject，收到该返回值的母函数，会停止解析，并把错误信息传递上去
		* 而例如parse_number识别到的不是数字，而是变量名，不一定不符合语法，会返回nullptr. 收到该返回值的母函数，会继续尝试其他的parser方法。
		
		*/
	};
}