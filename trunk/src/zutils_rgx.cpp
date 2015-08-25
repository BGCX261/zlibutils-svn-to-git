#ifdef _MSC_VER 
#ifdef _DEBUG
#pragma comment(lib, "pcred.lib")
#pragma comment(lib, "pcrecppd.lib")
#else
#pragma comment(lib, "pcre.lib")
#pragma comment(lib, "pcrecpp.lib")
#endif
#define PCRE_STATIC 1
#endif

#include "zutils.hpp"
#include "pcrecpp.h"

namespace utl
{
	namespace rgx
	{

		std::string FindAll(const char* const pattern, const char* const text, std::vector<std::string>& Results)
		{
			pcrecpp::RE re(pattern);
			std::string Error;
			if (!(Error = re.error()).empty())
				return Error;

			pcrecpp::StringPiece input(text);
			std::string s;
			while (re.FindAndConsume(&input, &s))
				Results.push_back(s);

			return Error;
		}

		std::string RemplaceAll(const char* const pattern, const char* const text, const char* const remplaceWith, std::string& Result)
		{
			pcrecpp::RE re(pattern);
			std::string Error;
			if (!(Error = re.error()).empty())
				return Error;

			Result = text;
			re.GlobalReplace(remplaceWith, &Result);

			return Error;
		}

	}
}




