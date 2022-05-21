cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(PICO_SDK_PATH ~/opt/pico-sdk)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_BUILD_TYPE Debug)
set(LINKER_SCRIPT memmap_default.ld)
set(MY_TARGET firmware)


include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(${MY_TARGET}
   src/main.c
   src/rak4270.c
)

#Enable printing over USB and disable printing over UART
pico_enable_stdio_usb(${MY_TARGET} 1)
pico_enable_stdio_uart(${MY_TARGET} 0)

# Use custom linker script
# target_link_options(${MY_TARGET} PUBLIC -T ${LINKER_SCRIPT})
# target_link_options(${MY_TARGET} PUBLIC "LINKER:-T${LINKER_SCRIPT}") 
# set_target_properties(${MY_TARGET} PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT}")
# set_target_properties(${MY_TARGET} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})



# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${MY_TARGET} pico_stdlib hardware_gpio hardware_uart
hardware_adc)


target_include_directories(${MY_TARGET} PUBLIC lib)




# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${MY_TARGET})
