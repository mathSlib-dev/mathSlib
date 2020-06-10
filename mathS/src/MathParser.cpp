#include <MathParser.h>

using namespace mathS;


bool mathS::Parser::is_figure(char c)
{
	return '0' <= c && c <= '9';
}

MathObject* mathS::Parser::Parse(const std::string& str, int& i, int lv)
{
	int len = str.size();
	switch (lv)
	{

	default:
		break;
	}
	return nullptr;
}

MathObject* mathS::Parser::parse_number(const std::string& str, int& i)
{
	int len = str.size();
	int ibegin = i;
	bool pointflag = false;
	if(!is_figure(str[i]) && str[i]!='i')
		return nullptr;
	for (; i < len; i++)
	{
		if (is_figure(str[i])) 
			continue;
		if (is_op(str[i]))
			return new Number(str.substr(ibegin, i-ibegin));
		if (str[i] == '.' && pointflag)
			break;
		else
			pointflag = true;
		if (str[i] == 'i')
		{
			i++;
			if (i==len || is_op(str[i]))
				return new Number(str.substr(ibegin, i-ibegin));
			else break;
		}
	}
	if(i==len)
		return new Number(str.substr(ibegin, i-ibegin));
	return new ErrorObject("Parse: Bad symbol after " + str.substr(ibegin, i - ibegin));
}

MathObject* mathS::Parser::parse_variable(const std::string& str, int& i)
{
	int len = str.size();
	int ibegin = i;
	if (!is_varchar(str[i]) || is_figure(str[i]))
		return nullptr;
	for (i; i < len; i++)
	{
		if (is_varchar(str[i]))
			continue;
		if (is_op(str[i]))
			return new Variable(str.substr(ibegin, i - ibegin));
		return new ErrorObject("Parse: Bad symbol after " + str.substr(ibegin, i - ibegin));
	}
	if (i == len)
		new ErrorObject("Parse: Bad symbol after " + str.substr(ibegin, i - ibegin));
}

MathObject* mathS::Parser::parse_vector(const std::string& str, int& i)
{
	if (str[i] != '{')
		return nullptr;
	int len = str.size();
	int ibegin = i;
	i++;
	MathObject* vlist = parse_list_forced(str, i);
	if (vlist == nullptr)
		return new ErrorObject("Parse: Unexpected error in parsing. ");
	if (vlist->GetType() == MathObject::Type::ERROR)
		return vlist;
	if (i>=len || str[i] != '}')
	{
		delete vlist;
		return new ErrorObject("Parse: Unmatched brackets.");
	}
	i++;
	Vector* ret = new Vector;
	ret->list = dynamic_cast<ListObject*>(vlist);
	return ret;
}
