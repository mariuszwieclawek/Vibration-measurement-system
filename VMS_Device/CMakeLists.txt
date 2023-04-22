# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(Vibration_measurement_system)


# FILE(GLOB app_sources src/*.c)

# target_sources(app PRIVATE ${app_sources})
# target_include_directories(app PRIVATE inc)

target_include_directories(app PRIVATE app)
target_include_directories(app PRIVATE app/bluetooth/inc)
target_include_directories(app PRIVATE app/led/inc)
target_include_directories(app PRIVATE app/button/inc)
target_include_directories(app PRIVATE app/accelerometer/inc)
target_include_directories(app PRIVATE app/pwm/inc)

target_sources(app PRIVATE app/main.c)
target_sources(app PRIVATE app/bluetooth/src/bluetooth.c)
target_sources(app PRIVATE app/led/src/led.c)
target_sources(app PRIVATE app/button/src/button.c)
target_sources(app PRIVATE app/accelerometer/src/acc.c)
target_sources(app PRIVATE app/pwm/src/pwm.c)

zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})