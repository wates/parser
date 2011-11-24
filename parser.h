
#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

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
		//:public CharHasAny<rest...>
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
		//:public CharHasAny<C,rest...>
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
		//:public CharSequence<rest...>
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
		//:public CharSequence<C,rest...>
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

	// ここまではプリミティブな式のみ 


	// あってもなくてもよい。 
	template <typename T>
	struct Option
		:virtual public T
	{
		bool Parse(const char *&text)
		{
			T::Parse(text);
			return true;
		}
	};

	// 0 個以上にヒット。あるだけ削る。 
	template<typename T>
	struct Any
		:public T
	{
		bool Parse(const char *&text)
		{
			while(T::Parse(text));
			return true;
		}
	};

	// 1 個以上にヒット。 
	template<typename T>
	struct More
		:public T
	{
		bool Parse(const char *&text)
		{
			if(T::Parse(text))
			{
				while(T::Parse(text));
				return true;
			}
			return false;
		}
	};


	// どれかにヒット 
	struct stop{};

	template<typename T=stop,typename...rest>
	struct Or
		:virtual public T
		,virtual public Or<rest...>
	{
		bool Parse(const char *&text)
		{
			const char *src=text;
			if(T::Parse(text))
				return true;
			text=src;
			if(Or<rest...>::Parse(text))
				return true;
			text=src;
			return false;
		}
	};
	template<typename...rest>
	struct Or<stop,rest...>
	{
		static bool Parse(const char *text)
		{
            UNUSED(text);
			return false;
		}
	};

	template<typename T=stop,typename...rest>
	struct Rule
		:virtual public T
		,virtual public Rule<rest...>
	{
		bool Parse(const char *&text)
		{
			const char *src=text;
			if(T::Parse(text)&&
				Rule<rest...>::Parse(text))
				return true;
			text=src;
			return false;
		}
	};

	template<typename...rest>
	struct Rule<stop,rest...>
	{
		static bool Parse(const char *text)
		{
            UNUSED(text);
			return true;
		}
	};

}


#endif
