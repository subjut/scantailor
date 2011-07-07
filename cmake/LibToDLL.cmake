# Usage:
# LIB_TO_DLL(output_list_of_dlls ${list_of_dot_lib_files})
MACRO(LIB_TO_DLL out_list_)
	SET(${out_list_} "")
	FOREACH(lib_file_ ${ARGN})
		#STRING(REGEX REPLACE "\\.lib$" ".dll" dll_file_ "${lib_file_}")
		#IF(NOT "${dll_file_}" STREQUAL "${lib_file_}")
		#	LIST(APPEND ${out_list_} "${dll_file_}")
		#ENDIF()
		GET_FILENAME_COMPONENT(lib_file_name_ "${lib_file_}" NAME)
		GET_FILENAME_COMPONENT(dir_ "${lib_file_}" PATH)
		
		STRING(REGEX REPLACE "^lib(.*)\\.a$" "\\1.dll" dll_file_name_ "${lib_file_name_}")
		IF("${dll_file_name_}" STREQUAL "${lib_file_name_}")
 			STRING(REGEX REPLACE "^(.*)\\.lib$" "\\1.dll" dll_file_name_ "${lib_file_name_}")
		ENDIF()
		
		IF(NOT "${dll_file_name_}" STREQUAL "${lib_file_name_}")
			LIST(APPEND ${out_list_} "${dir_}/${dll_file_name_}")
		ENDIF()
	ENDFOREACH()
ENDMACRO()