project(bmic)
cmake_minimum_required(VERSION 2.6)

set(BMIC_VERSION_MAJOR 0)
set(BMIC_VERSION_MINOR 0)
set(BMIC_VERSION_PATCH 1)
set(BMIC_VERSION ${BMIC_VERSION_MAJOR}.${BMIC_VERSION_MINOR}.${BMIC_VERSION_PATCH})

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}" -DBMIC_VERSION="${BMIC_VERSION}")

include(GNUInstallDirs)
include(CheckLists.cmake)
include(CMakeLists.default.cmake)

set(BUILD_WITH_UNIT_TESTS ON CACHE BOOL "Build unit tests")
set(BUILD_WITH_EXTRA_DEBUG OFF CACHE BOOL "Build extra debug information")
set(BUILD_WITH_DOCUMENTATION OFF CACHE BOOL "Build documentation")
set(GRU_DIR "/usr" CACHE STRING "GRU directory")

if (UNIX)
	message(STATUS "Adding Unix related build rules")
	include(CMakeLists.unix.cmake)
else (UNIX)
	if (WIN32)
		message(STATUS "Adding Windows related build rules")
		include(CMakeLists.windows.cmake)
	endif (WIN32)
endif(UNIX)

include_directories(${GRU_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${CURL_INCLUDE_DIR})
link_libraries(${GRU_LIB})

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

if (BUILD_WITH_DOCUMENTATION)
	add_custom_target(documentation
		COMMAND ${CMAKE_COMMAND} -P ../documentation.cmake)
	install(
		DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/target/share/doc 
		DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}-${BMIC_VERSION_MAJOR}
	)
endif(BUILD_WITH_DOCUMENTATION)
