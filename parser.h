
#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

namespace parser
{
    template<char C=0,char ...rest>
    struct Char
    {
        static bool Parse(const char *&text)
        {
            //return (C==*text&&text++)||Char<rest...>::Parse(text);
            if(C==*text)
            {
                text++;
                return true;
            }
            else
            {
                return Char<rest...>::Parse(text);
            }
        }
    };

    template<char...rest>
    struct Char<0,rest...>
    {
        static bool Parse(const char *&text)
        {
            UNUSED(text);
            return false;
        }
    };

    template<char C=0,char...rest>
    struct Text
    {
        static bool Parse(const char *&text)
        {
            //return (C==*text++ && Text<rest...>::Parse(text) ) || (text--,false);
            if(C==*text++ &&
                Text<rest...>::Parse(text))
            {
                return true;
            }
            else
            {
                text--;
                return false;
            }
        }
    };
    template<char...rest>
    struct Text<0,rest...>
    {
        static bool Parse(const char *&text)
        {
            return true;
        }
    };

    template <char min,char max>
    struct Range
    {
        static bool Parse(const char *&text)
        {
            if(min<=*text &&
                max>=*text)
            {
                text++;
                return true;
            }
            else
            {
                return false;
            }
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
            //return (T::Parse(text) && Any<T>::Parse(text) ) || true;
            while(T::Parse(text))
                continue;
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
                while(T::Parse(text))
                    continue;
                return true;
            }
            else
            {
                return false;
            }
        }
    };


    // どれかにヒット 
    template<typename T=void,typename...rest>
    struct Or
        :virtual public T
        ,virtual public Or<rest...>
    {
        bool Parse(const char *&text)
        {
            return T::Parse(text) || Or<rest...>::Parse(text);
        }
    };
    template<typename...rest>
    struct Or<void,rest...>
    {
        static bool Parse(const char *text)
        {
            UNUSED(text);
            return false;
        }
    };

    template<typename T=void,typename...rest>
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
    struct Rule<void,rest...>
    {
        static bool Parse(const char *text)
        {
            UNUSED(text);
            return true;
        }
    };

}


#endif
