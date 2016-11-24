if (${CMAKE_SYSTEM_NAME} EQUAL "Linux")
 add_definitions(-D_GNU_SOURCE -DLINUX_BUILD)
endif (${CMAKE_SYSTEM_NAME} EQUAL "Linux")

include_directories(.)

add_library(
	bmic SHARED
        base/common/bmic_debug.c
        base/common/bmic_credentials.c
        base/common/bmic_object.c
        base/common/bmic_regex.c
        base/common/bmic_list.c
        base/common/bmic_cap_info.c
        base/common/bmic_op_arg.c
        base/common/bmic_op_sig.c
        base/common/bmic_op_info.c
        base/transport/bmic_endpoint.c
        base/transport/bmic_endpoint_http.c
        base/transport/bmic_transport.c
        base/format/bmic_json.c
        management/common/bmic_handle.c
        management/common/bmic_discovery_hint.c
        management/common/bmic_product.c
        management/discovery/bmic_discovery.c
        product/common/bmic_api_io.c
        product/common/bmic_path.c
        product/common/bmic_finder.c
        product/artemis/bmic_artemis_mi.c
        product/artemis/bmic_artemis.c
        product/activemq/bmic_activemq_mi.c
        product/activemq/bmic_activemq.c
        product/bmic_product_register.c
)

target_link_libraries(
	bmic
	gru
        curl
        ${LIBJSON}
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
        bmic/util/bmic_print_simple.c
        bmic/bmic_capabilities_main.c
        bmic/bmic_discovery_main.c
        bmic/bmic_queue_main.c
        bmic/bmic.c
)

target_link_libraries(
	bmic-client
	bmic
)
