#include "zutils.hpp"
#include "pugixml.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace utl
{
	namespace xml
	{
		std::string GetChildValue(const DataPairs_t& DataPairs, const char* const ChildName)
		{
			DataPairs_t::const_iterator itm = DataPairs.find(ChildName);
			if (itm != DataPairs.end())
				return itm->second;
			else
				return "";
		}

		std::string GetChildValueByPos(const vDataPairs_t& vDataPairs, const char* const ChildName, const size_t pos)
		{
			DataPairs_t::const_iterator itm = vDataPairs.at(pos).find(ChildName);
			if (itm != vDataPairs.at(pos).end())
				return itm->second;
			else
				return "";
		}


		bool GetDataFrom2Levels(const char* const xml, const char* const rootName, const Childs_t childsNames, vDataPairs_t& vDataPairs )
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load(xml);
			if (!result) return false;
			DataPairs_t nodes;

			for (pugi::xml_node itn = doc.child(rootName); itn; itn = itn.next_sibling(rootName))
			{
				for(Childs_t::const_iterator itv = childsNames.begin(); itv != childsNames.end(); itv++)
				{
					nodes[itv->c_str()] = itn.child_value(itv->c_str());
				}
				vDataPairs.push_back(nodes);
			}


			return true;
		}

		bool GetDataFromLevelN(const char* const xml, const Childs_t childsNamesToGet, Childs_t childsToTarget, vDataPairs_t& vDataPairs )
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load(xml);
			if (!result) return false;
			DataPairs_t nodes;
			pugi::xml_node nodeToIterate = doc.child(childsToTarget.at(0).c_str());
			std::string nodeTarget = childsToTarget.back();
			childsToTarget.pop_back();

			for(Childs_t::const_iterator it = childsToTarget.begin()+1; it != childsToTarget.end(); it++)
			{
				nodeToIterate = nodeToIterate.child(it->c_str());
			}

			for (pugi::xml_node itn = nodeToIterate.first_child(); itn; itn = itn.next_sibling())
			{
				if (nodeTarget.compare(itn.name()) != 0) continue;
				for(Childs_t::const_iterator itv = childsNamesToGet.begin(); itv != childsNamesToGet.end(); itv++)
				{
					nodes[itv->c_str()] = itn.child_value(itv->c_str());
				}
				vDataPairs.push_back(nodes);
			}

			return true;
		}


	}
}




