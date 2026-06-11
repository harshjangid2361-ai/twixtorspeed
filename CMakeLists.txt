cmake_minimum_required(VERSION 3.12)
project(TwixtorSpeed)

set(CMAKE_CXX_STANDARD 17)

set(ADOBE_SDK_PATH "C:/Users/Harshitjangid/Downloads/AfterEffectsSDK_25.6_61_win")

include_directories(${ADOBE_SDK_PATH}/Headers)

add_library(TwixtorSpeed SHARED src/TwixtorSpeed.cpp)

set_target_properties(TwixtorSpeed PROPERTIES
    SUFFIX ".aex"
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output"
)

if(MSVC)
    target_link_directories(TwixtorSpeed PRIVATE 
        "${ADOBE_SDK_PATH}/Libraries/Win/Release")
    target_link_libraries(TwixtorSpeed
        AE_Effect_Unicode.lib
        AE_UtilLib.lib)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

message(STATUS "Adobe SDK: ${ADOBE_SDK_PATH}")
message(STATUS "Output: ${CMAKE_BINARY_DIR}/output/TwixtorSpeed.aex")
