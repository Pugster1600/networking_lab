cmake_minimum_required(VERSION 3.28)
project(FT601)

# At the top of your CMakeLists.txt
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# C++ standards
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_STANDARD 17)
set (CMAKE_C_STANDARD_REQUIRED ON)

# add main.cpp as the main exexcutable for 
set(INC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib)

message(STATUS "Pointer size: ${CMAKE_SIZEOF_VOID_P}")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Building 64-bit executable")
else()
    message(STATUS "Building 32-bit executable")
endif()

if (WIN32)
# windows does not have "lib" prefix in front of dll
  message (STATUS "STATUS: Building Windows configuration")
  if(MINGW)
    message (STATUS "STATUS: Building MINGW config")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
  endif()

  link_directories(${LIB})  # where ftd3xx.lib is located
  add_executable(FT601 ${SRC}/main.c) 
  target_link_libraries(FT601 PRIVATE FTD3XXWU) #ft3
  target_compile_definitions(FT601 PRIVATE _WIN32 WIN32_LEAN_AND_MEAN) # macros basically
  target_include_directories(FT601 PRIVATE ${INC}) # THIS LINE MUST BE AT THE END FOR SOME REASON

  # Move .dll and .lib to the executable location
  add_custom_command(TARGET FT601 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB}/ftd3xxwu.dll"
        $<TARGET_FILE_DIR:FT601>      # Copies to executable directory
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB}/ftd3xxwu.lib"
        $<TARGET_FILE_DIR:FT601>
    COMMENT "Copying ftd3xxwu.dll and ftd3xxwu.lib to executable directory")
else ()
  message (STATUS "STATUS: Building Linux configuration")
  # INC/header directory
  link_directories(${LIB}) # tells linker where to find the library to linker against (NOTE: ORDER MATTERS HERE!!!) - this needs to be first
  add_executable(FT601 ${SRC}/main.c) #NOTE: include flags like def CPP if need be! (used in compiling)
  target_link_libraries(FT601 FTD3XXWU) #the actual file to look for in link_directories when building FT601, ftd3xx converted to libftd3xx.so when doing -lftd3xx flag
  target_compile_definitions(FT601 PRIVATE _LINUX)
  target_include_directories(FT601 PRIVATE ${INC}) #private means only used for the target, not target linking against another target
  
  set_target_properties(FT601 PROPERTIES
    BUILD_RPATH "${CMAKE_SOURCE_DIR}/lib"
  )
endif ()

