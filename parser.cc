
#include "parser.h"

#include <stdio.h>

template<typename T>
void print_result(const char *source)
{
	const char *from=source;
	T t;
	printf("\"%s\" as %d - read %d chars. rest - \"%s\"\n",from,t.Parse(source),source-from,source);
}


int main()
{
	{
		typedef parser::Char<'a','i','u','e','o'> charany;

		const char *source="ouieaieAaiueo";
		while(charany::Parse(source))
			printf("1");
		printf("\n");
	}
	{
		typedef parser::Text<'a','i','u','e','o'> seq;		
		print_result<seq>("aiueooo");
		print_result<seq>("aiuooo");
	}
	{
		typedef parser::Text<' ','-',' ','-'> seq;
		print_result<seq>(" - -");
		print_result<seq>(" - -!");
		print_result<seq>(" !");
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

		typedef Rule<Option<tInt>,Char<'.'>,More<tNum>,Option<tLogInt> > tFloat;

		print_result<tFloat>("10.0");
		print_result<tFloat>("0.123");
		print_result<tFloat>("3210.");
		print_result<tFloat>("-1.0E+50");
		print_result<tFloat>("-.0");
		print_result<tFloat>("-0.10E-3");
		print_result<tFloat>("+1.0");
		
		typedef More<Char<' ','\t','\r','\n'> > tS;
	}

	{
		/*
           copied from RFC4180

		   file = [header CRLF] record *(CRLF record) [CRLF]
		   header = name *(COMMA name)
		   record = field *(COMMA field)
		   name = field
		   field = (escaped / non-escaped)
		   escaped = DQUOTE *(TEXTDATA / COMMA / CR / LF / 2DQUOTE) DQUOTE
		   non-escaped = *TEXTDATA
		   COMMA = %x2C
		   CR = %x0D ;as per section 6.1 of RFC 2234 [2]
		   DQUOTE =  %x22 ;as per section 6.1 of RFC 2234 [2]
		   LF = %x0A ;as per section 6.1 of RFC 2234 [2]
		   CRLF = CR LF ;as per section 6.1 of RFC 2234 [2]
		   TEXTDATA =  %x20-21 / %x23-2B / %x2D-7E
		*/

		using namespace parser;

		typedef Or<Range<0x20,0x21>,Range<0x23,0x2B>,Range<0x2D,0x7E> > TEXTDATA;
		typedef Char<0x0A> LF;
		typedef Char<0x22> DQUATE;
		typedef Char<0x0D> CR;
		typedef Rule<CR,LF> CRLF;
		typedef Char<0x2C> COMMA;
		typedef Any<TEXTDATA> non_escaped;
		typedef Rule<DQUATE,Any<Or<TEXTDATA,COMMA,CR,LF,Rule<DQUATE,DQUATE> > >,DQUATE> escaped;
		typedef Or<escaped,non_escaped> field;
		typedef field name;
		typedef Rule<field,Any<Rule<COMMA,field> > > record;
		typedef Rule<name,Any<Rule<COMMA,name> > > header;
		typedef Rule<Option<Rule<header,CRLF> >,record,Any<Rule<CRLF,record> >,Option<CRLF> > file;

		print_result<file>("100,200,300\r\nabc,def,ghij");

        // 空行や空レコードは OK 
		print_result<file>("100,200,300\r\nabc,def,ghij,,\r\n\r\n");

        // 二重引用符はカンマと一体でないとだめ 
		print_result<file>("100,200,300\r\nabc,d\"ef,g\"hij");
		print_result<file>("100,200,300\r\nabc,\"def,g\",hij");

        // 改行は CRLF でないとだめ
		print_result<file>("100,200,300\nabc,def,ghij");
	}


	return 0;
}
