# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

set(PICO_EXTRAS_PATH ${USERHOME}/.pico-sdk/pico-extras-sdk-1.5.1)
include(${USERHOME}/.pico-sdk/pico-extras-sdk-1.5.1/external/pico_extras_import.cmake)

project(ICHack C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(include)
# Add executable. Default name is the project name, version 0.1

add_executable(ICHack 
    src/ICHack.cpp
    src/credits.cpp
    src/debug.cpp
    src/quotes.cpp
    src/logo_patterns.cpp
    src/letters.cpp
    src/render.cpp
    src/input.cpp
    )

add_compile_definitions(DEBUG=0)
pico_set_program_name(ICHack "ICHack")
pico_set_program_version(ICHack "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(ICHack 0)
pico_enable_stdio_usb(ICHack 0)

# Add the standard library to the build
target_link_libraries(ICHack
        pico_stdlib
        )

# Add the standard include files to the build
target_include_directories(ICHack PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(ICHack 
        hardware_pwm
        pico_runtime
        hardware_sleep
        hardware_rtc
        pico_rand
        )

pico_add_extra_outputs(ICHack)

