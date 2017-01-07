project(bmic)
cmake_minimum_required(VERSION 2.6)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_definitions(-DLINUX_BUILD -D_GNU_SOURCE)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		message(STATUS "64 bits compiler detected")
		set(APP_BUILD_PLATFORM 64)
		set(APP_BUILD_PLATFORM_NAME "x86_64")
		set(CPP_LIBRARY_DIR "lib64")
	else (CMAKE_SIZEOF_VOID_P EQUAL 8)
		message(STATUS "32 bits compiler detected")
		set(APP_BUILD_PLATFORM 32)
		set(APP_BUILD_PLATFORM_NAME "i686")
		set(CPP_LIBRARY_DIR "lib")
	endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

	if(EXISTS "/etc/redhat-release")
			add_definitions(-D__RH_DISTRO__)
			set(LIBJSON json-c)
	endif(EXISTS "/etc/redhat-release")

	if(EXISTS "/etc/debian_version")
			add_definitions(-D__DEBIAN_DISTRO__)
			set(LIBJSON json)
	endif(EXISTS "/etc/debian_version")

	if(EXISTS "/etc/ubuntu-release")
			add_definitions(-D__UBUNTU_DISTRO__)
			set(LIBJSON json)
	endif(EXISTS "/etc/ubuntu-release")
else (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	message(STATUS "Compiling for " ${CMAKE_SYSTEM_NAME} "")
	if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
		set(CPP_LIBRARY_DIR "lib")
		set(CMAKE_MACOSX_RPATH TRUE)
		add_definitions(-D__OSX__)
		set(LIBJSON json-c)
	endif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/lib)
set (CTEST_BINARY_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin)

set(RUNTIME_DIR "bin")
set(CPP_INCLUDE_DIR "include")
set(SHARED_DIR "share")
set(BUILD_WITH_UNIT_TESTS ON CACHE BOOL "Build unit tests")
set(BUILD_WITH_EXTRA_DEBUG OFF CACHE BOOL "Build extra debug information")

#### GNU Compiler flags
if (CMAKE_COMPILER_IS_GNUCXX)
	if (BUILD_WITH_EXTRA_DEBUG)
			set(BMIC_DEBUG_DEFINES "-DBMIC_DEBUG=2")
	endif (BUILD_WITH_EXTRA_DEBUG)
	
	set(CMAKE_C_FLAGS "-Wall -Wshadow -Wconversion -std=c99 -fstrict-aliasing -fstack-protector-all -fstack-protector-strong ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during all build types." FORCE
	)

	set(CMAKE_C_FLAGS_DEBUG "-DNDEBUG ${BMIC_DEBUG_DEFINES} -fdiagnostics-color=auto -g ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during debug build." FORCE
	)

	set(CMAKE_C_FLAGS_RELEASE "-O2 -fomit-frame-pointer -D_FORTIFY_SOURCE=1 ${CMAKE_USER_C_FLAGS}" CACHE STRING
		"Flags used by the compiler during release build." FORCE
	)
endif (CMAKE_COMPILER_IS_GNUCXX)

macro (FailIfNotSet VARIABLE name)
	if (NOT ${VARIABLE})
		set(MSG "** A required file, " ${name} ", was not found on your system **")
		message(FATAL_ERROR ${MSG})
	endif (NOT ${VARIABLE})
endmacro(FailIfNotSet)

include (CheckIncludeFiles)
if (UNIX)
	CHECK_INCLUDE_FILES(stdlib.h HAVE_STDLIB_H)
	FailIfNotSet(HAVE_STDLIB_H stdlib.h)

	CHECK_INCLUDE_FILES(stdio.h HAVE_STDIO_H)
	FailIfNotSet(HAVE_STDIO_H stdio.h)

	CHECK_INCLUDE_FILES(stdarg.h HAVE_STDARG_H)
	FailIfNotSet(HAVE_STDARG_H stdarg.h)

	CHECK_INCLUDE_FILES(errno.h HAVE_ERRNO_H)
	FailIfNotSet(HAVE_ERRNO_H errno.h)

	CHECK_INCLUDE_FILES(string.h HAVE_STRING_H)
	FailIfNotSet(HAVE_STRING_H string.h)

	CHECK_INCLUDE_FILES(strings.h HAVE_STRINGS_H)
	FailIfNotSet(HAVE_STRINGS_H strings.h)

	CHECK_INCLUDE_FILES(stdbool.h HAVE_STDBOOL_H)
	FailIfNotSet(HAVE_STDBOOL_H stdbool.h)

	CHECK_INCLUDE_FILES(sys/time.h HAVE_SYS_TIME_H)
	FailIfNotSet(HAVE_SYS_TIME_H sys/time.h)

	CHECK_INCLUDE_FILES(sys/types.h HAVE_SYS_TYPES_H)
	FailIfNotSet(HAVE_SYS_TYPES_H sys/types.h)

	CHECK_INCLUDE_FILES(sys/stat.h HAVE_SYS_STAT_H)
	FailIfNotSet(HAVE_SYS_STAT_H sys/stat.h)

	CHECK_INCLUDE_FILES(inttypes.h HAVE_INT_TYPES_H)
	FailIfNotSet(HAVE_INT_TYPES_H inttypes.h)

	CHECK_INCLUDE_FILES(pthread.h HAVE_PTHREAD_H)
	FailIfNotSet(HAVE_PTHREAD_H pthread.h)

	CHECK_INCLUDE_FILES(fcntl.h HAVE_FCNTL_H)
	FailIfNotSet(HAVE_FCNTL_H fcntl.h)

	CHECK_INCLUDE_FILES(unistd.h HAVE_UNISTD_H)
	FailIfNotSet(HAVE_UNISTD_H unistd.h)

	CHECK_INCLUDE_FILES(ctype.h HAVE_CTYPE_H)
	FailIfNotSet(HAVE_CTYPE_H ctype.h)
endif (UNIX)

macro (DefineIfSet VARIABLE)
	if (${VARIABLE})
			set(MSG "Adding definition " ${VARIABLE} "")
			message(STATUS ${MSG})
			add_definitions(-D${VARIABLE})
	endif (${VARIABLE})
endmacro(DefineIfSet)

include(CheckFunctionExists)
if (UNIX)
	check_function_exists(strlcpy HAVE_STRLCPY)
	DefineIfSet(HAVE_STRLCPY)
endif (UNIX)


macro(SetLinkDirByPlatform PREFIX)
	if (${APP_BUILD_PLATFORM} EQUAL "64")
		link_directories(${PREFIX}/lib64)
	else(${APP_BUILD_PLATFORM} EQUAL "64")
		link_directories(${PREFIX}/lib)
	endif(${APP_BUILD_PLATFORM} EQUAL "64")
endmacro(SetLinkDirByPlatform PREFIX)


set(GRU_DIR /usr CACHE String "GRU install prefix")
add_definitions(-D__GRU_SUPPORT__)
include_directories(${GRU_DIR}/include/gru)
SetLinkDirByPlatform(${GRU_DIR})

include_directories(src)
add_subdirectory(src/ objs)

if (BUILD_WITH_UNIT_TESTS)
	enable_testing()

	include_directories(test)
	add_subdirectory(test)
endif (BUILD_WITH_UNIT_TESTS)

add_custom_target(format
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/util/format.sh
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

