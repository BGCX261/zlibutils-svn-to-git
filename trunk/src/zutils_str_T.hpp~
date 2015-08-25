#ifndef ZLIBUTIS_STR_T_HPP
#define ZLIBUTIS_STR_T_HPP

// TEMPLATES OF ZUTILS_STR

#include <string>
#include <vector>

namespace utl
{
	// Operaciones con cadenas
	namespace str
	{
		template<typename T1> bool split_T(const char* line_in, T1 sep, std::vector<std::string>& lines)
		{
			std::string line(line_in), extr;
			size_t ini = 0, end = 0;
			end = line.find_first_of(sep);
			while(end != std::string::npos)
			{
				extr = line.substr(ini, end-ini);
				if (extr.size() > 0)
					lines.push_back(extr);
				
				ini = end+1;
				end = line.find_first_of(sep, end+1);
				if (end == std::string::npos && ini < line.size())
				{
					std::string extr = line.substr(ini);
					if (extr.size() > 0)
						lines.push_back(extr);
				}
			}

			if (lines.size() == 0) return false;

			return true;
		}
	}
}

#endif // ZLIBUTIS_STR_T_HPP

