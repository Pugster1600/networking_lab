cmake_minimum_required(VERSION 3.28)
project(FT601)

# C++ standards
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_STANDARD 17)
set (CMAKE_C_STANDARD_REQUIRED ON)

# add main.cpp as the main exexcutable for 
set(INC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if (WIN32)
# windows does not have "lib" prefix in front of dll
  message (STATUS "STATUS: Building Windows configuration")
  link_directories(${LIB})  # where ftd3xx.lib is located
  add_executable(FT601 ${SRC}/main.c) 
  target_link_libraries(FT601 PRIVATE ftd3xx)
  target_compile_definitions(FT601 PRIVATE _WIN32 WIN32_LEAN_AND_MEAN) # macros basically
  target_include_directories(FT601 PRIVATE ${INC}) # THIS LINE MUST BE AT THE END FOR SOME REASON
  # windows does not have a path for searching for .dll
  # instead it looks for current working direcotry, PATH or system direcotires
  # this is why .dll must be next to the .exe
  # cmake -B build -G "Ninja"
  # cmake -G "MinGW Makefiles" .. -DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake
  # can also use windows C compiler
  # cmake -G "Ninja" .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ 
  # specifies the toolchain to use

else ()
  message (STATUS "STATUS: Building Linux configuration")
  # INC/header directory
  link_directories(${LIB}) # tells linker where to find the library to linker against (NOTE: ORDER MATTERS HERE!!!) - this needs to be first
  add_executable(FT601 ${SRC}/main.c) #NOTE: include flags like def CPP if need be! (used in compiling)
  target_link_libraries(FT601 ftd3xx) #the actual file to look for in link_directories when building FT601, ftd3xx converted to libftd3xx.so when doing -lftd3xx flag
  target_compile_definitions(FT601 PRIVATE _LINUX)
  target_include_directories(FT601 PRIVATE ${INC}) #private means only used for the target, not target linking against another target
  
  # Link it to your target - runtime library search path -> where to find the .so at runtime
  # readelf -d MyApp | grep RPATH
  # without rpath, cmake only looks for it in /lib and /usr/lib
  # so rpath makes it more isolated
  # cmake ..
  set_target_properties(FT601 PROPERTIES
    BUILD_RPATH "${CMAKE_SOURCE_DIR}/lib"
  )
endif ()

