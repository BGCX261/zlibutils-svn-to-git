#pragma comment(lib, "zip.lib")

#include "zutils.hpp"

#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

#include "zip.h"

namespace utl
{
	namespace io
	{
		bool FileReadBinInCharsetU(const char* const Path, const char* const charset_from, const char* const charset_to, ustr::uString& fileContent)
		{
			std::ifstream BinFile(Path, std::ios::in | std::ios::binary | std::ios::ate);
			std::ifstream::pos_type fileSize = 0;
			char* fileContents = NULL;

			if (BinFile.is_open())
			{
				fileSize = BinFile.tellg();
				size_t len = static_cast<long>(fileSize)+10;
				fileContents = new char[len];
				memset(fileContents, 0, len);
				BinFile.seekg(0, std::ios::beg);
				if (!BinFile.read(fileContents, static_cast<std::streamsize>(fileSize)))
				{
					delete[] fileContents;
					fileContents = NULL;
					BinFile.close();
					return false;
				}
				BinFile.close();
				char* fileContentsConv = NULL;
				if (cnv::PerformCharsetConversion(charset_from, charset_to, fileContents, fileContentsConv) == false)
				{
					if (fileContents) delete[] fileContents;
					fileContents = NULL;
					if (fileContentsConv) delete[] fileContentsConv;
					fileContentsConv = NULL;
					return false;
				}
				else
				{
					if (fileContents) delete[] fileContents;
					fileContents = NULL;
					fileContent.assign(fileContentsConv);
					if (fileContentsConv) delete[] fileContentsConv;
					fileContentsConv = NULL;
				}

			} else return false;

			return true;
		}

		bool FileReadBinInCharset(const char* const Path, const char* const charset_from, const char* const charset_to, std::string& fileContent)
		{
			std::ifstream BinFile(Path, std::ios::in | std::ios::binary | std::ios::ate);
			std::ifstream::pos_type fileSize = 0;
			char* fileContents = NULL;

			if (BinFile.is_open())
			{
				fileSize = BinFile.tellg();
				size_t len = static_cast<long>(fileSize)+10;
				fileContents = new char[len];
				memset(fileContents, 0, len);
				BinFile.seekg(0, std::ios::beg);
				if (!BinFile.read(fileContents, static_cast<std::streamsize>(fileSize)))
				{
					delete[] fileContents;
					fileContents = NULL;
					BinFile.close();
					return false;
				}
				BinFile.close();
				char* fileContentsConv = NULL;
				if (cnv::PerformCharsetConversion(charset_from, charset_to, fileContents, fileContentsConv) == false)
				{
					if (fileContents) delete[] fileContents;
					fileContents = NULL;
					if (fileContentsConv) delete[] fileContentsConv;
					fileContentsConv = NULL;
					return false;
				}
				else
				{
					if (fileContents) delete[] fileContents;
					fileContents = NULL;
					fileContent.assign(fileContentsConv);
					if (fileContentsConv) delete[] fileContentsConv;
					fileContentsConv = NULL;
				}

			} else return false;

			return true;
		}

		bool FileWriteBinInCharset(const char* const Path, const char* const charset_from, const char* const charset_to, const char* const fileContent)
		{
			std::ofstream BinFile(Path, std::ios::out | std::ios::binary);
			std::ofstream::pos_type fileSize = 0;

			if (BinFile.is_open())
			{
				char* fileContentsConv = NULL;

				if (cnv::PerformCharsetConversion(charset_from, charset_to, fileContent, fileContentsConv) == false)
				{
					if (fileContentsConv) delete[] fileContentsConv;
					fileContentsConv = NULL;
					BinFile.close();
					return false;
				}
				else
				{
					BinFile.write(fileContentsConv, strlen(fileContentsConv));
					if (fileContentsConv) delete[] fileContentsConv;
					fileContentsConv = NULL;					
				}
				BinFile.close();

			} else return false;

			return true;
		}

		bool FileReadBinU(const char* const Path, ustr::uString& fileContent)
		{
			std::ifstream BinFile(Path, std::ios::in | std::ios::binary | std::ios::ate);
			std::ifstream::pos_type fileSize = 0;
			char* fileContents = NULL;

			if (BinFile.is_open())
			{
				fileSize = BinFile.tellg();
				size_t len = static_cast<long>(fileSize)+10;
				fileContents = new char[len];
				memset(fileContents, 0, len);
				BinFile.seekg(0, std::ios::beg);
				if (!BinFile.read(fileContents, static_cast<std::streamsize>(fileSize)))
				{
					delete[] fileContents;
					fileContents = NULL;
					BinFile.close();
					return false;
				}
				BinFile.close();
				fileContent.assign(fileContents);
				if (fileContents) delete[] fileContents;
				fileContents = NULL;				
			} else return false;

			return true;
		}

		bool FileReadBin(const char* const Path, std::string& fileContent)
		{
			std::ifstream BinFile(Path, std::ios::in | std::ios::binary | std::ios::ate);
			std::ifstream::pos_type fileSize = 0;
			char* fileContents = NULL;

			if (BinFile.is_open())
			{
				fileSize = BinFile.tellg();
				size_t len = static_cast<long>(fileSize)+10;
				fileContents = new char[len];
				memset(fileContents, 0, len);
				BinFile.seekg(0, std::ios::beg);
				if (!BinFile.read(fileContents, static_cast<std::streamsize>(fileSize)))
				{
					delete[] fileContents;
					fileContents = NULL;
					BinFile.close();
					return false;
				}
				BinFile.close();
				fileContent.assign(fileContents);
				if (fileContents) delete[] fileContents;
				fileContents = NULL;				
			} else return false;

			return true;
		}


		bool FileWriteBin(const char* const Path, const char* const fileContent)
		{
			std::ofstream BinFile(Path, std::ios::out | std::ios::binary);
			std::ofstream::pos_type fileSize = 0;

			if (BinFile.is_open())
			{
				BinFile.write(fileContent, strlen(fileContent));
				BinFile.close();
			} else return false;

			return true;
		}

		bool ZipReadinCharset(const char* const path, const char* const fileName, const char* const charset_from, 
			const char* const charset_to, std::string& Content)
		{
			int err = 0;
			// Open the ZIP archive
			zip *z = zip_open(path, 0, &err);
			if (!err)
			{
				// Search for the file of given name
				struct zip_stat st;
				zip_stat_init(&st);
				zip_stat(z, fileName, 0, &st);

				// Alloc memory for its uncompressed contents
				char* contents = new char[st.size+10];
				if (contents == NULL)
					return false;
				memset(contents, 0, st.size+10);


				// Read the compressed file
				zip_file *f = NULL;
				f = zip_fopen(z, fileName, 0);
				if (f == NULL)
				{
					if (contents) delete[] contents;
					contents = NULL;
					return false;
				}
				zip_fread(f, contents, st.size);
				zip_fclose(f);

				// And close the archive
				zip_close(z);

				char* fileContentsConv = NULL;
				if (cnv::PerformCharsetConversion(charset_from, charset_to, contents, fileContentsConv) == false)
				{
					if (contents) delete[] contents;
					contents = NULL;
					if (fileContentsConv) delete[] fileContentsConv;
					fileContentsConv = NULL;
					return false;
				}
				else
				{
					if (contents) delete[] contents;
					contents = NULL;
					Content.assign(fileContentsConv);
					if (fileContentsConv) delete[] fileContentsConv;
					fileContentsConv = NULL;
				}

			} else return false;

			return true;
		}

		bool ZipRead(const char* const path, const char* const fileName, std::string& Content)
		{
			int err = 0;
			// Open the ZIP archive
			zip *z = zip_open(path, 0, &err);
			if (!err)
			{
				// Search for the file of given name
				struct zip_stat st;
				zip_stat_init(&st);
				zip_stat(z, fileName, 0, &st);

				// Alloc memory for its uncompressed contents
				char* contents = new char[st.size+10];
				if (contents == NULL)
					return false;
				memset(contents, 0, st.size+10);


				// Read the compressed file
				zip_file *f = NULL;
				f = zip_fopen(z, fileName, 0);
				if (f == NULL)
				{
					if (contents) delete[] contents;
					contents = NULL;
					return false;
				}
				zip_fread(f, contents, st.size);
				zip_fclose(f);

				// And close the archive
				zip_close(z);
				Content.assign(contents);
				if (contents) delete[] contents;
				contents = NULL;

			} else return false;

			return true;
		}        


	}
}


