CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

SET(
	CMAKE_USER_MAKE_RULES_OVERRIDE
	"${CMAKE_SOURCE_DIR}/../../../cmake/default_cflags.cmake"
)
SET(
	CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
	"${CMAKE_SOURCE_DIR}/../../../cmake/default_cxxflags.cmake"
)

PROJECT("Scan Tailor Dependencies")

INCLUDE(../../../cmake/SetDefaultBuildType.cmake)
INCLUDE(../../../cmake/SetDefaultGccFlags.cmake)
INCLUDE(../../../cmake/ToNativePath.cmake)
INCLUDE(../../../cmake/PatchFile.cmake)
INCLUDE(TestCXXAcceptsFlag)

ST_SET_DEFAULT_BUILD_TYPE(Release)
ST_SET_DEFAULT_GCC_FLAGS()

GET_FILENAME_COMPONENT(source_outer_dir "${PROJECT_SOURCE_DIR}/../../../.." ABSOLUTE)
GET_FILENAME_COMPONENT(build_outer_dir "${PROJECT_BINARY_DIR}/.." ABSOLUTE)


SET(suggest_jom FALSE)

IF(MSVC)
	FIND_PROGRAM(JOM_EXECUTABLE jom HINTS "${source_outer_dir}" "${build_outer_dir}")
	IF(JOM_EXECUTABLE)
		TO_NATIVE_PATH("${JOM_EXECUTABLE}" make_command)
	ELSE()
		SET(make_command nmake)
		SET(suggest_jom TRUE)
	ENDIF()

	IF(MSVC60 OR MSVC70 OR MSVC71 OR MSVC80 OR MSVC90 OR MSVC10 OR MSVC11)
		MESSAGE(FATAL_ERROR "This Visual Studio version is too old and is not supported. Only Visual Studio 2013 and 2015 are currently supported.")
	ELSEIF(MSVC12)
		SET(qt_platform win32-msvc)
		SET(boost_toolset msvc-12.0)
	ELSEIF(MSVC14)
		SET(qt_platform win32-msvc)
		SET(boost_toolset msvc-14.0)
	ELSE()
		MESSAGE(FATAL_ERROR "A new and unsupported version of Visual Studio was detected. Only Visual Studio 2013 and 2015 are currently supported.")
	ENDIF()
ELSE()
	MESSAGE(FATAL_ERROR "Unsupported compiler. Only Visual Studio 2013 and 2015 is currently supported.")
ENDIF()


FILE(GLOB jpeg_dirs1 "${build_outer_dir}/jpeg-[0-9][a-z]")
FILE(GLOB jpeg_dirs2 "${source_outer_dir}/jpeg-[0-9][a-z]")
FILE(GLOB zlib_dirs1 "${build_outer_dir}/zlib-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB zlib_dirs2 "${source_outer_dir}/zlib-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB png_dirs1 "${build_outer_dir}/libpng-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB png_dirs2 "${source_outer_dir}/libpng-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB png_dirs3 "${build_outer_dir}/lpng[0-9]*")
FILE(GLOB png_dirs4 "${source_outer_dir}/lpng[0-9]*")
FILE(GLOB tiff_dirs1 "${build_outer_dir}/tiff-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB tiff_dirs2 "${source_outer_dir}/tiff-[0-9]*.[0-9]*.[0-9]*")	
FILE(GLOB openjpeg_dirs1 "${build_outer_dir}/openjpeg-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB openjpeg_dirs2 "${source_outer_dir}/openjpeg-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB freetype_dirs1 "${build_outer_dir}/freetype-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB freetype_dirs2 "${source_outer_dir}/freetype-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB podofo_dirs1 "${build_outer_dir}/podofo-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB podofo_dirs2 "${source_outer_dir}/podofo-[0-9]*.[0-9]*.[0-9]*")
FILE(GLOB qt_dirs1 "${build_outer_dir}/qt-win-*-5.[0-9]*")
FILE(GLOB qt_dirs2 "${build_outer_dir}/qt-everywhere-*-5.[0-9]*")
FILE(GLOB qt_dirs3 "${source_outer_dir}/qt-win-*-5.[0-9]*")
FILE(GLOB qt_dirs4 "${source_outer_dir}/qt-everywhere-*-5.[0-9]*")
FILE(GLOB qt_dirs5 "C:/Qt/Qt5.*/*/Src")
FILE(GLOB boost_dirs1 "${build_outer_dir}/boost_1_[0-9]*_[0-9]*")
FILE(GLOB boost_dirs2 "${source_outer_dir}/boost_1_[0-9]*_[0-9]*")

FIND_PATH(
	JPEG_DIR jpeglib.h HINTS ${jpeg_dirs1} ${jpeg_dirs2}
	DOC "Path to jpeg source directory."
)
FIND_PATH(
	ZLIB_DIR zlib.h HINTS ${zlib_dirs1} ${zlib_dirs2}
	DOC "Path to zlib source directory."
)
FIND_PATH(
	PNG_DIR png.h HINTS ${png_dirs1} ${png_dirs2} ${png_dirs3} ${png_dirs4}
	DOC "Path to libpng source directory."
)
FIND_PATH(
	TIFF_DIR libtiff/tiff.h HINTS ${tiff_dirs1} ${tiff_dirs2}
	DOC "Path to top-level tiff source directory."
)
FIND_PATH(
	OPENJPEG_DIR src/lib/openjp2/openjpeg.h HINTS ${openjpeg_dirs1} ${openjpeg_dirs2}
	DOC "Path to top-level openjpeg source directory."
)
FIND_PATH(
	FREETYPE_DIR ChangeLog.20 HINTS ${freetype_dirs1} ${freetype_dirs2}
	DOC "Path to top-level freetype source directory."
)
FIND_PATH(
	PODOFO_DIR podofo_config.h.in HINTS ${podofo_dirs1} ${podofo_dirs2}
	DOC "Path to top-level podofo source directory."
)
FIND_PATH(
	QT_SRC_DIR NAMES qtbase/qtbase.pro HINTS ${qt_dirs1} ${qt_dirs2}
	${qt_dirs3} ${qt_dirs4} ${qt_dirs5}
	DOC "Path to top-level Qt5 source directory. If you installed a binary version, it will be something like C:\\Qt\\Qt5.0.2\\5.0.2\\Src"
)
FIND_PATH(
	BOOST_DIR boost/foreach.hpp HINTS ${boost_dirs1} ${boost_dirs2}
	DOC "Path to top-level Boost source directory."
)

IF(NOT JPEG_DIR)
	MESSAGE(FATAL_ERROR "JPEG source directory not found. You may specify it manually.")
ELSEIF(NOT ZLIB_DIR)
	MESSAGE(FATAL_ERROR "ZLIB source directory not found. You may specify it manually.")
ELSEIF(NOT PNG_DIR)
	MESSAGE(FATAL_ERROR "LibPNG source directory not found. You may specify it manually.")
ELSEIF(NOT TIFF_DIR)
	MESSAGE(FATAL_ERROR "TIFF source directory not found. You may specify it manually.")
ELSEIF(NOT OPENJPEG_DIR)
	MESSAGE(FATAL_ERROR "OpenJPEG source directory not found. You may specify it manually.")
ELSEIF(NOT FREETYPE_DIR)
	MESSAGE(FATAL_ERROR "Freetype source directory not found. You may specify it manually.")	
ELSEIF(NOT PODOFO_DIR)
	MESSAGE(FATAL_ERROR "Podofo source directory not found. You may specify it manually.")
ELSEIF(NOT QT_SRC_DIR)
	MESSAGE(FATAL_ERROR "Qt5 source directory not found. You may specify it manually. If you installed a binary version, it will be something like C:\\Qt\\Qt5.0.2\\5.0.2\\Src")
ELSEIF(NOT BOOST_DIR)
	MESSAGE(FATAL_ERROR "Boost source directory not found. You may specify it manually.")
ENDIF()

SET(QTBASE_DIR "${QT_SRC_DIR}/qtbase")
SET(QTBUILD_DIR "${PROJECT_BINARY_DIR}/qt-build")

FILE(GLOB qt_possible_prebuilt_dirs "${QTBUILD_DIR}/*")
UNSET(QT_PREBUILT_DIR)
FIND_PATH(
	QT_PREBUILT_DIR bin/linguist.exe HINTS ${qt_possible_prebuilt_dirs}
	DOC "[optional] Installation path of a pre-built version of Qt5 including qttools. If you installed a binary version, it will be something like C:\\Qt\\Qt5.0.2\\5.0.2\\msvc2012"
)

SET(ZLIB_LIBRARY_NAME zdll)
SET(PNG_LIBRARY_NAME libpng)
SET(JPEG_LIBRARY_NAME libjpeg)
SET(TIFF_LIBRARY_NAME libtiff)
SET(OPENJPEG_LIBRARY_NAME openjp2)
SET(FREETYPE_LIBRARY_NAME freetype)
SET(PODOFO_LIBRARY_NAME podofo)

MACRO(LIST_ITEMS_PREPEND LIST PREFIX)
	SET(tmp_list_)
	FOREACH(item ${${LIST}})
		LIST(APPEND tmp_list_ "${PREFIX}${item}")
	ENDFOREACH(item)
	SET(${LIST} ${tmp_list_})
ENDMACRO(LIST_ITEMS_PREPEND)


#=================================== JPEG ===================================#

# Patch jmorecfg.h to:
# 1. Prevent double definition of INT32.
# 2. Build a DLL rather than a static library.
IF(NOT EXISTS "${JPEG_DIR}/jmorecfg.h.orig")
	FILE(READ "${JPEG_DIR}/jmorecfg.h" jmorecfg_h_orig)
	STRING(REPLACE "XMD_H" "_BASETSD_H" jmorecfg_h "${jmorecfg_h_orig}")
	STRING(
		REGEX REPLACE "#define[ \t]+GLOBAL\\(type\\)[^\n]*"
		"#ifdef JPEG_BUILD\n#define GLOBAL(type) __declspec(dllexport) type\n#else\n#define GLOBAL(type) __declspec(dllimport) type\n#endif"
		jmorecfg_h "${jmorecfg_h}"
	)
	STRING(
		REGEX REPLACE "#define[ \t]+EXTERN\\(type\\)[^\n]*"
		"#ifdef JPEG_BUILD\n#define EXTERN(type) extern __declspec(dllexport) type\n#else\n#define EXTERN(type) extern __declspec(dllimport) type\n#endif"
		jmorecfg_h "${jmorecfg_h}"
	)
	FILE(WRITE "${JPEG_DIR}/jmorecfg.h" "${jmorecfg_h}")
	FILE(WRITE "${JPEG_DIR}/jmorecfg.h.orig" "${jmorecfg_h_orig}")
	SET(jmorecfg_h "")
	SET(jmorecfg_h_orig "")
ENDIF(NOT EXISTS "${JPEG_DIR}/jmorecfg.h.orig")

SET(
	libjpeg_sources
	jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c
	jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c
	jcomapi.c jcparam.c jcprepct.c jcsample.c jctrans.c jdapimin.c
	jdapistd.c jdarith.c jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c
	jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c
	jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c
	jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c jquant1.c
	jquant2.c jutils.c jmemmgr.c jmemnobs.c
)
LIST_ITEMS_PREPEND(libjpeg_sources "${JPEG_DIR}/")

CONFIGURE_FILE("${JPEG_DIR}/jconfig.vc" "${JPEG_DIR}/jconfig.h" COPYONLY)

ADD_LIBRARY(${JPEG_LIBRARY_NAME} SHARED ${libjpeg_sources})
SET_TARGET_PROPERTIES(
	${JPEG_LIBRARY_NAME} PROPERTIES
	DEFINE_SYMBOL JPEG_BUILD
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/staging/libs"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${QTBUILD_DIR}/bin"
)


#=================================== ZLIB ===================================#

SET(
	zlib_sources
	adler32.c compress.c crc32.c deflate.c inffast.c inflate.c
	inftrees.c trees.c uncompr.c zutil.c
)
LIST_ITEMS_PREPEND(zlib_sources "${ZLIB_DIR}/")

ADD_LIBRARY(${ZLIB_LIBRARY_NAME} SHARED ${zlib_sources})
SET_TARGET_PROPERTIES(
	${ZLIB_LIBRARY_NAME} PROPERTIES
	DEFINE_SYMBOL ZLIB_DLL
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/staging/libs"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${QTBUILD_DIR}/bin"
)


#================================== LIBPNG ==================================#

IF(NOT EXISTS "${PNG_DIR}/pnglibconf.h")
	IF(EXISTS "${PNG_DIR}/scripts/pnglibconf.h.prebuilt")
		CONFIGURE_FILE(
			"${PNG_DIR}/scripts/pnglibconf.h.prebuilt"
			"${PNG_DIR}/pnglibconf.h" COPYONLY
		)
	ENDIF()
ENDIF()

INCLUDE_DIRECTORIES(${ZLIB_DIR})

SET(
	libpng_sources
	png.c pngset.c pngget.c pngrutil.c pngtrans.c pngwutil.c pngread.c
	pngrio.c pngwio.c pngwrite.c pngrtran.c pngwtran.c pngmem.c
	pngerror.c pngpread.c
)
LIST_ITEMS_PREPEND(libpng_sources "${PNG_DIR}/")

ADD_DEFINITIONS(-DPNG_NO_MODULEDEF)
ADD_LIBRARY(${PNG_LIBRARY_NAME} SHARED ${libpng_sources})
TARGET_LINK_LIBRARIES(${PNG_LIBRARY_NAME} ${ZLIB_LIBRARY_NAME})
SET_TARGET_PROPERTIES(
	${PNG_LIBRARY_NAME} PROPERTIES
	DEFINE_SYMBOL PNG_BUILD_DLL
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/staging/libs"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${QTBUILD_DIR}/bin"
)


#=================================== TIFF ===================================#

INCLUDE_DIRECTORIES("${JPEG_DIR}") # ZLIB_DIR already included above

SET(
	libtiff_sources
	tif_aux.c tif_close.c tif_codec.c tif_color.c tif_compress.c
	tif_dir.c tif_dirinfo.c tif_dirread.c tif_dirwrite.c
	tif_dumpmode.c tif_error.c tif_extension.c tif_fax3.c
	tif_fax3sm.c tif_getimage.c tif_jpeg.c tif_ojpeg.c tif_flush.c
	tif_luv.c tif_lzw.c tif_next.c tif_open.c tif_packbits.c
	tif_pixarlog.c tif_predict.c tif_print.c tif_read.c tif_swab.c
	tif_strip.c tif_thunder.c tif_tile.c tif_version.c
	tif_warning.c tif_write.c tif_zip.c tif_unix.c libtiff.def
)
LIST_ITEMS_PREPEND(libtiff_sources "${TIFF_DIR}/libtiff/")

SET(tiff_vc_config "${TIFF_DIR}/libtiff/tif_config.vc.h")
IF(EXISTS "${TIFF_DIR}/libtiff/tif_config.h.vc")
	SET(tiff_vc_config "${TIFF_DIR}/libtiff/tif_config.h.vc")
ENDIF(EXISTS "${TIFF_DIR}/libtiff/tif_config.h.vc")
CONFIGURE_FILE(
	"${tiff_vc_config}" "${TIFF_DIR}/libtiff/tif_config.h" COPYONLY
)
IF(EXISTS "${TIFF_DIR}/libtiff/tiffconf.vc.h")
	CONFIGURE_FILE(
		"${TIFF_DIR}/libtiff/tiffconf.vc.h"
		"${TIFF_DIR}/libtiff/tiffconf.h" COPYONLY
	)
ENDIF(EXISTS "${TIFF_DIR}/libtiff/tiffconf.vc.h")

ADD_LIBRARY(${TIFF_LIBRARY_NAME} SHARED ${libtiff_sources})
TARGET_LINK_LIBRARIES(
	${TIFF_LIBRARY_NAME}
	${JPEG_LIBRARY_NAME} ${ZLIB_LIBRARY_NAME}
)
SET_TARGET_PROPERTIES(
	${TIFF_LIBRARY_NAME} PROPERTIES
	COMPILE_FLAGS "-DJPEG_SUPPORT -DOJPEG_SUPPORT -DZIP_SUPPORT"
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/staging/libs"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${QTBUILD_DIR}/bin"
)


#================================= OPENJPEG =================================#

SET(
	openjpeg_sources
	thread.c bio.c cio.c dwt.c event.c image.c invert.c j2k.c jp2.c mct.c mqc.c
	openjpeg.c opj_clock.c pi.c t1.c t2.c tcd.c tgt.c function_list.c opj_malloc.c
)
LIST_ITEMS_PREPEND(openjpeg_sources "${OPENJPEG_DIR}/src/lib/openjp2/")

CONFIGURE_FILE(
  ${OPENJPEG_DIR}/src/lib/openjp2/opj_config.h.cmake.in
  ${OPENJPEG_DIR}/src/lib/openjp2/opj_config.h
  @ONLY
)

CONFIGURE_FILE(
  ${OPENJPEG_DIR}/src/lib/openjp2/opj_config_private.h.cmake.in
  ${OPENJPEG_DIR}/src/lib/openjp2/opj_config_private.h
  @ONLY
)

add_definitions(-DOPJ_EXPORTS)

ADD_LIBRARY(${OPENJPEG_LIBRARY_NAME} SHARED ${openjpeg_sources})

TARGET_LINK_LIBRARIES(
	${OPENJPEG_LIBRARY_NAME}
  ${PNG_LIBRARY_NAME} ${TIFF_LIBRARY_NAME}
)

SET_TARGET_PROPERTIES(
	${OPENJPEG_LIBRARY_NAME} PROPERTIES
	DEFINE_SYMBOL OPENJPEG_BUILD
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/staging/libs"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${QTBUILD_DIR}/bin"
)


#================================= FREETYPE =================================#

INCLUDE_DIRECTORIES(${PNG_DIR})

# Patch ftoption.h to:
# 1. Use our zlib and png libs.
# 2. Build shared library.
FILE(READ "${FREETYPE_DIR}/include/freetype/config/ftoption.h"
  FTOPTION_H)
STRING(REGEX REPLACE
  "/\\* +(#define +FT_CONFIG_OPTION_SYSTEM_ZLIB) +\\*/" "\\1"
  FTOPTION_H "${FTOPTION_H}")
STRING(REGEX REPLACE
  "/\\* +(#define +FT_CONFIG_OPTION_USE_PNG) +\\*/" "\\1"
  FTOPTION_H "${FTOPTION_H}")
STRING(REPLACE "/* #define FT_EXPORT(x)      extern x */"
		"#define FT_EXPORT(x) __declspec(dllexport) x"
		FTOPTION_H "${FTOPTION_H}")
STRING(REPLACE "/* #define FT_EXPORT_DEF(x)  x */"
		"#define FT_EXPORT_DEF(x)  __declspec(dllexport) x"
		FTOPTION_H "${FTOPTION_H}")
FILE(WRITE "${FREETYPE_DIR}/include/freetype/config/ftoption.h-new"
  "${FTOPTION_H}")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${FREETYPE_DIR}/include/freetype/config/ftoption.h-new"
  "${FREETYPE_DIR}/include/freetype/config/ftoption.h")
	
set(freetype_sources
  src/autofit/autofit.c src/base/ftbase.c src/base/ftbbox.c src/base/ftbdf.c
  src/base/ftbitmap.c src/base/ftcid.c src/base/ftfntfmt.c src/base/ftfstype.c
  src/base/ftgasp.c src/base/ftglyph.c src/base/ftgxval.c src/base/ftinit.c
  src/base/ftlcdfil.c src/base/ftmm.c src/base/ftotval.c src/base/ftpatent.c
  src/base/ftpfr.c src/base/ftstroke.c src/base/ftsynth.c src/base/ftsystem.c
  src/base/fttype1.c src/base/ftwinfnt.c src/bdf/bdf.c src/bzip2/ftbzip2.c
  src/cache/ftcache.c src/cff/cff.c src/cid/type1cid.c src/gzip/ftgzip.c
  src/lzw/ftlzw.c src/pcf/pcf.c src/pfr/pfr.c src/psaux/psaux.c
	src/pshinter/pshinter.c	src/psnames/psnames.c src/raster/raster.c
  src/sfnt/sfnt.c src/smooth/smooth.c src/truetype/truetype.c
  src/type1/type1.c src/type42/type42.c src/winfonts/winfnt.c
)

if (WIN32)
  set(freetype_sources ${freetype_sources} builds/windows/ftdebug.c)
else (WIN32)
  set(freetype_sources ${freetype_sources} src/base/ftdebug.c)
endif (WIN32)

LIST_ITEMS_PREPEND(freetype_sources "${FREETYPE_DIR}/")

add_definitions(-DFT2_BUILD_LIBRARY)

INCLUDE_DIRECTORIES("${FREETYPE_DIR}/include")

ADD_LIBRARY(${FREETYPE_LIBRARY_NAME} SHARED ${freetype_sources})

target_link_libraries(
	${FREETYPE_LIBRARY_NAME}
	${ZLIB_LIBRARY_NAME} ${PNG_LIBRARY_NAME}
)

SET_TARGET_PROPERTIES(
	${FREETYPE_LIBRARY_NAME} PROPERTIES
	DEFINE_SYMBOL FREETYPE_DLL
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/staging/libs"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${QTBUILD_DIR}/bin"
)


#================================== PODOFO ==================================#

# we use a bit of a different approach here then above and add podofo as as
# an external subproject

# Tell Podofo the correct dependency paths
IF(WIN32)
	SET(LIB_EXT ".lib")
ELSE()
	SET(LIB_EXT ".so")
ENDIF()

SET(PODOFO_BUILD_LIB_ONLY ON)
SET(ZLIB_INCLUDE_DIR ${ZLIB_DIR})	
SET(ZLIB_LIBRARY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs/${ZLIB_LIBRARY_NAME}${LIB_EXT}")
SET(LIBJPEG_INCLUDE_DIR ${JPEG_DIR})	
SET(LIBJPEG_LIBRARY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs/${JPEG_LIBRARY_NAME}${LIB_EXT}")	
SET(PNG_PNG_INCLUDE_DIR ${PNG_DIR})	
SET(PNG_LIBRARY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs/${PNG_LIBRARY_NAME}${LIB_EXT}")	
SET(TIFF_INCLUDE_DIR "${TIFF_DIR}/libtiff")	
SET(TIFF_LIBRARY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs/${TIFF_LIBRARY_NAME}${LIB_EXT}")	
SET(FREETYPE_INCLUDE_DIR_FT2BUILD "${FREETYPE_DIR}/include")
SET(FREETYPE_INCLUDE_DIR_FTHEADER "${FREETYPE_DIR}/include/freetype")	
SET(FREETYPE_LIBRARY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs/${FREETYPE_LIBRARY_NAME}${LIB_EXT}")	

# Include podofo project
ADD_SUBDIRECTORY(${PODOFO_DIR} "${PROJECT_BINARY_DIR}/podofo-build")

# Modify target to make sure libs are copied to the correct place
SET_TARGET_PROPERTIES(
	podofo_shared PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/staging/libs"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/staging/libs"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${QTBUILD_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${QTBUILD_DIR}/bin"
)

#================================= Boost ================================#

SET(boost_64bit_flags "")
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	LIST(APPEND boost_64bit_flags "address-model=64")
ENDIF()

ADD_CUSTOM_TARGET(
	boost ALL
	COMMAND cmd /c bootstrap
	COMMAND b2 --with-test toolset=${boost_toolset}
	threading=multi link=static variant=debug,release ${boost_64bit_flags}
	stage
	WORKING_DIRECTORY "${BOOST_DIR}"
	VERBATIM
)


#=================================== Qt ===================================#

# Backward compatibility.  We used to patch projects.pro, but now we achieve
# the same result by other means.
IF(EXISTS "${QTBASE_DIR}/projects.pro.orig")
	CONFIGURE_FILE(
		"${QTBASE_DIR}/projects.pro.orig"
		"${QTBASE_DIR}/projects.pro" COPYONLY
	)
	FILE(REMOVE "${QTBASE_DIR}/projects.pro.orig")
ENDIF(EXISTS "${QTBASE_DIR}/projects.pro.orig")


TO_NATIVE_PATH("${JPEG_DIR}" JPEG_INCLUDE_DIR)
TO_NATIVE_PATH("${ZLIB_DIR}" ZLIB_INCLUDE_DIR)
TO_NATIVE_PATH("${PNG_DIR}" PNG_INCLUDE_DIR)
TO_NATIVE_PATH("${FREETYPE_DIR}/include" FREETYPE_INCLUDE_DIR)
TO_NATIVE_PATH("${QTBASE_DIR}" QTBASE_DIR_NATIVE)


# Find all *.conf files under mkspecs that mention -Zc:wchar_t- and remove
# that minus at the end.  That's necessary to make Qt compatible with other
# libraries that use wchar_t stuff.
FILE(GLOB_RECURSE conf_files "${QTBASE_DIR}/mkspecs/*.conf")
FOREACH(conf_file ${conf_files})
	FILE(READ "${conf_file}" contents)
	STRING(REGEX REPLACE "-Zc:wchar_t-" "-Zc:wchar_t" new_contents "${contents}")
	IF(NOT "${contents}" STREQUAL "${new_contents}")
		# Make a backup copy, if not already there.
		IF(NOT EXISTS "${conf_file}.orig")
			CONFIGURE_FILE("${conf_file}" "${conf_file}.orig" COPYONLY)
		ENDIF()

		FILE(WRITE "${conf_file}" "${new_contents}")
	ENDIF()
ENDFOREACH()


IF(MSVC)
	# Disable frame pointer ommission in release mode to ensure
	# good stack traces from our built-in crash reporter.
	PATCH_FILE(
		"${QTBASE_DIR}/mkspecs/${qt_platform}/qmake.conf"
		
		# Add the flag.
		"/(QMAKE_CFLAGS_RELEASE[ \t]*=)/\\1 -Oy-/"
		
		# Get rid of duplicates, in case it was already there.
		"/( -Oy-)+/ -Oy-/"
	)
ENDIF()
	

# Force debugging symbols even for release builds.
# Qt5 has a -force-debug-info configure option
# but I had to do the following to make it actually work:
PATCH_FILE(
	"${QTBASE_DIR}/.qmake.conf"
	
	# Put "CONFIG += force_debug_info" at the end.
	"/(.)$/\\1\nCONFIG += force_debug_info\n/"
)


SET(qt_build_script "${CMAKE_BINARY_DIR}/build-qt.bat")

SET(maybe_skip_building_tools "")
IF(QT_PREBUILT_DIR)
	SET(maybe_skip_building_tools "goto exit")
ENDIF()

ADD_CUSTOM_COMMAND(
	OUTPUT "${qt_build_script}"
	COMMAND "${CMAKE_COMMAND}"
	"-DPROJECT_ROOT=${CMAKE_SOURCE_DIR}/../../.."
	"-DTARGET_FILE=${qt_build_script}"
	"-DPLATFORM=${qt_platform}"
	"-DMAYBE_SKIP_BUILDING_TOOLS=${maybe_skip_building_tools}"
	"-DMAKE_COMMAND=${make_command}"
	"-DQTBUILD_DIR=${QTBUILD_DIR}"
	"-DQTBASE_DIR=${QTBASE_DIR}"
	"-DJPEG_INCLUDE_DIR=${JPEG_INCLUDE_DIR}"
	"-DZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR}"
	"-DPNG_INCLUDE_DIR=${PNG_INCLUDE_DIR}"
	"-DFREETYPE_INCLUDE_DIR=${FREETYPE_INCLUDE_DIR}"
	"-DJPEG_LINK_DIR=$<TARGET_LINKER_FILE_DIR:${JPEG_LIBRARY_NAME}>"
	"-DZLIB_LINK_DIR=$<TARGET_LINKER_FILE_DIR:${ZLIB_LIBRARY_NAME}>"
	"-DPNG_LINK_DIR=$<TARGET_LINKER_FILE_DIR:${PNG_LIBRARY_NAME}>"
	"-DFREETYPE_LINK_DIR=$<TARGET_LINKER_FILE_DIR:${FREETYPE_LIBRARY_NAME}>"
	-P "${CMAKE_CURRENT_SOURCE_DIR}/generate_qt_build_script.cmake"
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/generate_qt_build_script.cmake"
	VERBATIM
)
	
ADD_CUSTOM_TARGET(
	Qt ALL
	COMMAND "${qt_build_script}"
	DEPENDS "${qt_build_script}"
	WORKING_DIRECTORY "${QTBUILD_DIR}"
)

# boost added so that boost is built before Qt. That's helpful if build
# errors occur, as rebuilding boost is much faster than rebuilding Qt.
ADD_DEPENDENCIES(
	Qt ${ZLIB_LIBRARY_NAME} ${JPEG_LIBRARY_NAME}
	${PNG_LIBRARY_NAME} ${TIFF_LIBRARY_NAME} ${OPENJPEG_LIBRARY_NAME}
	${FREETYPE_LIBRARY_NAME}
)

# If you have multiple versions of Qt installed, this script
# can easily pick the wrong one.
MESSAGE(STATUS "----------------------------")
MESSAGE(STATUS "QT_SRC_DIR is ${QT_SRC_DIR}")
IF(QT_PREBUILT_DIR)
	MESSAGE(STATUS "QT_PREBUILT_DIR is ${QT_PREBUILT_DIR}")
ELSE()
	MESSAGE(STATUS "QT_PREBUILT_DIR is not set. Using a source-only version of Qt? Fine, though \
building will take longer.")
ENDIF()
MESSAGE(STATUS "If you've got multiple versions of Qt installed, check we've got the right one.")
IF(suggest_jom)
	MESSAGE(STATUS "Note that building Qt with NMAKE is slow. Consider using JOM instead. \
Drop jom.exe to the folder with other dependencies and/or set JOM_EXECUTABLE to point to it.")
ENDIF()
MESSAGE(STATUS "----------------------------")


# Dump symbols of all DLLs we are going to link to.
OPTION(DUMP_DLL_SYMBOLS "Enable if you are going to build crash reporter." OFF)
IF(DUMP_DLL_SYMBOLS)
	FIND_PATH(
		SYMBOLS_PATH . PATHS "${build_outer_dir}/symbols"
		"${source_outer_dir}/symbols" NO_DEFAULT_PATH
		DOC "The directory to write symbol information into."
	)
	IF(NOT SYMBOLS_PATH)
		MESSAGE(FATAL_ERROR "SYMBOLS_PATH directory is not set.")
	ENDIF(NOT SYMBOLS_PATH)

	# We can't build it, because it requires ATL, which is not part
	# of the Visual Studio Express Edition, so we rely on a pre-built
	# version which can be found in the Mozilla repository.
	FIND_PROGRAM(
		DUMP_SYMS_EXECUTABLE dump_syms PATHS "${build_outer_dir}" "${source_outer_dir}"
		DOC "Path to dump_syms.exe, which can be found in Mozilla repository."
	)
	IF(NOT DUMP_SYMS_EXECUTABLE)
		MESSAGE(
			FATAL_ERROR "dump_syms.exe wasn't found.  Specify its location manually by setting the DUMP_SYMS_EXECUTABLE variable.  dump_syms.exe may be found in the Mozilla repository under /toolkit/crashreporter/tools/win32"
		)
	ENDIF(NOT DUMP_SYMS_EXECUTABLE)
	
	ADD_CUSTOM_TARGET(
		dump_dll_symbols ALL
		COMMAND "${CMAKE_COMMAND}" "-DSYMBOLS_PATH=${SYMBOLS_PATH}"
		"-DDUMP_SYMS_EXECUTABLE=${DUMP_SYMS_EXECUTABLE}"
		"-DMOVE_SYMBOLS_SCRIPT=${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/move_sym_file.cmake"
		-P "${CMAKE_CURRENT_SOURCE_DIR}/dump_dll_syms.cmake"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/dump_dll_syms.cmake"
		WORKING_DIRECTORY "${QTBASE_DIR}/bin"
		VERBATIM
	)
	
	# Qt depends on the rest of them.
	ADD_DEPENDENCIES(dump_dll_symbols Qt)
ENDIF()

#==========================================================================#

CONFIGURE_FILE(export-vars.cmake.in "${CMAKE_BINARY_DIR}/export-vars.cmake" @ONLY)
