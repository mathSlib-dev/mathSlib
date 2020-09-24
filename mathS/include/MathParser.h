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

    public:
	    // 从字符串初始化
	    Parser(const std::string& c) : lexer(c) { }

		// 获得解析的 MathObject 符号表达式对象
	    Ptr<MathObject> Parse();
    private:
		Lexer lexer;

	    short level(const std::string& c);
		/// <summary>
		/// 从 start 位置开始解析一个对象，直到到达右括号或末尾. 若没有对象则返回 EmptyObject. 若检测到语法错误则返回ErrorObject
		/// </summary>
		/// <param name="tokens"></param>
		/// <param name="start"></param>
		/// <param name="i">引用变量，解析结束时，i在被解析的对象对应token的后一位索引(即往后继续解析开始的位置)</param>
		/// <returns></returns>
		Ptr<MathObject> parseObject(const std::vector<Token>& tokens, const int start, int& i);

		Ptr<MathObject> parseAtom(const std::vector<Token>& tokens, const int start, int& i);
		Ptr<MathObject> parseFunction(const std::vector<Token>& tokens, const int start, int& i);
		Ptr<MathObject> parseLocate(const std::vector<Token>& tokens, const int start, int& i);
		Ptr<MathObject> parsePower(const std::vector<Token>& tokens, const int start, int& i);
		Ptr<MathObject> parseItem(const std::vector<Token>& tokens, const int start, int& i);
		Ptr<MathObject> parsePolynomial(const std::vector<Token>& tokens, const int start, int& i);
		Ptr<MathObject> parseMap(const std::vector<Token>& tokens, const int start, int& i);
		Ptr<MathObject> parseCompare(const std::vector<Token>& tokens, const int start, int& i);
		std::vector<Ptr<MathObject>> parseList(const std::vector<Token>& tokens, const int start, int& i);

	};
}