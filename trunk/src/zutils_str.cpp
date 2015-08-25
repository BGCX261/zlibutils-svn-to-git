#include "zutils.hpp"
#include "zutils_str_T.hpp"

#include <cstdarg>
#include <cstring>



namespace utl
{
	namespace str
	{
		bool SplitByString(const char* line, const char* sep, std::vector<std::string>& lines)
		{
			return split_T<const char*>(line, sep, lines);
		}

		bool SplitByChar(const char* line, const char sep, std::vector<std::string>& lines)
		{
			return split_T<const char>(line, sep, lines);
		}

		void loadVectorOfStrings_(std::vector<std::string>& v, const char* tag, ...)
		{
			va_list args;
			va_start(args, tag);
			v.push_back(tag);
			for(;;)
			{
				const char* s = va_arg(args, const char*);
				if (strcmp(s, "::END") != 0)
					v.push_back(s);	
				else
					break;
			}
			va_end(args);
		}

	}
}

