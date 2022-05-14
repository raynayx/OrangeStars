cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(PICO_SDK_PATH ~/opt/pico-sdk)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_BUILD_TYPE Debug)


include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(firmware
   src/main.c
   src/rak4270.c
)

#Enable printing over USB and disable printing over UART
pico_enable_stdio_usb(firmware 1)
pico_enable_stdio_uart(firmware 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(firmware pico_stdlib hardware_gpio hardware_uart)


target_include_directories(firmware PUBLIC lib)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(firmware)
