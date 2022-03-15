cmake_minimum_required(VERSION 3.0.0)
project(audio_avb VERSION 0.1.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

set(ICC_Generared_Source "./lbs/src_gen/")

add_custom_command(
  COMMENT "Generate ICC code"
  OUTPUT ${ICC_Generared_Source}audio.h ${ICC_Generared_Source}audio.cpp ${ICC_Generared_Source}test.h ${ICC_Generared_Source}test.cpp
  COMMAND /bin/bash ./gradleDocker.sh --image=icc_generator --run="\"python3 ./generate_lbs.py\""
  WORKING_DIRECTORY .
  VERBATIM
)

include(CTest)
enable_testing()

include_directories(${ICC_Generared_Source}/ ./cpp)
include_directories(${ICC_Generared_Source}/ ./lbs/src)

# iccserializer library
project(iccserializerlib VERSION 1.0.1 DESCRIPTION "serializes and deserializes ICC messages")
file(GLOB iccserializerlib_SRC
    CONFIGURE_DEPENDS
    ./lbs/src/serializer.cpp
    ${ICC_Generared_Source}*.cpp
    ${ICC_Generared_Source}*.h
)
add_library(iccserializerlib SHARED ${iccserializerlib_SRC})

target_include_directories(iccserializerlib PRIVATE .)

# executables
add_executable(audio_avb ./cpp/main_audio.cpp)
target_link_libraries(audio_avb iccserializerlib)

add_executable(test_avb ./cpp/main_test.cpp)
target_link_libraries(test_avb iccserializerlib)

add_executable(test_avb_audio ./cpp/utest_audio.cpp)
target_link_libraries(test_avb_audio gtest gtest_main pthread iccserializerlib)

add_executable(test_avb_test ./cpp/utest_test.cpp)
target_link_libraries(test_avb_test gtest gtest_main pthread iccserializerlib)

add_executable(test_avb_trace ./cpp/utest_trace.cpp)
target_link_libraries(test_avb_trace gtest gtest_main pthread iccserializerlib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
