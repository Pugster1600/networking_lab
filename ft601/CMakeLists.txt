cmake_minimum_required(VERSION 3.28)
project(FT601)

# C++ standards
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# add main.cpp as the main exexcutable for 
set(INC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# INC/header directory
link_directories(${LIB}) # tells linker where to find the library to linker against (NOTE: ORDER MATTERS HERE!!!)
add_executable(FT601 ${SRC}/main.c) #NOTE: include flags like def CPP if need be! (used in compiling)
target_link_libraries(FT601 ftd3xx) #the actual file to look for in link_directories when building FT601, ftd3xx converted to libftd3xx.so when doing -lftd3xx flag
target_include_directories(FT601 PRIVATE ${INC}) #private means only used for the target, not target linking against another target

# Link it to your target
set_target_properties(FT601 PROPERTIES
  BUILD_RPATH "${CMAKE_SOURCE_DIR}/lib"
)

