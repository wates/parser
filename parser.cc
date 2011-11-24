
#include "parser.h"

#include <stdio.h>

template<typename T>
void print_result(const char *source)
{
	const char *from=source;
	T t;
	printf("\"%s\" as %d - read %d chars. left - \"%s\"\n",from,t.Parse(source),source-from,source);
}


int main()
{
	{
		typedef parser::Char<'a','i','u','e','o'> charany;

		const char *source="ouieaieA";
		while(charany::Parse(source))
			printf("1");
		printf("\n");
	}
	{
		typedef parser::Text<'a','i','u','e','o'> seq;		
		print_result<seq>("aiueooo");
	}
	{
		typedef parser::Text<' ','-',' ','-'> seq;
		print_result<seq>(" - -");
	}
	{
		typedef parser::Text<' '> seq;
		print_result<seq>(" - -");
	}
	{
		typedef parser::Range<'0','9'> num;
		print_result<num>("0");
		print_result<num>("5");
		print_result<num>("9");
		print_result<num>("a");
		print_result<num>("A");
	}
	{
		typedef parser::Range<'a','z'> az;
		print_result<az>("9");
		print_result<az>("a");
		print_result<az>("z");
		print_result<az>("A");
	}
	{
		typedef parser::Text<'f','o','o'> seq;
		typedef parser::Option<seq> opt;
		print_result<opt>("foofoofoo");
	}
	{
		typedef parser::Text<'f','o','o'> seq;
		typedef parser::Option<seq> opt;
		print_result<opt>("vvfoofoofoo");
	}
	{
		typedef parser::Text<'f','o','o'> seq;
		typedef parser::Any<seq> any;
		print_result<any>("foofoofoo");
	}
	{
		typedef parser::Text<'f','o','o'> seq;
		typedef parser::More<seq> more;
		print_result<more>("foofoofoo");
	}
	{
		typedef parser::Text<'f','o','o'> seq;
		typedef parser::More<seq> more;
		print_result<more>("vvfoofoofoo");
	}
	{
		using namespace parser;
		typedef Or<Range<'0','9'>
						,Range<'a','z'>
						,Range<'A','Z'> > numbet;

		print_result<numbet>("a");
		print_result<numbet>("z");
		print_result<numbet>("Z");
		print_result<numbet>("0");
		print_result<numbet>(" ");
		print_result<numbet>(",");

		typedef Any<numbet> numbets;
		print_result<numbets>("abcABC912--");
		print_result<numbets>("==abcABC912==");
	}
	{
		using namespace parser;

		typedef Range<'0','9'> tNum;
		typedef Range<'1','9'> tNonZeroNum;
		typedef Or<Rule<tNonZeroNum,Any<tNum> >,Char<'0'> > tUnsignedInt;
		typedef Rule<Option<Char<'+'> >,tUnsignedInt> tPositiveNum;
		typedef Rule<Char<'-'>,tUnsignedInt > tNegativeNum;
		typedef Or<tPositiveNum,tNegativeNum> tInt;

		typedef Rule<Char<'E','e'>,Option<Char<'+','-'> >,More<tNum> > tLog;
		typedef Rule<Option<tInt>,Option<tLog> > tLogInt;

		typedef Rule<Option<tInt>,Char<'.'>,More<tNum>,Option<tLogInt> > tFloat;

		print_result<tUnsignedInt>("100");
		print_result<tUnsignedInt>("0123");
		print_result<tUnsignedInt>("3210");
		print_result<tUnsignedInt>("-1");
		print_result<tUnsignedInt>("+10");

		print_result<tInt>("100");
		print_result<tInt>("0123");
		print_result<tInt>("3210");
		print_result<tInt>("-1");
		print_result<tInt>("-0");
		print_result<tInt>("-010");
		print_result<tInt>("+10");

		print_result<tFloat>("10.0");
		print_result<tFloat>("0.123");
		print_result<tFloat>("3210.");
		print_result<tFloat>("-1.0E+50");
		print_result<tFloat>("-.0");
		print_result<tFloat>("-0.10E-3");
		print_result<tFloat>("+1.0");
		
	}


	return 0;
}
