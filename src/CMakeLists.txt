add_definitions(-D_GNU_SOURCE -D_POSIX_C_SOURCE)

if (${CMAKE_SYSTEM_NAME} EQUAL "Linux") 
 add_definitions(-DLINUX_BUILD) 
endif (${CMAKE_SYSTEM_NAME} EQUAL "Linux")

include_directories(.)

add_library(
	bmic SHARED
        base/common/bmic_credentials.c
        base/transport/bmic_endpoint.c   
        base/transport/bmic_endpoint_http.c        
        base/transport/bmic_transport.c
        base/format/bmic_json.c
        management/common/bmic_discovery_hint.c        
        management/common/bmic_product.c
        management/discovery/bmic_discovery.c
        product/artemis/bmic_artemis.c
        product/bmic_product_register.c
)

target_link_libraries(
	bmic
	gru
        curl
        json-c
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