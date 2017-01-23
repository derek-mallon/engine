cmake_minimum_required(VERSION 2.8)
project(engine)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(ENGINE_SOURCES ../src/asset.c ../src/component.c ../src/engine.c ../src/file_handling.c ../src/geometry.c ../src/io.c ../src/mem.c ../src/module_startup_shutdown.c ../src/physics.c ../src/render.c ../src/sdl_wrapper.c ../src/term.c ../src/utils.c)

include_directories("${PROJECT_SOURCE_DIR}/../include" ${SDL2_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

add_library(engine ${ENGINE_SOURCES})

target_link_libraries(engine ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})



set(PROJECT_CREATOR_SOURCES ../src/main.c ../src/proj_conf.c)

include_directories("${PROJECT_SOURCE_DIR}/../include")

add_executable(project_creator ${PROJECT_CREATOR_SOURCES})

target_link_libraries(project_creator engine)

#Unit Testing

enable_testing()

include_directories("${PROJECT_SOURCE_DIR}/../include")

add_executable(mem_tests ../tests/mem_tests.c)

target_link_libraries(mem_tests engine)

add_test(MEM_TESTS ../bin/mem_tests)

include_directories("${PROJECT_SOURCE_DIR}/../include")

add_executable(io_tests ../tests/io_tests.c)

target_link_libraries(io_tests engine)

add_test(IO_TESTS ../bin/io_tests)

include_directories("${PROJECT_SOURCE_DIR}/../include")

add_executable(sdl_wrapper_tests ../tests/sdl_wrapper_tests.c)

target_link_libraries(sdl_wrapper_tests engine)

add_test(SDL_WRAPPER_TESTS ../bin/sdl_wrapper_tests)





