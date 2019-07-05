cmake_minimum_required(VERSION 3.0)

project(cidk)

set(CMAKE_CXX_COMPILER g++)

add_compile_options(-std=c++17 -Wall -Werror -O2 -g)

file(GLOB_RECURSE src src/cidk/*.cpp)

add_library(lib_static STATIC ${src})
set_target_properties(lib_static PROPERTIES OUTPUT_NAME cidk)
target_include_directories(lib_static PUBLIC src/)

add_executable(cidk ${src} src/main.cpp)
target_include_directories(cidk PUBLIC src/)

file(GLOB inc src/cidk/*.hpp)
install(FILES ${inc} DESTINATION include/cidk)

install(FILES build/cidk DESTINATION bin PERMISSIONS WORLD_EXECUTE)
install(FILES build/libcidk.a DESTINATION lib)
