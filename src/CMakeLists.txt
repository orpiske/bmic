if (${CMAKE_SYSTEM_NAME} EQUAL "Linux")
 add_definitions(-D_GNU_SOURCE -DLINUX_BUILD)
endif (${CMAKE_SYSTEM_NAME} EQUAL "Linux")

include_directories(.)

add_library(
	bmic-base SHARED
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
)

target_link_libraries(
	bmic-base
	gru
        curl
        ${LIBJSON}
)

add_library(
	bmic-management SHARED
        management/common/bmic_handle.c
        management/common/bmic_discovery_hint.c
        management/common/bmic_product.c
        management/common/bmic_api_io.c
        management/discovery/bmic_discovery.c
        management/interfaces/jolokia/bmic_jolokia.c
        management/interfaces/jolokia/bmic_jolokia_io.c
        management/interfaces/jolokia/bmic_jolokia_parse.c
)

target_link_libraries(
	bmic-management
        bmic-base
	gru
        curl
        ${LIBJSON}
)

add_library(
	bmic-product SHARED
        product/common/bmic_path.c
        product/common/bmic_finder.c
        product/artemis/bmic_artemis_mi.c
        product/artemis/bmic_artemis_json.c
        product/artemis/bmic_artemis.c
        product/activemq/bmic_activemq_mi.c
        product/activemq/bmic_activemq_json.c
        product/activemq/bmic_activemq.c
        product/complements/java/bmic_java.c
        product/bmic_product_register.c
        context/bmic_context.c
)

target_link_libraries(
	bmic-product
        bmic-base
        bmic-management
	gru
        curl
        ${LIBJSON}
)

install(TARGETS bmic-base bmic-management bmic-product 
	LIBRARY DESTINATION ${CPP_LIBRARY_DIR}
)

install(DIRECTORY transport
        DESTINATION include/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h"
)

add_executable(
	bmic
        bmic/util/bmic_print_simple.c
        bmic/util/bmic_type_map.c
        bmic/bmic_capabilities_main.c
        bmic/bmic_discovery_main.c
        bmic/bmic_queue_main.c
        bmic/bmic_operations_main.c
        bmic/bmic_top_main.c
        bmic/bmic.c
)

target_link_libraries(
	bmic
	bmic-product
        bmic-base
        bmic-management
)
