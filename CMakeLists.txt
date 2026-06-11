cmake_minimum_required(VERSION 3.12)
project(TwixtorSpeed)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========== ADOBE SDK PATH ==========
set(ADOBE_SDK_PATH "C:/Adobe/AE_SDK")

# ========== INCLUDE DIRECTORIES ==========
include_directories(${ADOBE_SDK_PATH}/Headers)
include_directories(${ADOBE_SDK_PATH}/Headers/AEConfig)

# ========== SOURCE FILES ==========
set(SOURCES
    src/TwixtorSpeed.cpp
)

# ========== BUILD PLUGIN ==========
add_library(TwixtorSpeed SHARED ${SOURCES})

# Set output properties
set_target_properties(TwixtorSpeed PROPERTIES
    SUFFIX ".aex"
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output"
)

# ========== WINDOWS SETTINGS ==========
if(MSVC)
    set(ADOBE_LIB_PATH "${ADOBE_SDK_PATH}/Libraries/Win/Release")
    
    target_link_directories(TwixtorSpeed PRIVATE ${ADOBE_LIB_PATH})
    target_link_libraries(TwixtorSpeed
        AE_Effect_Unicode.lib
        AE_UtilLib.lib
    )
    
    target_compile_options(TwixtorSpeed PRIVATE /W4)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# ========== MACOS SETTINGS ==========
if(APPLE)
    set(ADOBE_LIB_PATH "${ADOBE_SDK_PATH}/Libraries/Mac/Release")
    
    target_link_directories(TwixtorSpeed PRIVATE ${ADOBE_LIB_PATH})
    target_link_libraries(TwixtorSpeed
        AE_Effect
        AE_Util
    )
    
    set_target_properties(TwixtorSpeed PROPERTIES
        MACOSX_BUNDLE FALSE
    )
endif()

# ========== LINUX SETTINGS ==========
if(UNIX AND NOT APPLE)
    set(ADOBE_LIB_PATH "${ADOBE_SDK_PATH}/Libraries/Linux/Release")
    
    target_link_directories(TwixtorSpeed PRIVATE ${ADOBE_LIB_PATH})
    target_link_libraries(TwixtorSpeed
        AE_Effect
        AE_Util
    )
endif()

message(STATUS "")
message(STATUS "=== Twixtor Speed Plugin ===")
message(STATUS "Adobe SDK: ${ADOBE_SDK_PATH}")
message(STATUS "Output: ${CMAKE_BINARY_DIR}/output/TwixtorSpeed.aex")
message(STATUS "")
