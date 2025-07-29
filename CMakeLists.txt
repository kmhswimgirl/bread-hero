# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type") # Change from pico_w to pico

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(clone-hero-1 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(clone-hero-1 
    clone-hero-1.c 
    usb_descriptors.c
    bread-touch.c
)

pico_set_program_name(clone-hero-1 "clone-hero-1")
pico_set_program_version(clone-hero-1 "0.1")

# # Generate PIO header
# pico_generate_pio_header(clone-hero-1 ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(clone-hero-1 1)
pico_enable_stdio_usb(clone-hero-1 0)

# Add the standard library to the build
target_link_libraries(clone-hero-1
    pico_stdlib
    tinyusb_device
    tinyusb_board
)

# Add the standard include files to the build
target_include_directories(clone-hero-1 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/src
)

include_directories(${CMAKE_SOURCE_DIR}/pico-sdk/src/rp2_common/tinyusb/include)

# Add any user requested libraries
target_link_libraries(clone-hero-1 
        hardware_i2c
        hardware_pio
        )

pico_add_extra_outputs(clone-hero-1)