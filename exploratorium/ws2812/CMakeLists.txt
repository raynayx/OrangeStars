cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(PICO_SDK_PATH ~/opt/pico-sdk)
set(PICO_BOARD seeed_xiao_rp2040)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_BUILD_TYPE Debug)
set(MY_TARGET firmware)

include(pico_sdk_import.cmake)
project(my_project)


# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(${MY_TARGET}
        src/ws2812.c
        )

# generate the header file into the source tree as it is included in the RP2040 datasheet
# pico_generate_pio_header(pio_ws2812 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

#Enable printing over USB and disable printing over UART
pico_enable_stdio_usb(${MY_TARGET} 1) 
pico_enable_stdio_uart(${MY_TARGET} 0)

target_link_libraries(${MY_TARGET} pico_stdlib hardware_pio)
target_include_directories(${MY_TARGET} PUBLIC lib)


pico_add_extra_outputs(${MY_TARGET})

# # add url via pico_set_program_url
# example_auto_set_url(pio_ws2812)

# add_executable(pio_ws2812_parallel)

# pico_generate_pio_header(pio_ws2812_parallel ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# target_sources(pio_ws2812_parallel PRIVATE ws2812_parallel.c)

# target_compile_definitions(pio_ws2812_parallel PRIVATE
#         PIN_DBG1=3)

# target_link_libraries(pio_ws2812_parallel PRIVATE pico_stdlib hardware_pio hardware_dma)
# pico_add_extra_outputs(pio_ws2812_parallel)

# # add url via pico_set_program_url
# example_auto_set_url(pio_ws2812_parallel)

# # Additionally generate python and hex pioasm outputs for inclusion in the RP2040 datasheet
# add_custom_target(pio_ws2812_datasheet DEPENDS ${CMAKE_CURRENT_LIST_DIR}/generated/ws2812.py)
# add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/ws2812.py
#         DEPENDS ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio
#         COMMAND Pioasm -o python ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio ${CMAKE_CURRENT_LIST_DIR}/generated/ws2812.py
#         )
# add_dependencies(pio_ws2812 pio_ws2812_datasheet)