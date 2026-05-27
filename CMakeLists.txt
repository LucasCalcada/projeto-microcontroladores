cmake_minimum_required(VERSION 3.18)

set(PICO_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/pico-sdk)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

add_compile_options(
  -Wall
  -Wno-format
  -Wextra
  -O3
)

project(projSemestral C CXX ASM)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(projSemestral
  src/main.c
  src/motor.c
)

target_include_directories(projSemestral PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(projSemestral
  pico_stdlib
  hardware_pwm
  hardware_gpio
  hardware_uart
  hardware_irq
)

pico_enable_stdio_usb(projSemestral 1)
pico_enable_stdio_uart(projSemestral 1)
