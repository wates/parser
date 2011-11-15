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

}



#include <stdio.h>

int main()
{
	typedef parser::Char<'a'> a;
	typedef parser::Text<'a','i','u','e','o'> aiueo;

	const char *source="aiueooo";
	bool is=aiueo::Parse(source);

	printf("%d\n",is);

	return 0;
}
