#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

namespace parser
{
	template <char min,char max>
	struct CharRange
	{
		static bool Parse(const char *&text)
		{
			if(min<=*text&&max>=*text)
			{
				text++;
				return true;
			}
			return false;
		}
	};

	template<char C=0,char... rest>
	struct CharHasAny
		:public CharHasAny<rest...>
	{
		static bool Equal(const char *&text)
		{
			return C==*text||CharHasAny<rest...>::Equal(text);
		}
	};

	template<>
	struct CharHasAny<0>
	{
		static bool Equal(const char *&text)
		{
            UNUSED(text);
			return false;
		}
	};

	template<char C,char ...rest>
	struct Char
		:public CharHasAny<C,rest...>
	{
		static bool Parse(const char *&text)
		{
			if(CharHasAny<C,rest...>::Equal(text))
			{
				text++;
				return true;
			}
			return false;
		}
	};

	template<char C=0,char...rest>
	struct CharSequence
		:public CharSequence<rest...>
	{
		static const int length=1+CharSequence<rest...>::length;
		static bool Equal(const char *text)
		{
			return C==*text&&CharSequence<rest...>::Equal(text+1);
		}
	};

	template<char...rest>
	struct CharSequence<0,rest...>
	{
		static const int length=0;
		static bool Equal(const char *text)
		{
            UNUSED(text);
			return true;
		}
	};

	template<char C=0,char...rest>
	struct Text
		:public CharSequence<C,rest...>
	{
		static bool Parse(const char *&text)
		{
			const char *save=text;
			if(CharSequence<C,rest...>::Equal(text))
			{
				text+=CharSequence<C,rest...>::length;
				return true;
			}
			text=save;
			return false;
		}
	};

	template <char min,char max>
	struct Range
	{
		static bool Parse(const char *&text)
		{
			if(min<=*text&&max>=*text)
			{
				text++;
				return true;
			}
			return false;
		}
	};

}

#include <stdio.h>

template<typename T>
void print_result(const char *source)
{
	const char *from=source;
	printf("\"%s\" as %d - read %d chars. left - \"%s\"\n",from,T::Parse(source),source-from,source);
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


	return 0;
}
