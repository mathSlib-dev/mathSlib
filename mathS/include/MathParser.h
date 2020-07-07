#pragma once

#include <MathObject.h>
#include <string>
#include <lexer.h>

/*
	In this file, we introduce methods that parse text input of expressions 
	and transform them into MathObject.

	
*/
namespace mathS
{
	class Parser
	{
		/* 
		注意parser们何时会返回 nullptr，何时返回 MathErrorObject:

		* 发现非法语法时，parse返回的是MathErrorObject，包含了语法错误的信息；如果发现了MathErrorObject，收到该返回值的母函数，会停止解析，并把错误信息传递上去
		* 而例如parse_number识别到的不是数字，而是变量名，不一定不符合语法，会返回nullptr. 收到该返回值的母函数，会继续尝试其他的parser方法。
		
		*/
    public:
	    Parser(const std::string& c) : lexer(c) { }
	    MathObject* Parse();
    private:
	    short priority(const std::string& c);
	    Lexer lexer;
	};
}