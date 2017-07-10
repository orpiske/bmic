# Simple files
set(json-simple-test-files simple/simple.json)

set(SHARED_DATA_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../build/target/tests/share)

# ... then traverse subdirectories
foreach(file ${json-simple-test-files})
	message("Copying test data ${CMAKE_CURRENT_SOURCE_DIR}/base/format/${file} to ${SHARED_DATA_OUTPUT_PATH}")
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/base/format/${file} DESTINATION ${SHARED_DATA_OUTPUT_PATH})
endforeach(file)


# Artemis files
set(
	json-artemis-files
	artemis/artemis-v200.json
)

set(SHARED_DATA_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../build/target/tests/share)

# ... then traverse subdirectories
foreach(file ${json-artemis-files})
	message("Copying test data ${CMAKE_CURRENT_SOURCE_DIR}/base/format/${file} to ${SHARED_DATA_OUTPUT_PATH}")
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/base/format/${file} DESTINATION ${SHARED_DATA_OUTPUT_PATH})
endforeach(file)


# ActiveMQ files
set(json-activemq-files
	activemq/activemq-v5.14.1.json
)

set(SHARED_DATA_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../build/target/tests/share)

# ... then traverse subdirectories
foreach(file ${json-activemq-files})
	message("Copying test data ${CMAKE_CURRENT_SOURCE_DIR}/base/format/${file} to ${SHARED_DATA_OUTPUT_PATH}")
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/base/format/${file} DESTINATION ${SHARED_DATA_OUTPUT_PATH})
endforeach(file)
