cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_SYSTEM_NAME Generic)

project(Example C ASM)

include(../../cmake/stm32cube.cmake)

set(WARNINGS "-Wall -Wpedantic -Wextra")
set(CFLAGS "-O0 -g -LTO -std=c99 ${WARNINGS} ${STM32CUBE_DEFINITIONS} ${STM32CUBE_CFLAGS}")

add_definitions(-DHSE_VALUE=24000000)

set(CMAKE_C_FLAGS "${CFLAGS} ${CMAKE_C_FLAGS}")

set(SRC_FILES
	main.c
	interrupts.c

	BSP/devboard/devboard.c
)

add_executable(Example ${SRC_FILES})
target_include_directories(Example PRIVATE ${STM32CUBE_INCLUDE_DIRS})
target_link_libraries(Example ${STM32CUBE_LIBRARIES})

open_ocd_write_flash(Example)
