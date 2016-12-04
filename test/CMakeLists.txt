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
           bmic-base bmic-management bmic-product)

add_test(NAME bmic-json-simple-test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND bmic_json_simple_test 
        ${CMAKE_BINARY_DIR}/target/tests/share/simple.json)


add_executable(bmic_artemis_test
	product/artemis/bmic_artemis_test.c
)

target_link_libraries(bmic_artemis_test
           bmic-base bmic-management bmic-product)


add_test(NAME bmic-artemis-cap-test-130
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND bmic_artemis_test "1.3.0"
        ${CMAKE_BINARY_DIR}/target/tests/share/artemis-v130.json)

add_test(NAME bmic-artemis-cap-test-140
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND bmic_artemis_test "1.4.0"
        ${CMAKE_BINARY_DIR}/target/tests/share/artemis-v140.json)

add_test(NAME bmic-artemis-cap-test-150
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/target/tests/bin
    COMMAND bmic_artemis_test "1.5.0"
        ${CMAKE_BINARY_DIR}/target/tests/share/artemis-v150.json)