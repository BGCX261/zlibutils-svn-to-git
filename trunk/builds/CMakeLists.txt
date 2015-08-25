#  cmake -DCMAKE_BUILD_TYPE=Debug ../../../.
#  cmake -DCMAKE_BUILD_TYPE=Release ../../../.
#  cmake -DCMAKE_BUILD_TYPE=Debug ../../../. -G Xcode
#  cmake -DCMAKE_BUILD_TYPE=Release ../../../. -G Xcode

SET(CMAKE_LEGACY_CYGWIN_WIN32 0)

project(LIBZUTILS_0_1)
cmake_minimum_required( VERSION 2.6 FATAL_ERROR )

SET(TARGET "zutils")
 
MESSAGE( "SCAN DE SISTEMA" )
MESSAGE( "==========================================================================" )
MESSAGE( STATUS "SISTEMA ACTUAL              : "${CMAKE_SYSTEM_NAME} )
MESSAGE( STATUS "MODO                        : "${CMAKE_BUILD_TYPE} )
MESSAGE( STATUS "CMAKE_COMPILER_IS_GNUCXX    : "${CMAKE_COMPILER_IS_GNUCXX} )
MESSAGE( STATUS "UNIX                        : "${UNIX} )
MESSAGE( STATUS "WIN32                       : "${WIN32} )
MESSAGE( STATUS "APPLE                       : "${APPLE} )
MESSAGE( STATUS "MINGW                       : "${MINGW} )
MESSAGE( STATUS "MSYS                        : "${MSYS} )
MESSAGE( STATUS "CYGWIN                      : "${CYGWIN} )
MESSAGE( STATUS "BORLAND                     : "${BORLAND} )
MESSAGE( STATUS "WATCOM                      : "${WATCOM} )
MESSAGE( "==========================================================================" )

add_library( ${TARGET} SHARED
../../../../src/zutils.hpp
../../../../src/zutils_cnv.cpp
../../../../src/zutils_io.cpp 
../../../../src/zutils_rgx.cpp
../../../../src/zutils_str.cpp
../../../../src/zutils_str_T.hpp
../../../../src/zutils_ustr.hpp
../../../../src/zutils_conn.cpp
../../../../src/zutils_xml.cpp
../../../../src/pugiconfig.hpp
../../../../src/pugixml.cpp
../../../../src/pugixml.hpp)
								
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

	  MESSAGE( "SISTEMA: Apple OS X" )
	  MESSAGE( "==========================================================================" )	
	  IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
		  MESSAGE( "Compilando en MODO RELEASE" )
		  add_definitions("-std=c++98 -O3 -Wall -Wextra -pedantic -W") 
	  ELSE()
		  MESSAGE( "Compilando en MODO DEBUG" )
		  add_definitions("-std=c++98 -g -O0 -Wall -Wextra -pedantic -W") 
	  ENDIF()        
	  MESSAGE( "==========================================================================" )	

	         
      FIND_PATH(LIBZIP_INCLUDE zip.h "/usr/local/include")	  
	  FIND_LIBRARY(LIBZIP_LIBRARY NAMES zip libzip PATHS /usr/local/lib) 
       
      FIND_PATH(CURL_INCLUDE curl.h "/opt/local/include/curl")	  
	  FIND_LIBRARY(CURL_LIBRARY NAMES curl libcurl PATHS /opt/local/lib) 
         
	  FIND_PATH(ICONV_INCLUDE iconv.h "/opt/local/include")	  
	  FIND_LIBRARY(ICONV_LIBRARY NAMES iconv.2 libiconv.2 PATHS /opt/local/lib)
   
      FIND_PATH(PCRE_INCLUDE pcre.h "/opt/local/include")	  
	  FIND_LIBRARY(PCRE_LIBRARY NAMES pcre libpcre PATHS /opt/local/lib)
      
      FIND_PATH(PCRECPP_INCLUDE pcrecpp.h "/opt/local/include")	  
	  FIND_LIBRARY( PCRECPP_LIBRARY NAMES pcrecpp libpcrecpp PATHS /opt/local/lib)

	  include_directories ( ${ICONV_INCLUDE} ${PCRE_INCLUDE}  ${PCRECPP_INCLUDE} ${CURL_INCLUDE} ${LIBZIP_INCLUDE} )
	  target_link_libraries ( ${TARGET} ${PCRE_LIBRARY} ${PCRECPP_LIBRARY} ${CURL_LIBRARY} /opt/local/lib/libiconv.2.dylib ${LIBZIP_LIBRARY} ) 

ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

	  MESSAGE( "SISTEMA: Linux/GNU" )
	  MESSAGE( "==========================================================================" )	
	  IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
		  MESSAGE( "Compilando en MODO RELEASE" )
		  add_definitions("-std=c++98 -O3 -Wall -Wextra -pedantic -W") 
	  ELSE()
		  MESSAGE( "Compilando en MODO DEBUG" )
		  add_definitions("-std=c++98 -g -O0 -Wall -Wextra -pedantic -W") 
	  ENDIF()        
	  MESSAGE( "==========================================================================" )	
       
      FIND_PATH(LIBZIP_INCLUDE zip.h "/usr/include")	  
	  FIND_LIBRARY(LIBZIP_LIBRARY NAMES zip libzip PATHS /usr/lib) 
       
      FIND_PATH(CURL_INCLUDE curl.h "/usr/include/curl")	  
	  FIND_LIBRARY(CURL_LIBRARY NAMES curl libcurl PATHS /usr/lib) 
         
	  FIND_PATH(ICONV_INCLUDE iconv.h "/usr/include")	  
	  FIND_LIBRARY(ICONV_LIBRARY NAMES iconv_hook libiconv_hook PATHS /usr/lib)
   
      FIND_PATH(PCRE_INCLUDE pcre.h "/usr/include")	  
	  FIND_LIBRARY(PCRE_LIBRARY NAMES pcre libpcre PATHS /usr/lib)
      
      FIND_PATH(PCRECPP_INCLUDE pcrecpp.h "/usr/include")	  
	  FIND_LIBRARY( PCRECPP_LIBRARY NAMES pcrecpp libpcrecpp PATHS /usr/lib)

	  include_directories ( ${ICONV_INCLUDE} ${PCRE_INCLUDE}  ${PCRECPP_INCLUDE} ${CURL_INCLUDE} ${LIBZIP_INCLUDE} )
	  target_link_libraries ( ${TARGET} ${PCRE_LIBRARY} ${PCRECPP_LIBRARY} ${CURL_LIBRARY} ${ICONV_LIBRARY} ${LIBZIP_LIBRARY} ) 
	  
ENDIF()

ADD_CUSTOM_COMMAND( TARGET ${TARGET} POST_BUILD COMMAND cp  ../../../../src/zutils.hpp ../../../../include/zutils.hpp ) 
ADD_CUSTOM_COMMAND( TARGET ${TARGET} POST_BUILD COMMAND cp  ../../../../src/zutils_ustr.hpp ../../../../include/zutils_ustr.hpp )
