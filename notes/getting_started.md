## 1. FT600Q-FT601Q IC Device Manager and Driver installation
- we are using FT601Q
- driver installation: https://ftdichip.com/drivers/d3xx-drivers/ -> under windows x64
- it uses the D3XX driver
- install it, the go to device manager
- once you see the device, left click and update driver
- if you do not see it enumerate in device manager, something is up with the configuration or maybe faulty soldering or something
- it will show up as "FTDI..."

## 2. Getting windows to recgonize the driver
- .inf tells windows how to isntall the driver, which hardware it supports and where to find other needed files. ACts as the driver installation script
- .sys is the system driver file- the actual kernle mode driver that windows loads to interface with the hardware (the actual talker)
- .cat ensures .inf and sys have not been tampered with
- .dll - usermode helper library for applications that want to talk to the device via the FTDI's APIs (so real run time stuff for you other programs)
- .lib is used by developes to link c/c++ applications to the .dll
- If you're writing software to talk to the FT601: Your app loads ftd3xx.dll at runtime.
- so the driver is the kernel talking to the chip and the .dll/api is us talking to the chip
- the folder you are looking for is the .inf!!
- there is also an .exe "1.4.0.1 WHQL Certified. Available as a setup executable"
- this will do the process you are about to do but automatically 
- C:\Users\jshi4000\Downloads\Winusb_D3XX_Release_1.4.0.1\WU_FTD3XX_Driver is the folder you want to use
- This will have the .inf file in there
- once you are able to talk to the chip, we can use the GUI they provide to talk to the chip (the underlying thing is the dll)

## 3. API guides
- https://ftdichip.com/software-examples/ft600-601-software-examples-2/
- .lib is what you link against for the .dll to resolve headers and function references (either static or dynamic)
- .dll is the actual runtime dynamically loaded library 
- NOTE: if building in wsl, use the linux versions
- use WSL
- download the linux version
- move the .tar
- check installation guide.pdf
- THIS STUFF BELOW IS SYSTEMWIDE!!

 sudo rm /usr/lib/libftd3xx.so
 sudo cp libftd3xx.so /usr/lib/
 sudo cp libftd3xx.so* /usr/lib/
 sudo cp 51-ftd3xx.rules /etc/udev/rules.d/
 sudo udevadm control--reload-rules

 sudo ldconfig to update the inker cache
 .rules tells the kernel how to handle the ft601 usb device -> ensure user has access to ft601 when connected
 reload rules to update the .rules thing

the copy moves it to system library directory kinda like manually installing driver rather than using a package manager

gcc -I./include main.c -L./lib -lftd3xx -o ft601_app
LD_LIBRARY_PATH=./lib ./ft601_app

### about linking
in linux, .so is the dynamic library
- you need header file to resolve references at compile time
- the .so is the actual definition at runtime
- .a is static linking
- .so has the binary
- the .so.1.0.16 has the symbol resolution for the linking

in windows, .dll is basically .so
-.lib is static
- .lib is also a stub for linking to a .dll (so basically like starter info)
- .dll and .so both have compiled code that get loaded dyanmically lazily at run time
- .lib is linked to .exe BUT it tells the linker that the undefined code is resolved in the .dll
- .so does all of this for linux
/usr/bin/cc CMakeFiles/FT601.dir/src/main.c.o -o FT601   -L/mnt/c/Users/jshi4000/networking_lab/ft601/lib  -Wl,-rpath,/mnt/c/Users/jshi4000/networking_lab/ft601/lib -lftd3xx

SO
- we need to tell the compiler linker the include path, the dll path, the src path etc
- compiler processes each source file individually (definitions resolved at linking time)
- tell it the flags and header files

- then we link all the files we compiled including against the dll and header files and linker flags
- -c means compile, -o means object
- then we tell it the path of the dll -> -L..../lib
- and the dll name aka -lftd3xx

note: dont just blindy do it, ask what hte reason is and the reasons you are faililng

NOTE: git might not be able to add symbol files for linking (symbols resolved at link time BUT definitions are resolved at run time)
- function resolution, symbol resolution then definition resolution

CPP properties
- > c/c++: edit configurations (JSON)
- click the edit configurations (JSOn one)

user uses an api to ask the os to use the driver

make connections yourself to better understand
- that is what school does not teach you to do 
- aka systems level stuff connection sbetween related concepts

since vms are kinda like a subdirectory and can only see inside that subdirectoyr, if you do not forward another folder like say devices to it via like a mount or osmething then it will not be able to see it

-------------------------------------------------
# linux version
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

# INC/header directory
link_directories(${LIB}) # tells linker where to find the library to linker against (NOTE: ORDER MATTERS HERE!!!)
add_executable(FT601 ${SRC}/main.c) #NOTE: include flags like def CPP if need be! (used in compiling)
target_link_libraries(FT601 ftd3xx) #the actual file to look for in link_directories when building FT601, ftd3xx converted to libftd3xx.so when doing -lftd3xx flag
target_include_directories(FT601 PRIVATE ${INC}) #private means only used for the target, not target linking against another target

# Link it to your target
set_target_properties(FT601 PROPERTIES
  BUILD_RPATH "${CMAKE_SOURCE_DIR}/lib"
)

------------------------------------------------
## 4. About cmake and dynamic linking
1. MACROS
2. linux vs windows dynamic linking
- in linux, you can tell the kernel where to find the .so file
- this is done with the RPATH in cmake
- without rpath, cmake only looks for it in /lib and /usr/lib
- in windows, you cannot do that, so you can either put it in path or you must include the .dll and .lib in the same path as the .exe
- windows does not have a path for searching for .dll
- instead it looks for current working direcotry, PATH or system direcotires
- this is why .dll must be next to the .exe
3. target include directories must be the last thing for some reason