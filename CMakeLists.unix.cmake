if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_definitions(-DLINUX_BUILD -D_GNU_SOURCE)
else (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	message(STATUS "Compiling for " ${CMAKE_SYSTEM_NAME} "")
	set(CPP_LIBRARY_DIR "lib")

	if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
		set(CMAKE_MACOSX_RPATH TRUE)
		add_definitions(-D__OSX__)
	endif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/${CMAKE_INSTALL_BINDIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/${CMAKE_INSTALL_LIBDIR})
set (CTEST_BINARY_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/${CMAKE_INSTALL_BINDIR})

find_path(JSON_INCLUDE_DIR json.h
          PATH_SUFFIXES json json-c)

find_library(JSON_LIB NAMES json json-c)
link_libraries(${JSON_LIB})

message(STATUS "JSON headers found at ${JSON_INCLUDE_DIR}")
message(STATUS "JSON library found at ${JSON_LIB}")

find_path(GRU_INCLUDE_DIR common/gru_base.h
        PATH_SUFFIXES gru-0
	HINTS ${GRU_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
find_library(GRU_LIB NAMES gru-0
	HINTS ${GRU_DIR}/${CMAKE_INSTALL_LIBDIR})


message(STATUS "GRU headers found at ${GRU_INCLUDE_DIR}")
message(STATUS "GRU library found at ${GRU_LIB}")

find_path(CURL_INCLUDE_DIR curl/curl.h)
find_library(CURL_LIB NAMES curl)

message(STATUS "CURL headers found at ${CURL_INCLUDE_DIR}")
message(STATUS "CURL library found at ${CURL_LIB}")
