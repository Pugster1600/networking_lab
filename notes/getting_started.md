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

