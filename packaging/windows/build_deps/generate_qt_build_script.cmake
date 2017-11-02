FILE(
	WRITE "${TARGET_FILE}"
	# Without .gitignore Qt 5 assumes configure.exe is present.
	"if not exist configure.exe type nul >>.gitignore"
	"\n"
	"cd \"${QTBUILD_DIR}\"\n"
	# cmd /c is used because it may be configure.bat, which otherwise wouldn't return control.
	"cmd /c ${QTBASE_DIR}\\configure -platform ${PLATFORM}"
	" -debug-and-release -shared -force-debug-info"
	" -no-gif -system-zlib -system-libpng -system-libjpeg -no-openssl"
	" -opengl desktop -nomake examples -nomake tests -opensource -confirm-license -no-ltcg"
	" -I \"${JPEG_INCLUDE_DIR}\" -I \"${ZLIB_INCLUDE_DIR}\""
	" -I \"${PNG_INCLUDE_DIR}\" -I \"${FREETYPE_INCLUDE_DIR}\""
	" -L \"${JPEG_LINK_DIR}\" -L \"${ZLIB_LINK_DIR}\""
	" -L \"${PNG_LINK_DIR}\""
	" -D _BIND_TO_CURRENT_VCLIBS_VERSION=1"
	"\n"
	"if errorlevel 1 goto exit\n"
	"${MAKE_COMMAND}\n"
	"if errorlevel 1 goto exit\n"
	# Build qttools if not yet done
	"${MAYBE_SKIP_BUILDING_TOOLS}\n"
	"mkdir \"${QTBUILD_DIR}\\..\\qttools-build\"\n"
	"cd \"${QTBUILD_DIR}\\..\\qttools-build\"\n"
	"if errorlevel 1 goto exit\n"
	"\"${QTBUILD_DIR}\\bin\\qmake.exe\" -makefile -after \"CONFIG += release force_debug_info\" \"${QTBASE_DIR}\\..\\qttools\\qttools.pro\"\n"
	"if errorlevel 1 goto exit\n"
	"${MAKE_COMMAND}\n"
	"if errorlevel 1 goto exit\n"
	# move qttools over to qt-build dir
	"move /Y \"${QTBUILD_DIR}\\..\\qttools-build\\bin\\*\" \"${QTBUILD_DIR}\\bin\"\n"
	"robocopy \"${QTBUILD_DIR}\\..\\qttools-build\\lib" \"${QTBUILD_DIR}\\lib /move /s\"\n"
	# copy platform file for windows so qdesigner etc. work
	"mkdir \"${QTBUILD_DIR}\\bin\\platforms\"\n"
	"copy /Y \"${QTBUILD_DIR}\\plugins\\platforms\\qwindows*" \"${QTBUILD_DIR}\\bin\\platforms\"\n"
	":exit\n"
)
