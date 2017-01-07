if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_definitions(-DLINUX_BUILD -D_GNU_SOURCE)
	set(LIBJSON json-c)

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
	set(CPP_LIBRARY_DIR "lib")
	set(LIBJSON json-c)

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
		set(CMAKE_MACOSX_RPATH TRUE)
		add_definitions(-D__OSX__)
	endif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/lib)
set (CTEST_BINARY_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin)
