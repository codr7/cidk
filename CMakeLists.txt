cmake_minimum_required(VERSION 3.5)

project(cidk)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-std=c++17 -Wall -Werror -O2 -g)
add_link_options(-rdynamic)

file(GLOB_RECURSE src src/cidk/*.cpp)

add_library(lib_static STATIC ${src})
set_target_properties(lib_static PROPERTIES OUTPUT_NAME cidk)
target_include_directories(lib_static PUBLIC src/)
target_link_libraries(lib_static dl stdc++fs)

add_library(lib_shared SHARED ${src})
set_target_properties(lib_shared PROPERTIES OUTPUT_NAME cidk)
target_include_directories(lib_shared PUBLIC src/)

add_executable(cidk ${src} src/main.cpp)
target_include_directories(cidk PUBLIC src/)
target_link_libraries(cidk dl stdc++fs)

file(GLOB root_inc src/cidk/*.hpp)
install(FILES ${root_inc} DESTINATION include/cidk)

file(GLOB libs_inc src/cidk/libs/*.hpp)
install(FILES ${libs_inc} DESTINATION include/cidk/libs)

file(GLOB ops_inc src/cidk/ops/*.hpp)
install(FILES ${ops_inc} DESTINATION include/cidk/ops)

file(GLOB types_inc src/cidk/types/*.hpp)
install(FILES ${types_inc} DESTINATION include/cidk/types)

install(FILES build/cidk DESTINATION bin PERMISSIONS WORLD_EXECUTE)
install(FILES build/libcidk.a DESTINATION lib)
install(FILES build/libcidk.so DESTINATION lib)
