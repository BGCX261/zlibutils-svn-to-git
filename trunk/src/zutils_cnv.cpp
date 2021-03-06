#ifdef _MSC_VER 
#ifdef _DEBUG
#pragma comment(lib, "libiconv_a_debug.lib")
#else
#pragma comment(lib, "libiconv_a.lib")
#endif
#endif

#include "zutils.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <locale>
#include <cstring>
#include <cstdlib>
#include <cwchar>
#include <cerrno>

#include "iconv.h"

namespace utl
{
	namespace cnv
	{
		static bool InitCharsetsConversion (const char* const charset_from, const char* const charset_to, iconv_t& conv_desc)
		{
			conv_desc = iconv_open (charset_to, charset_from);
			return ( conv_desc == (iconv_t)-1) ? false : true;		
		}

		#ifdef _MSC_VER 
		#pragma warning(disable : 4996)
		#endif
		static bool CloseCharsetsConversion (iconv_t conv_desc)
		{
			int v = 0;
			v = iconv_close (conv_desc);
			if (v != 0) 
			{
				fprintf (stderr, "iconv_close failed: %s\n", strerror (errno));
				return false;
			}

			return true;
		}

		#ifdef _MSC_VER 
		#pragma warning(disable : 4996)
		#endif
		bool PerformCharsetConversion (const char* const charset_from, const char* const charset_to, const char* str_from, char*& str_to)
		{

			iconv_t conv_desc;	
			size_t iconv_value = 0;
			size_t len = 0;
			size_t to_len = 0;
			size_t len_start = 0;
			size_t to_len_start = 0;

			char* to_start;
			char* from_start;


			len = strlen (str_from);	
			if (!len) return false;
			if (!InitCharsetsConversion(charset_from, charset_to, conv_desc)) return false;
			to_len = len*4;
			str_to = NULL;
			//str_to = (char*)calloc(to_len, sizeof(char));
			str_to = new char[to_len];
			memset(str_to, 0, to_len);
			if (str_to == NULL)
			{
				fprintf (stderr, "Calloc failed to request memory.\n");
				return false;
			}
			len_start = len;
			to_len_start = to_len;
			to_start = str_to;
			from_start = (char*)str_from;

			iconv_value = iconv (conv_desc, (char**)&from_start, &len, &to_start, &to_len);			
			if (errno == EILSEQ)
			{
				printf ("Error[%d] : %s.\n", errno, strerror (errno));
				fprintf (stderr, "Invalid multibyte sequence.\n");
				free (str_to);
				str_to = NULL;
				CloseCharsetsConversion(conv_desc);
				return false;
			}
			else if (errno == EINVAL)
			{
				printf ("Error[%d] : %s.\n", errno, strerror (errno));
				fprintf (stderr, "Incomplete multibyte sequence.\n");
				free (str_to);
				str_to = NULL;
				CloseCharsetsConversion(conv_desc);
				return false;
			}
			else if (errno == E2BIG)
			{
				printf ("Error[%d] : %s.\n", errno, strerror (errno));
				fprintf (stderr, "No more room.\n");
				free (str_to);
				str_to = NULL;
				CloseCharsetsConversion(conv_desc);
				return false;
			}

			if (!CloseCharsetsConversion(conv_desc)) return false;
			return true;
		}

	}
}


