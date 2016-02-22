# - Try to find curl
# Once done this will define
#
#  CURL_FOUND - system has CURL
#  CURL_INCLUDE_DIRS - the CURL include directory
#  CURL_LIBRARIES - Link these to use CURL
#  CURL_DEFINITIONS - Compiler switches required for using CURL

include (DynamicLet)
include (FindPackageHandleStandardArgs)

letmany (CMAKE_FIND_LIBRARY_SUFFIXES ".dylib") 
find_path (CURL_INCLUDE_DIR 
  NAMES curl.h
  PATH_SUFFIXES include/curl
  PATHS ${CMAKE_SOURCE_DIR}/external/curl/result
  )
find_library (CURL_LIBRARY
  NAMES curl
  PATH_SUFFIXES lib
  PATHS ${CMAKE_SOURCE_DIR}/external/curl/result
  )
endletmany (CMAKE_FIND_LIBRARY_SUFFIXES) 

find_package_handle_standard_args (curl 
  DEFAULT_MSG 
  CURL_LIBRARY
  CURL_INCLUDE_DIR
)                
set(CURL_LIBRARIES    ${CURL_LIBRARY})
set(CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR})



