ocv_clear_vars(HAVE_WEBNN)
ocv_clear_vars(WEBNN_EMSDK)
if(WITH_WEBNN)
  set(WEBNN_HEADER_DIRS "$ENV{WEBNN_NATIVE_DIR}/gen/src/include")
  set(WEBNN_INCLUDE_DIRS "$ENV{WEBNN_NATIVE_DIR}/../../src/include")
  set(WEBNN_LIBRARIES "$ENV{WEBNN_NATIVE_DIR}/libwebnn_native.so")
endif()

try_compile(VALID_WEBNN
      "$ENV{WEBNN_NATIVE_DIR}"
      "$ENV{WEBNN_NATIVE_DIR}/gen/src/webnn/webnn_cpp.cpp"
      CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:STRING=${WEBNN_INCLUDE_DIRS}\;${WEBNN_HEADER_DIRS}"
                  "-DLINK_LIBRARIES:STRING=${WEBNN_LIBRARIES}"
      OUTPUT_VARIABLE TRY_OUT
      )

try_compile(VALID_WEBNN
      "${OpenCV_BINARY_DIR}"
      "${OpenCV_SOURCE_DIR}/cmake/checks/webnn.cpp"
      CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:STRING=${WEBNN_INCLUDE_DIRS}\;${WEBNN_HEADER_DIRS}"
                  "-DLINK_LIBRARIES:STRING=${WEBNN_LIBRARIES}"
      OUTPUT_VARIABLE TRY_OUT
      )
if(NOT ${VALID_WEBNN})
  message(WARNING "Can't use WebNN-native")
  return()
endif()
message(AUTHOR_WARNING "Use WebNN-native")

set(HAVE_WEBNN 1)
