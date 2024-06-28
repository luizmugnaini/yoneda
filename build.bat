@echo off

:: --- Environment setup --------------------------------------------------------

where /Q cl.exe || (
	set __VSCMD_ARG_NO_LOGO=1
	for /f "tokens=*" %%i in ('"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationPath') do set "VS=%%i"
	if "!VS!" equ "" (
		echo ERROR: Visual Studio installation not found
		exit /b 1
	)
	call "!VS!\VC\Auxiliary\Build\vcvarsall.bat" x64 || exit /b 1
)

:: --- Argument unpacking -------------------------------------------------------

for %%a in (%*) do set "%%a=1"

:: Defaults: MSVC and debug build.
if not "%msvc%"=="1" if not "%clang%"=="1" set msvc=1
if not "%release%"=="1" set debug=1

if "%debug%"=="1" (
   set release=0
   echo [debug mode]
)
if "%release%"=="1" (
   set debug=0
   echo [release mode]
)

:: Compiler explicitly specified
if "%msvc%"=="1" (
   set clang=0
   echo [msvc compile]
)
if "%clang%"=="1" (
   set msvc=0
   echo [clang compile]
)

if "%1" == "release" (
   set release_build=1
   echo [release build]
) else (
   set release_build=0
   echo [debug build]
)

:: --- Yoneda lib stuff ---------------------------------------------------------

set inc_dir=..\include
set src_files=..\src\*.c

set obj_files=*.obj
set out_name=yoneda.lib

:: --- Compiler flags -----------------------------------------------------------

set cl_common=cl /I %inc_dir% /nologo /std:c11 /EHsc
set cl_nolink=/c
set cl_debug=/D YO_DEBUG=1 /Z7 /Oy-
set cl_release=/D YO_DEBUG=0 /O2
set cl_lib=lib /NOLOGO /OUT:%out_name%

set clang_common=clang-cl -I %inc_dir% /std:c11 -fcolor-diagnostics -nobuiltininc /W3 -Wuninitialized -Wswitch -Wcovered-switch-default -Wshadow -Wpedantic -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wmisleading-indentation -Wformat=2 -Wno-unused -Wno-sometimes-uninitialized
set clang_nolink=-c
set clang_debug=-D YO_DEBUG=1 -g
set clang_release=-D YO_DEBUG=0 -g -O2
set clang_lib=llvm-ar rc %out_name%

:: --- Choosing the compiler ----------------------------------------------------

if "%msvc%"=="1" (
   set compile_debug=%cl_common% %cl_debug%
   set compile_nolink=%cl_nolink%
   set compile_release=%cl_common% %cl_release%
   set compile_lib=%cl_lib%
)

if "%clang%"=="1" (
   set compile_debug=%clang_common% %clang_debug%
   set compile_nolink=%clang_nolink%
   set compile_release=%clang_common% %clang_release%
   set compile_lib=%clang_lib%
)

if "%debug%"=="1"   set compile=%compile_debug%
if "%release%"=="1" set compile=%compile_release%

:: TODO: continue stuff

:: --- Prepare stuff ------------------------------------------------------------

if not exist build mkdir build

:: --- Compile all --------------------------------------------------------------

pushd build
%compile% %compile_nolink% %src_files%
%compile_lib% %obj_files%

%compile% ..\tests\test_memory.c /link %out_name%
popd

:: --- Unset everything ---------------------------------------------------------

for %%a in (%*) do set "%%a=0"
set msvc=
set clang=
set debug=
set release=
set compile=
set compile_debug=
set compile_release=
set compile_link=
set compile_out=
set out_exe=
