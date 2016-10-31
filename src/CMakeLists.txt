add_definitions(-D_GNU_SOURCE -D_POSIX_C_SOURCE)

if (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 
 add_definitions(-DLINUX_BUILD) 
endif (${CMAKE_SYSTEM_NAME} EQUAL "Linux")

include_directories(.)

add_library(
	bmic SHARED
        transport/bmic_endpoint_http.c        
        transport/bmic_transport.c
)

target_link_libraries(
	bmic
	gru
        curl
)

install(TARGETS bmic
	LIBRARY DESTINATION ${CPP_LIBRARY_DIR}
)

install(DIRECTORY transport
        DESTINATION include/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h"
)

add_executable(
	bmic-client
        bmic/bmic.c
)

target_link_libraries(
	bmic-client
	bmic
)