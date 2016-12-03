# In all project subdirectories
set(json-test-files simple/simple.json)

set(SHARED_DATA_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../build/target/tests/share)

# ... then traverse subdirectories
foreach(file ${json-test-files}) 
    message("Copying script file ${CMAKE_CURRENT_SOURCE_DIR}/base/format/${file} to runtime dir ${SHARED_DATA_OUTPUT_PATH}")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/base/format/${file} DESTINATION ${SHARED_DATA_OUTPUT_PATH})
endforeach(file)


