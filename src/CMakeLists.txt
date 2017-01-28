include_directories(.)

add_library(
	bmic-base-${BMIC_VERSION_MAJOR} SHARED
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
	bmic-base-${BMIC_VERSION_MAJOR}
	${GRU_LIB}
	${CURL_LIB}
	${JSON_LIB}
)

install(
	DIRECTORY base/common base/format base/transport
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${BMIC_VERSION_MAJOR}/base
	FILES_MATCHING PATTERN "*.h"
)

add_library(
	bmic-management-${BMIC_VERSION_MAJOR} SHARED
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
	bmic-management-${BMIC_VERSION_MAJOR}
	bmic-base-${BMIC_VERSION_MAJOR}
	${GRU_LIB}
)

install(
	DIRECTORY management/common management/discovery
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${BMIC_VERSION_MAJOR}/management
	FILES_MATCHING PATTERN "*.h"
)

install(
	DIRECTORY management/interfaces/jolokia
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${BMIC_VERSION_MAJOR}/management/interfaces
	FILES_MATCHING PATTERN "*.h"
)

add_library(
	bmic-product-${BMIC_VERSION_MAJOR} SHARED
	product/common/bmic_path.c
	product/common/bmic_finder.c
	product/artemis/bmic_artemis_mi.c
	product/artemis/bmic_artemis_json.c
	product/artemis/bmic_artemis.c
	product/activemq/bmic_activemq_mi.c
	product/activemq/bmic_activemq_json.c
	product/activemq/bmic_activemq.c
	product/activemq/bmic_jamq6.c
	product/complements/java/bmic_java.c
	product/bmic_product_register.c
	context/bmic_context.c
)

target_link_libraries(
	bmic-product-${BMIC_VERSION_MAJOR}
	bmic-base-${BMIC_VERSION_MAJOR}
	bmic-management-${BMIC_VERSION_MAJOR}
	${GRU_LIB}
)

install(
	DIRECTORY product/common product/activemq product/artemis product/complements/java
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${BMIC_VERSION_MAJOR}/product
	FILES_MATCHING PATTERN "*.h"
)

install(
	DIRECTORY context product
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${BMIC_VERSION_MAJOR}
	FILES_MATCHING PATTERN "*.h"
)

install(
	TARGETS bmic-base-${BMIC_VERSION_MAJOR} bmic-management-${BMIC_VERSION_MAJOR} bmic-product-${BMIC_VERSION_MAJOR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_executable(
	bmic
	bmic/util/bmic_print_simple.c
	bmic/util/bmic_type_map.c
	bmic/util/bmic_opt_common.c
	bmic/util/bmic_action_common.c
	bmic/bmic_capabilities_main.c
	bmic/bmic_discovery_main.c
	bmic/bmic_queue_main.c
	bmic/bmic_topic_main.c
	bmic/bmic_operations_main.c
	bmic/bmic_top_main.c
	bmic/bmic.c
)

target_link_libraries(
	bmic
	bmic-product-${BMIC_VERSION_MAJOR}
	bmic-base-${BMIC_VERSION_MAJOR}
	bmic-management-${BMIC_VERSION_MAJOR}
)

install(
	TARGETS bmic
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
