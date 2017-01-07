project(bmic)
cmake_minimum_required(VERSION 2.6)

include(CheckLists.cmake)
include(CMakeLists.default.cmake)

set(BUILD_WITH_UNIT_TESTS ON CACHE BOOL "Build unit tests")
set(BUILD_WITH_EXTRA_DEBUG OFF CACHE BOOL "Build extra debug information")

if (UNIX)
	message(STATUS "Adding Unix related build rules")
	include(CMakeLists.unix.cmake)
else (UNIX)
	if (WIN32)
		message(STATUS "Adding Windows related build rules")
		include(CMakeLists.windows.cmake)
	endif (WIN32)
endif(UNIX)

set(GRU_DIR ${DEFAULT_PREFIX} CACHE String "GRU install prefix")
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
