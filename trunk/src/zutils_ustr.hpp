#ifndef ZLIBUTIS_USTR_HPP
#define ZLIBUTIS_USTR_HPP

#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>



namespace utl
{
	namespace ustr
	{

		/*
		* Url de Interes
		* UTF-8 Tabla : http://www.utf8-chartable.de/unicode-utf8-table.pl?utf8=0x
		* UTF-8 Wiki  : http://en.wikipedia.org/wiki/UTF-8
		*
		*/

		class uString
		{
		private:
			enum  {EXTRA_BYTES = 2, OFFSET = 2};
			char* ptr;
			size_t capInBytes, lenInUChars, lenInBytes;
			static size_t const npos = (size_t)-1;

		private:
			char* top() const
			{			
				char* ptr_ = (this->ptr + this->lenInBytes) - OFFSET;
				while(sizeInOctets(*(ptr_)) == 0)			
					ptr_--;			
				return ptr_;
			}

		public:
			uString(void) : ptr(NULL), lenInUChars(0), capInBytes(0), lenInBytes(0) {}

			template<typename T, size_t N> uString(T (&tx)[N])
			{
				lenInUChars = sizeInUChars(tx);				
				lenInBytes = N;
				npos = capInBytes = lenInBytes + EXTRA_BYTES;

				ptr = new char[capInBytes];
				memset(ptr, 0, capInBytes);
				strncpy(ptr, tx, lenInBytes);
			}

			uString(const char* tx, const size_t sizeInBytes)
			{
				lenInBytes = sizeInBytes;
				capInBytes = lenInBytes + EXTRA_BYTES;
				ptr = new char[capInBytes];
				memset(ptr, 0, capInBytes);
				strncpy(ptr, tx, lenInBytes);			
				lenInUChars = sizeInUChars(ptr);			
			}

			~uString(void)
			{
				lenInUChars = lenInBytes = capInBytes = 0;
				if (ptr != NULL)
				{
					delete[] ptr;
					ptr = NULL;
				}
			}

			uString(const uString& w)
			{
				if(this != &w)
				{
					this->lenInUChars = w.lenInUChars;
					this->lenInBytes = w.lenInBytes;
					this->capInBytes = w.capInBytes;
					this->ptr = new char[this->capInBytes];
					memset(this->ptr, 0, this->capInBytes);
					strncpy(this->ptr, w.ptr, this->lenInBytes);
				}
			}

			void assign(const char* tx)
			{
				if (this->lenInUChars != 0 && this->capInBytes != 0 && this->lenInBytes != 0 && this->ptr != NULL)
				{
					this->lenInUChars = this->lenInBytes = this->capInBytes = 0;	
					delete[] this->ptr;
					this->ptr = NULL;		
				}
				lenInUChars = sizeInUChars(tx);
				lenInBytes = strlen(tx) + 1;
				capInBytes = lenInBytes + EXTRA_BYTES;

				ptr = new char[capInBytes];
				memset(ptr, 0, capInBytes);
				strncpy(ptr, tx, lenInBytes);
			}

			uString& operator=(const uString& w)
			{
				if(this != &w)
				{
					if (this->lenInUChars != 0 && this->capInBytes != 0 && this->lenInBytes != 0 && this->ptr != NULL)
					{
						this->lenInUChars = this->lenInBytes = this->capInBytes = 0;	
						delete[] this->ptr;
						this->ptr = NULL;		
					}
					this->lenInUChars = w.lenInUChars;
					this->lenInBytes = w.lenInBytes;
					this->capInBytes = w.capInBytes;
					this->ptr = new char[this->capInBytes];
					memset(this->ptr, 0, this->capInBytes);				
					strncpy(this->ptr, w.ptr, this->lenInBytes);;
				}
				return *this;
			}

			uString& operator=(const char* tx)
			{
				if (this->lenInUChars != 0 && this->capInBytes != 0 && this->lenInBytes != 0 && this->ptr != NULL)
				{
					this->lenInUChars = this->lenInBytes = this->capInBytes = 0;	
					delete[] this->ptr;
					this->ptr = NULL;		
				}

				lenInUChars = sizeInUChars(tx);
				lenInBytes = strlen(tx) + 1;
				capInBytes = lenInBytes + EXTRA_BYTES;

				ptr = new char[capInBytes];
				memset(ptr, 0, capInBytes);
				strncpy(ptr, tx, lenInBytes);	

				return *this;
			}

			size_t sizeInChars(void) const
			{
				return lenInUChars;
			}

			size_t sizeInBytes(void) const
			{
				return lenInBytes;
			}

			size_t capacityInBytes(void) const
			{
				return capInBytes;
			}

			size_t error(void) const
			{
				return npos;
			}

			std::string str(void) const
			{
				return std::string(ptr);
			}

			const char* c_str(void) const 
			{
				return ptr;
			}

			uString substr(const size_t PosInUChars = 0, const size_t LenInUChars = 0)
			{
				if (PosInUChars == 0 && LenInUChars == 0)
					return uString();
				else
				{					
					size_t index = 0, lenInBytes = 0;
					char* ptr = NULL;
					for(uString::iterator itBase = this->begin(); itBase != this->end(); itBase++, index++)
					{
						if (index == PosInUChars)
							ptr = itBase.c_str();
						if (index >= PosInUChars && index < PosInUChars + LenInUChars)
						{
							lenInBytes += uString::sizeInOctets(*itBase.c_str());
						}
					}
					if (lenInBytes > 0)
						return uString(ptr, lenInBytes);
					else
						return uString();
				}
			}

			/*
			 * Return Param Values [ A.compare(B) ]:
			 *  0 : A == B
			 *  1 : A > B
			 * -1 : A < B
			 */
			int compare(const uString& toCompare) const
			{
				const char* ptrA = this->c_str();
				const char* ptrB = toCompare.c_str();
				size_t lenInOcts_A = uString::sizeInOctets(*ptrA);
				size_t lenInOcts_B = uString::sizeInOctets(*ptrB);

				if ( lenInOcts_A > lenInOcts_B )
					return 1;
				if ( lenInOcts_A < lenInOcts_B )
					return -1;

				if (lenInOcts_A == 1 && *ptrA == *ptrB )
					return 0;
				if (lenInOcts_A == 1 && *ptrA > *ptrB )
					return 1;
				if (lenInOcts_A == 1 && *ptrA < *ptrB )
					return -1;

				while (0 < lenInOcts_A--)
				{
					if (*(ptrA+lenInOcts_A) > *(ptrB+lenInOcts_A))
						return 1;
					if (*(ptrA+lenInOcts_A) < *(ptrB+lenInOcts_A))
						return -1;
				}

				return 0;
			}

			size_t find_first_of(uString toCompare)
			{
				uString::iterator itCmp = toCompare.begin();
				size_t pos = uString::npos, index = 0;
				for(uString::iterator itBase = this->begin(); itBase != this->end(); itBase++, index++)
				{
					if (itCmp == toCompare.end())
						break;
					else if ((*itBase).compare(*itCmp) && pos == uString::npos)
					{
						pos = index;
						itCmp++;
					}
					else if ((*itBase).compare(*itCmp) && pos != uString::npos)
					{
						itCmp++;
					}					
					else
					{
						pos = uString::npos;
						itCmp = toCompare.begin();
					}
				}

				if (pos != uString::npos && itCmp == toCompare.end())
					return pos;
				else
					return uString::npos;

			}

			static size_t sizeInOctets(const char c)
			{
				if (( c & 0x80 ) == 0 )          // lead bit is zero, must be a single ascii: 1 octet
				{
					return 1;
				}
				else if (( c & 0xE0 ) == 0xC0 )  // 110x xxxx : 2 octets
				{
					return 2;
				}
				else if (( c & 0xF0 ) == 0xE0 )  // 1110 xxxx : 3 octets
				{
					return 3;
				}
				else if (( c & 0xF8 ) == 0xF0 )  // 1111 0xxx : 4 octets
				{
					return 4;
				}
				else                             // Unrecognized lead byte (%02x)
				{
					return 0;
				}
			}

			static size_t sizeInUChars(const char* utx)
			{
				unsigned char* ptr = (unsigned char*)utx;
				size_t len = 0;
				while (*ptr != 0x00)
				{
					switch (sizeInOctets(*ptr))
					{
					case 1:
						ptr++;
						len++;
						break;
					case 2:
						ptr += 2;
						len++;
						break;
					case 3:
						ptr += 3;
						len++;
						break;
					case 4:
						ptr += 4;
						len++;
						break;
					default:
						ptr++;
						break;
					}
				}

				return len;
			}			


		public:
			class iterator;
			friend class iterator;
			class iterator
			{

			private:

				uString& s;
				char* currPtr;

			public:

				iterator(uString& is) : s(is), currPtr(is.ptr) {}
				iterator(uString& is, bool) : s(is), currPtr(is.ptr+(is.lenInBytes-1)) {}

				const uString operator*()	
				{ 
					return uString(currPtr, uString::sizeInOctets(*currPtr)); 

				}

				char* c_str(void)
				{
					return currPtr;
				}

				const uString operator++(int) // POSTFIX
				{					
					char* TmpcurrPtr = currPtr;
					currPtr += uString::sizeInOctets(*currPtr);
					return uString(currPtr, uString::sizeInOctets(*TmpcurrPtr));
				}

				const uString operator++() // PREFIX
				{
					currPtr += uString::sizeInOctets(*currPtr);
					return uString(currPtr, uString::sizeInOctets(*currPtr));
				}

				bool operator!=(const iterator& rv) const
				{
					return currPtr != rv.currPtr;
				}

				bool operator==(const iterator& rv) const
				{
					return currPtr == rv.currPtr;
				}

				iterator& operator=(const iterator& rv)
				{
					s = rv.s;
					currPtr = rv.currPtr;
					return *this;                
				}

				friend std::ostream& operator<<(std::ostream& os, const iterator& it) 
				{
					char ptr_[4];
					memset(ptr_, 0, 4);
					strncpy(&ptr_[0], it.currPtr, uString::sizeInOctets(*it.currPtr));

					return os << ptr_;
				}            

			private:
				iterator& operator+=(int) 	{ return *this; }
				iterator& operator-=(int) 	{ return *this; }
				iterator& operator--(int) 	{ return *this; }            
			};

			iterator begin() 
			{ 
				return iterator(*this); 
			}

			iterator end() 
			{ 
				return iterator(*this, true); 
			}

		};

		
	}
}


#endif // ZLIBUTIS_USTR_HPP


