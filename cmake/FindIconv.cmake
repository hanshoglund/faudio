# Find iconv library
#
# Author: Eddy Xu <eddyxu at gmail.com>
#
# Released under BSD license
#
#  ICONV_INCLUDE_DIRS   - where to find iconv.h, etc
#  ICONV_LIBRARIES      - Lists of libraries when using iconv
#  ICONV_FOUND          - True if iconv found


# Look for the header file
FIND_PATH( ICONV_INCLUDE_DIR NAMES iconv.h )
MARK_AS_ADVANCED( ICONV_INCLUDE_DIR )

# Look for the library
FIND_LIBRARY( ICONV_LIBRARY NAMES iconv )
MARK_AS_ADVANCED( ICONV_LIBRARY )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Iconv DEFAULT_MSG
    ICONV_LIBRARY ICONV_INCLUDE_DIR )

IF(ICONV_INCLUDE_DIR AND ICONV_LIBRARY)
  SET(ICONV_INCLUDE_DIRS ${ICONV_INCLUDE_DIR})
  SET(ICONV_LIBRARIES    ${ICONV_LIBRARY})
ENDIF()

MARK_AS_ADVANCED(ICONV_INCLUDE_DIR)
MARK_AS_ADVANCED(ICONV_LIBRARY)
