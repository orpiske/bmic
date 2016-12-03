PROJECT(bmic-tests)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/target/tests/bin)

add_custom_target(test-files ALL
   COMMAND ${CMAKE_COMMAND} -P test-files.cmake
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(bmic_json_simple_test
	base/format/bmic_json_simple_test.c
)

target_link_libraries(bmic_json_simple_test
           bmic-base)

add_test(NAME bmic-json-simple-test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND bmic_json_simple_test 
        ${CMAKE_BINARY_DIR}/target/tests/share/simple.json)


add_executable(bmic_artemis_test
	product/artemis/bmic_artemis_test.c
)

target_link_libraries(bmic_artemis_test
           bmic-base bmic-management bmic-product)
