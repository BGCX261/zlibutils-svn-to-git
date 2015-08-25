/*
 * NOTAS: POSIBLES PROBLEMAS EN EL LINKAGE EN WINDOWS
 * Cuando da un error de linkage que no puede resolver el simbolo, ej : "?GetByHttp@conn@@YA_NQBDPBUProxyData_t@1@AAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z"
 * Usaremos : dumpbin.exe -headers libzutils.lib | findstr /c:"AQUI EL TEXTO A BUSCAR"
 * AMBOS COLORIDOS NOMBRE DEBERAN SER IGUALES
 */

#ifndef ZLIBUTIS_HPP
#define ZLIBUTIS_HPP

#include <string>
#include <vector>
#include <map>

#include "zutils_ustr.hpp"

namespace utl
{
	/* 
	* OPERACIONES DE CONVERSION DE CARACTERES
	* Basada en : libiconv
	* Windows Version libiconv :1.14
	* Web:
	* http://www.gnu.org/software/libiconv/
	*/
	namespace cnv
	{
		/*
		* Encoding Validos :
		* http://www.iana.org/assignments/character-sets/character-sets.xml
		*/
		bool PerformCharsetConversion (const char* const charset_from, const char* const charset_to, const char* str_from, char*& str_to);
	}

	/*
	* OPERACIONES CON FICHEROS
	*/
	namespace io
	{
		/*
		* Encoding Validos :
		* http://www.iana.org/assignments/character-sets/character-sets.xml
		* readFile y writeFile no funcionan con UTF-16 UTF-32
		* Su mejor comportamiento es de UTF8 a ISO-XXX y de ISO-XXX a UTF8 o windows-XXX a UTF8 y UTF8 a windows-xxx		 
		*/
		bool FileReadBinInCharset(const char* const Path, const char* const charset_from, const char* const charset_to, std::string& fileContent);
		bool FileReadBinInCharsetU(const char* const Path, const char* const charset_from, const char* const charset_to, ustr::uString& fileContent);
		bool FileWriteBinInCharset(const char* const Path, const char* const charset_from, const char* const charset_to, const char* const fileContent);
		bool FileReadBin(const char* const Path, std::string& fileContent);
		bool FileReadBinU(const char* const Path, ustr::uString& fileContent);
		bool FileWriteBin(const char* const Path, const char* const fileContent);
		bool ZipReadinCharset(const char* const path, const char* const fileName, const char* const charset_from, 
						  const char* const charset_to, std::string& Content);
        bool ZipRead(const char* const path, const char* const fileName, std::string& Content);                  
	}

	/*
	* OPERACIONES CON CADENAS
	*/
	namespace str
	{
		void loadVectorOfStrings_(std::vector<std::string>& v, const char* tag, ...);
		#define loadVectorOfStrings(vector, tag, ...) loadVectorOfStrings_(vector, tag, __VA_ARGS__, "::END")
		bool SplitByString(const char* line, const char* sep, std::vector<std::string>& lines);
		bool SplitByChar(const char* line, const char sep, std::vector<std::string>& lines);	
		bool SplitByStringU(const char* line, const char* sep, std::vector<ustr::uString>& lines);
		bool SplitByCharU(const char* line, const char sep, std::vector<ustr::uString>& lines);
	}		

	/*
	* OPERACIONES CON EXPRESIONES REGULARES : Basadas en libpcre 
	* Windows Version libpcre : 8.32
	* Doc wrapper pcrecpp
	* http://regexkit.sourceforge.net/Documentation/pcre/pcrecpp.html
	*/
	namespace rgx
	{
		std::string FindAll(const char* const pattern, const char* const text, std::vector<std::string>& Results);
		std::string RemplaceAll(const char* const pattern, const char* const text, const char* const remplaceWith, std::string& Result);
	}

	/*
	* OPERACIONES CON SERVIDORES HTTP: Basada en libcurl(no ssl) y zlib 
	* Windows Version libcurl : 7.18.0 
	* Windows Version zlib : 1.2.3 
	* Cliente para hacer peticiones Http. Con o sin proxy.
	* http://curl.haxx.se/libcurl/
	*/	
	namespace conn
	{				
		typedef struct { const char* url; const char* user; const char* password; unsigned port; } ProxyData_t;
		bool GetByHttp(const char* const url, const ProxyData_t* proxyCredentials, std::string& Content);
		bool GetByHttpInCharset(const char* const url, const ProxyData_t* proxyCredentials, const char* const charset_from, 
			const char* const charset_to, std::string& Content);
		bool GetHttpFile(const char* const url, const char* const targetPath, const ProxyData_t* proxyCredentials);

	}

	/*
	* OPERACIONES CON XMLs : Basada en plugiXML
	* http://pugixml.org/
	*/
	namespace xml
	{
		typedef std::vector<std::map<std::string, std::string> > vDataPairs_t;	
		typedef std::map<std::string, std::string>  DataPairs_t;	
		typedef std::vector<std::string> Childs_t;
		/*
		 * Esta funcion solo es eficaz solo con XMLs de dos niveles, ej:
		 * <?xml version='1.0' encoding='UTF-8' ?>
		 * <Root>
		 *    <Dato1> valor </Dato1>
		 *    <Dato2> valor </Dato2>
		 * </Root>
		 */
		bool GetDataFrom2Levels(const char* const xml, const char* const rootName, const Childs_t childsNames, vDataPairs_t& vDataPairs );
		
		/*
		 * Esta funcion a diferencia de la anterior funciona a cualquier nivel es decir si tenemos:
		 * <?xml version='1.0' encoding='UTF-8' ?>
		 * <Root>
		 *	  <nivel1>
		 *        <nivel2>
		 *           <Dato1> valor </Dato1>
		 *           <Dato2> valor </Dato2>
		 *        </nivel2>
		 *        <nivel2>
		 *           <Dato1> valor </Dato1>
		 *           <Dato2> valor </Dato2>
		 *        </nivel2>
	     *        <nivel2>
		 *           <Dato1> valor </Dato1>
		 *           <Dato2> valor </Dato2>
		 *        </nivel2>
		 *    </nivel1>
		 * </Root>
		 * Y lo que deseamos es iterar sobre <nivel2> para obtner los valores de <Dato1> y <Dato2>
		 * Deberamos cargar
		 * childsToTarget = {"Root", "nivel1", "nivel2"}
		 * childsNamesToGet = {"Dato1", "Dato2"}
		 */
		bool GetDataFromLevelN(const char* const xml, const Childs_t childsNamesToGet, Childs_t childsToTarget, vDataPairs_t& vDataPairs );

		std::string GetChildValueByPos(const vDataPairs_t& vDataPairs, const char* const ChildName, const size_t pos);
		std::string GetChildValue(const DataPairs_t& DataPairs, const char* const ChildName);
	}

}

#endif // ZLIBUTIS_HPP


