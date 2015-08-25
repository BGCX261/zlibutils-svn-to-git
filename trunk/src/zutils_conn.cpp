#ifdef _MSC_VER 
#pragma comment(lib, "libcurl.lib")
#endif

#include "zutils.hpp"

#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <curl/curl.h>

#ifdef _MSC_VER
#define SLEEP(TIME) Sleep(TIME); 
#else
#include <unistd.h>
#define SLEEP(TIME) sleep(TIME); 
#endif

namespace utl
{
	namespace conn
	{
		typedef struct { char *memory; size_t size; } MemoryStruct_t;

		std::string static getProxyUrl(const ProxyData_t* proxyCredentials);
		std::string static getProxyUserPassword(const ProxyData_t* proxyCredentials);

		/*
		* Dentro de esta funcion se reserva memoria para Chunk->memory que luego hay que liberar. Si algo falla,
		* no sera necesario liberar la memoria.
		*/
		bool static getHttp(const char* const url, const ProxyData_t* proxyCredentials, const unsigned numero_intentos, 
			const unsigned wait_time, MemoryStruct_t* Chunk);

		void static initConn(const char* const url, const ProxyData_t* proxyCredentials, MemoryStruct_t* chunk);
		size_t static writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
		size_t static write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

		bool getHttp(const char* const url, const ProxyData_t* proxyCredentials, const unsigned numero_intentos, 
			const unsigned wait_time, MemoryStruct_t* Chunk)
		{
			Chunk->memory = (char*)malloc(1);
			Chunk->size = 0;	
			size_t intentos = 0;

			for (intentos = 0; intentos < numero_intentos; intentos++)
			{
				initConn(url, proxyCredentials, Chunk);
				if (Chunk->size > 0)
					break;        
				else		  	  
					SLEEP(wait_time);
			}

			if (Chunk->size <= 0 && intentos == numero_intentos)
			{
				if (Chunk->memory) free(Chunk->memory);
				Chunk->memory = NULL;
				Chunk->size = 0;
				return false;
			}

			return true;
		}

		bool GetByHttpInCharset(const char* const url, const ProxyData_t* proxyCredentials, const char* const charset_from, 
			const char* const charset_to, std::string& Content)
		{
			const unsigned INTENTOS = 5, WAIT_TIME = 30;
			MemoryStruct_t chunk;

			if (!getHttp(url, proxyCredentials, INTENTOS, WAIT_TIME, &chunk))
				return false;

			// CHARSET TRANSFORMACION
			char* fileContentsConv = NULL;
			if (cnv::PerformCharsetConversion(charset_from, charset_to, chunk.memory, fileContentsConv) == false)
			{
				if (chunk.memory)
				{
					free(chunk.memory);
					chunk.memory = NULL;
					chunk.size = 0;
				}
				if (fileContentsConv) delete[] fileContentsConv;
				fileContentsConv = NULL;
				return false;
			}
			else
			{
				if (chunk.memory)
				{
					free(chunk.memory);
					chunk.memory = NULL;
					chunk.size = 0;
				}
				Content.assign(fileContentsConv);
				if (fileContentsConv) delete[] fileContentsConv;
				fileContentsConv = NULL;
			}

			return true;
		}

		bool GetByHttp(const char* const url, const ProxyData_t* proxyCredentials, std::string& Content)
		{
			const unsigned INTENTOS = 5, WAIT_TIME = 30;
			MemoryStruct_t chunk;

			if (!getHttp(url, proxyCredentials, INTENTOS, WAIT_TIME, &chunk))
				return false;

			Content.append(chunk.memory);
			if (chunk.memory)
			{
				free(chunk.memory);
				chunk.memory = NULL;
				chunk.size = 0;
			}

			return true;
		}

		bool GetHttpFile(const char* const url, const char* const targetPath, const ProxyData_t* proxyCredentials)
		{
			CURL *curl_handle;
			curl_global_init(CURL_GLOBAL_ALL);
			FILE* fp;
			curl_handle = curl_easy_init();
			if (curl_handle)
			{
				if (proxyCredentials != NULL)
				{
					curl_easy_setopt(curl_handle, CURLOPT_PROXY, getProxyUrl(proxyCredentials).c_str());
					curl_easy_setopt(curl_handle, CURLOPT_PROXYUSERPWD, getProxyUserPassword(proxyCredentials).c_str());
				}

				fp = fopen(targetPath, "wb");
				curl_easy_setopt(curl_handle, CURLOPT_URL, url);
				curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
				curl_easy_perform(curl_handle);
				curl_easy_cleanup(curl_handle);
				fclose(fp);
			} else return false;

			return true;
		}

		std::string getProxyUrl(const ProxyData_t* proxyCredentials)
		{
			std::stringstream ss;
			ss << proxyCredentials->url << ":" << proxyCredentials->port;
			return ss.str();
		}

		std::string getProxyUserPassword(const ProxyData_t* proxyCredentials)
		{
			std::stringstream ss;
			ss << proxyCredentials->user << ":" << proxyCredentials->password;
			return ss.str();
		}

		void initConn(const char* const url, const ProxyData_t* proxyCredentials, MemoryStruct_t* chunk)
		{
			CURL *curl_handle;
			curl_global_init(CURL_GLOBAL_ALL);
			curl_handle = curl_easy_init();

			if (proxyCredentials != NULL)
			{
				curl_easy_setopt(curl_handle, CURLOPT_PROXY, getProxyUrl(proxyCredentials).c_str());
				curl_easy_setopt(curl_handle, CURLOPT_PROXYUSERPWD, getProxyUserPassword(proxyCredentials).c_str());
			}
			curl_easy_setopt(curl_handle, CURLOPT_URL, url);
			curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
			curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) chunk);
			curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
			curl_easy_perform(curl_handle);
			curl_easy_cleanup(curl_handle);
			curl_global_cleanup();        
		}

		size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
		{

			size_t realsize = size * nmemb;
			MemoryStruct_t* mem = (MemoryStruct_t *) userp;

			mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
			if (mem->memory == NULL) 
			{
				printf("No hay suficiente memoria (realloc ha devuelto NULL)\n");
				exit(1);
			}

			memcpy(&(mem->memory[mem->size]), contents, realsize);
			mem->size += realsize;
			mem->memory[mem->size] = 0;

			return realsize;
		}

		size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) 
		{
			size_t written;
			written = fwrite(ptr, size, nmemb, stream);
			return written;
		}

	}
}

