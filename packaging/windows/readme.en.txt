This document describes building the Windows version of Scan Tailor.

Earlier versions of Scan Tailor supported both Visual Studio and MinGW
compilers. MinGW support was dropped at some point, in order to reduce
maintenance effort. Furthermore, Scan Tailor started to use some C++11
features, making Visual Studio versions below 2013 not supported.


                                Downloading Prerequisites

First, download the following software.  Unless stated otherwise, take the
latest stable version.

1. Visual Studio 2017/2019 for Windows Desktop.
   Homepage: https://visualstudio.microsoft.com/
2. CMake >= 3.1.0
   Homepage: http://www.cmake.org
3. jpeg library (tested with 9c)
   Homepage: http://www.ijg.org/
   The file we need will be named jpegsrc.v9c.tar.gz or similarly.
4. zlib (tested with 1.2.11)
   Homepage: http://www.zlib.net/
   We need a file named like zlib-x.x.x.tar.gz, where x.x.x represents
   the version number.
5. libpng (tested with 1.6.37)
   Homepage: http://www.libpng.org/pub/png/libpng.html
   We need a file named like libpng-x.x.x.tar.gz, where x.x.x represents
   the version number.
6. libtiff (tested with 4.0.10)
   Homepage: http://www.simplesystems.org/libtiff/
   Get the source package from ftp://download.osgeo.org/libtiff/ .
   The file is named tiff-x.x.x.tar.gz, where x.x.x represents the
   version number.
7. freetype (tested with 2.10.1; needed for podofo)
   Homepage: https://sourceforge.net/projects/freetype/files/freetype2/
   We need a file named like freetype-x.x.x.tar.gz, where x.x.x represents
   the version number.
8. podofo (tested with 0.9.6)
   Homepage: http://podofo.sourceforge.net/download.html
   We need a file named like podofo-x.x.x.tar.gz, where x.x.x represents
   the version number.
9. openjpeg  (tested with 2.3.1)
   Homepage: http://www.openjpeg.org/
   This is needed for JPG2000 input image support.
10. Qt5 >= 5.3 (tested with 5.13.0)
   Homepage: http://qt-project.org/
   Either a source-only or any of the binary versions will do. In either case
   a custom build of Qt will be made, though a binary version will result in
   less things to build.
11. ActivePerl (necessary to build Qt5)
   Homepage: http://www.activestate.com/activeperl/downloads
   A 32-bit version is suggested. Whether or not 64-bit version would work
   is unclear. When installing make sure that "Add Perl to the PATH environment
   variable" option is set.
   Note: Perl seems not to be required for the source build of Qt 5.9.
12. Boost (tested with 1.70.0)
   Homepage: http://boost.org/
   You can download it in any file format, provided you know how to unpack it.
13. Eigen3 (tested with 3.3.7)
   Homepage: http://eigen.tuxfamily.org/
14. NSIS 2.x (tested with 2.46)
   Homepage: http://nsis.sourceforge.net/
15. For OpenCL support, install an OpenCL SDK from Intel, AMD or CUDA Toolkit
   from Nvidia.
   Note: You don't actually have to install it. Scantailor will find the
   header files and library if you set them up as follows (see below for
   directory structure):
   c:\build
      | opencl -> [opencl.lib goes here]
         | CL -> [headers go here]

                                    Instructions

1. Create a build directory. Its full path should have no spaces. From now on
   this document will be assuming the build directory is C:\build

2. Unpack jpeg, libpng, libtiff, zlib, openjpeg, freetype, podofo, boost,
   Eigen, and scantailor itself to the build directory.  You should get a
   directory structure like this:
   C:\build
     | boost_1_70_0
     | eigen-eigen-323c052e1731
     | jpeg-9c
     | libpng-1.6.37
     | opencl [optional, if no sdk is installed and you want opencl support]
     | openjpeg-2.3.1
     | podofo-0.9.6
     | qt-everywhere-src-5.13.0 [if building qt from source]
     | scantailor
     | tiff-4.0.10
     | zlib-1.2.11
   
   If you went for a source-only version of Qt, unpack it there as well.
   Otherwise, install Qt into whatever directory its installer suggests.
   IMPORTANT: Tell the installer to install Source Components as well.
   
   If you don't know how to unpack .tar.gz files, try this tool:
   http://www.7-zip.org/

3. Install Visual Studio, ActivePerl and CMake.

4. Create two more subdirectories there:
     | scantailor-build
     | scantailor-deps-build

5. Launch CMake and specify the following:

   Where is the source code: C:\build\scantailor\packaging\windows\build-deps
   Where to build the binaries: C:\build\scantailor-deps-build

   Click "Configure". Select the project type "Visual Studio 12/14" or
   "Visual Studio 12/14 Win64" for 64-bit builds. Keep in mind that 64-bit
   builds are only possible on a 64-bit version of Windows. Visual Studio 12
   is the same as Visual Studio 2013 and Visual Studio 14 is the same as
   Visual Studio 2015. If any of the paths weren't found,
   enter them manually, then click "Configure" again. If everything went right,
   the "Generate" button will become clickable. Click it. Sometimes it's
   necessary to click "Configure" more than once before "Generate" becomes
   clickable.

6. We will be building Scan Tailor's dependencies here. This step is the
   longest one (may take a few hours), but fortunately it only has to be done
   once. When building newer versions of Scan Tailor, you won't need to
   redo this step unless you want to update any libraries.
   
   Go to C:\build\scantailor-deps-build and open file
   "Scan Tailor Dependencies.sln".  It will open in Visual Studio.
   IMPORTANT: Set the build type to RelWithDebInfo. If you leave Debug
   (which is the default), your builds won't run on other computers.
   Even with build type set to RelWithDebInfo, some libraries (Qt, boost)
   will be built in both debug and release configurations. When building
   Scan Tailor itself, the appropriate library configuration will be
   selected automatically.
   
   Now do Build -> Build Solution.
   
   Make sure the building process finishes without errors. Warnings may
   be ignored.

7. Launch CMake again and specify following:

   Where is the source code: C:\build\scantailor
   Where to build the binaries: C:\build\scantailor-build

   Click "Configure", then "Generate", just like on step 5.

8. Now we are going to build Scan Tailor itself.  On subsequent builds of the
   same (possibly modified) version, you can start right from this step.
   For building a different version, start from step 7.
   
   Go to C:\build\scantailor-build and open file "Scan Tailor.sln".
   It will open in Visual Studio. Select the desired build type. Debug builds
   won't work on other computers.
   
   Now do Build -> Build Solution
   
   Make sure the building process finishes without errors. Warnings may
   be ignored.
      
   If everything went right, the installer named scantailor-VERSION-install.exe
   will appear in C:\build\scantailor-build. The VERSION part of the name will
   be replaced by the actual version, taken from the version.h file in the root
   of the source directory.
