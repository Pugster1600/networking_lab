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

issue encountered:
device manager sees the device but the API says it is not connected
- this is usually something to do with the device configuration rather than the driver itselfq

sometimes you might have to load the firmwrae yourself first
super speed connection is mandatory 
- we know this because everything else is failing
- python and c that we wrote did not work
- the one that the manufactuer wrote did not work
- we know this is supposr to operate at super speed
- but in usbview it only shows high speed
- the documentation tells us that it only supports super speed

super important to note: 
the dll and the driver must match
- ie if the dll is ft3xxwu then the driver must also be the ft3xxwu driver
- if it is NOT the wu then the dll is ft3xx and the driver should also be ft3xx

------------------------------------------------------------------------------
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
  target_link_libraries(FT601 PRIVATE ftd3xx)
  target_compile_definitions(FT601 PRIVATE _WIN32 WIN32_LEAN_AND_MEAN) # macros basically
  target_include_directories(FT601 PRIVATE ${INC}) # THIS LINE MUST BE AT THE END FOR SOME REASON

  # Move .dll and .lib to the executable location
  add_custom_command(TARGET FT601 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB}/ftd3xx.dll"
        $<TARGET_FILE_DIR:FT601>      # Copies to executable directory
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB}/ftd3xx.lib"
        $<TARGET_FILE_DIR:FT601>
    COMMENT "Copying ftd3xx.dll and ftd3xx.lib to executable directory")
else ()
  message (STATUS "STATUS: Building Linux configuration")
  # INC/header directory
  link_directories(${LIB}) # tells linker where to find the library to linker against (NOTE: ORDER MATTERS HERE!!!) - this needs to be first
  add_executable(FT601 ${SRC}/main.c) #NOTE: include flags like def CPP if need be! (used in compiling)
  target_link_libraries(FT601 ftd3xx) #the actual file to look for in link_directories when building FT601, ftd3xx converted to libftd3xx.so when doing -lftd3xx flag
  target_compile_definitions(FT601 PRIVATE _LINUX)
  target_include_directories(FT601 PRIVATE ${INC}) #private means only used for the target, not target linking against another target
  
  set_target_properties(FT601 PROPERTIES
    BUILD_RPATH "${CMAKE_SOURCE_DIR}/lib"
  )
endif ()

as of 10/11/2025, NOTE: write the dates down to better remember
the wu driver is more recen tbut the 1.3 release actually works with the other files

- as per https://ftdichip.com/utilities/#ft60x-configuration
- the 1.4 version is more recent which uses the winusb drivers
- usb controller when 1.3
- usb device when 1.4
- also in the readme for the winusb installation

## Configuration
- the write configuration thing is talking to the chip to configure it in a certain way based on the things in the GUI
- such as the number of channels, clk speed and such


## xc7z020 FPGA
compare it to something that you know should work

possible failure points right now are the boot options

-----------------------------
PLL can muliply or divide frequencies so that we can use various different frequencies without different crystals
- this is used by both the tx and and rx

VP, VN are not connected
fpga_tck, fpga_tdp -> these are jtag? becuase not terminated but also not connected to anything -> yep they are jtag

xc7z010clg400-1

# porting an FPGA
## FPGA pins
- grouped into IO banks
- only a specific voltage standard like 1.8 LVCMOS
- only one voltage level at a time

-only some certian pins can feed into the global clock network
-analog pins
- configuration pins like jtag
- deicated TX and RX pins

normal IO pins are felxible though as long as voltage standard matches

Compare I/O availability (number and types of banks, voltage standards).

Map required I/Os from the old FPGA to the new one.

Identify missing or constrained signals — e.g. if UART2_TX had no suitable pin, you might need to drop or multiplex it.

Modify your top-level module to remove or reassign those ports.

Update the constraint file (.xdc/.ucf/.qsf) with the new pin assignments.

Regenerate bitstream and verify with the board schematic.

---------------------------
120 pins on the old board
80 pins on the new board
- BUT the total utilization is >80

so we know some of these are not going to work

https://download.amd.com/adaptive-socs-and-fpgas/developer/adaptive-socs-and-fpgas/package-pinout-files/z7packages/xc7z010clg400pkg.txt

we are using the alinx NOT xilinx one
- so its a board not the chip itself
- it has a bunch of extra features already on it

banks share electrical routes
- some banks are not available

try to have them in the same bank if possible


xilinx ip is pre-designed, pre-verified hardware block that performs a specific function — like a reusable component you drop into your design instead of coding it all yourself.

such as memory, math/logic ie the dsp blocks, 

fifo_generator_0 my_fifo (
    .clk(clk),
    .rst(rst),
    .din(data_in),
    .wr_en(wr_en),
    .rd_en(rd_en),
    .dout(data_out),
    .full(full),
    .empty(empty)
);

this is the instantiation

| **Name**                 | **Full Form / Meaning**               | **Used For**                                               | **Notes**                                                                                               |
| ------------------------ | ------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- |
| **MIO**                  | **Multiplexed I/O**                   | Processor-side general-purpose I/O (PS section in Zynq).   | Controlled by the ARM processor, not by PL logic. Used for UART, SPI, I²C, SD, etc.                     |
| **EMIO**                 | **Extended Multiplexed I/O**          | Processor I/O routed through programmable logic (PL).      | Lets PS peripherals access FPGA fabric pins.                                                            |
| **DDR / DDRx**           | **Double Data Rate memory interface** | Memory interface signals — address, data, control, clocks. | Uses **dedicated DDR banks** (HP or dedicated memory interface banks). Must follow layout/timing rules. |
| **HR**                   | **High-Range bank**                   | General I/O at up to 3.3 V.                                | Supports LVCMOS/LVTTL. Lower performance but higher voltage range.                                      |
| **HP**                   | **High-Performance bank**             | High-speed I/O at lower voltages (1.2–1.8 V).              | Used for DDR, LVDS, and high-bandwidth interfaces.                                                      |
| **PS**                   | **Processing System**                 | The ARM CPU subsystem (in Zynq, Zynq MPSoC).               | Separate from programmable logic (PL).                                                                  |
| **PL**                   | **Programmable Logic**                | The FPGA fabric (user logic).                              | Where your HDL/IP designs go.                                                                           |
| **GT / GTX / GTH / GTY** | **Gigabit Transceiver banks**         | High-speed serial links (e.g., PCIe, Ethernet, SATA).      | Dedicated differential TX/RX pairs, not normal GPIO.                                                    |
| **XADC**                 | **Xilinx ADC**                        | Analog-to-digital converter inputs.                        | Dedicated analog pins (VP/VN, VAUXP/VAUXN).                                                             |
| **VREF / VCCO**          | —                                     | Bank reference voltage and I/O supply voltage.             | Each bank has its own VCCO.                                                                             |
| **CONFIG Bank**          | —                                     | Used for JTAG, SPI, BPI configuration during power-up.     | Often Bank 0, not freely assignable until after boot.                                                   |
| **SYSMON**               | **System Monitor**                    | Internal temperature/voltage monitor analog pins.          | Shared with XADC inputs in some devices.                                                                |

always test a baseline out



when you remove a module, it still has that module driving the internal signals
- so when you remove it it says it is still driving it along with something else

so if you want to rmeove a port you can remove it then add an additional signal inside
- this way so that its the exact same just removing the port

------------------------------------
1. how do you start talking with the adc
- like it boots then what

how do you tell adc when to start collecting data and stuff

1. parallel mode or serial
- parallel mode is multiple lines vs serial is saying using spi to get the data

2. to start
- adc waits for an external trigger

3. end
- fpga gets an end of conversion or some rdy signal or soemthing

those serial pins can be configured as parallel or serial
- if serial they are spi configed
- if parallel, look at label 2

clock driven and we configure the timing

valid data presented on each ddr
- so use the adc clk on the fpga to capture the data

Provide ADC clock

Connect your FPGA or clock generation hardware to drive the ADC clock (ENC+ / ENC−) at the appropriate rate (e.g., 20 MHz for 20 MS/s or appropriate divider) per datasheet.

Ensure the ADC driver has good analog input front‑end (differential, proper termination, etc).

Configure ADC via SPI

Use the SPI port on the LTC2270 to set any mode (output format, range, power mode).

For example, you might select DDR LVDS or CMOS output.

Capture data in FPGA

Use the ADC clock (or its derivative) as a capture clock in your FPGA.

If DDR output: e.g., use ISERDESE2 (in Xilinx FPGAs) or equivalent to capture data on both edges.

Align data correctly: the ADC datasheet gives setup/hold specs.

Optional: determine frame/word boundaries

Each conversion produces a word (or two words for dual channel). You need to map which bits correspond to channel A, channel B.

The datasheet will show pin assignments and timing.

If the conversion is continuous, you might not have a separate “EOC” signal; you treat each clock as a valid sample event.

Buffer or process data

Once captured, you can store the data in RAM, process in your FPGA, send to PC, etc.

NOTE: need tsetup and thold on the fpga side so that we can propely sample

when does the output of the adc clck begin so that the fpga can sample it
- since the fpga uses an external clk to know when to sample
- find this information out

basically the adc is always sampling when it is on
- we just need to make sure the adc is in parallel mode 
1. parallel mode
2. adc continously gathers data and outputs it on the fifo bus
- the adc is supplying the differential clk signal

t_setup = minimum time the data must be stable before the clock edge the FPGA samples on

t_hold = minimum time the data must remain stable after the clock edge

t_delay / t_OD = propagation delay from analog input → digital output

So the FPGA should capture the ADC data:

FPGA sample edge
=
ADC data valid
≈
ADC CLK rising/falling edge
+
𝑡
delay
FPGA sample edge=ADC data valid≈ADC CLK rising/falling edge+t
delay
	​
FPGA typically uses DDR or edge-aligned sampling:

enc+ and enc-
- also ain+ nad ain-
- positive and negativ einput of the differential pair
- this is where they sample
- but we can also sample differnetial analog signals

AIN is true analog input
ENC is the connector that is connected to AIN


AIN = analog input
ENC = enable conversion
- looking at the schematic, we can kinda see that enc is part of the enabel circuit

we can look at the verilog code to see which clk is being used for adc

if you have a signal driving multiple different things
- understanding what you see form the oscillascope
- then the signal will get weakened
- we need to increase load voltage in order to actually see what is going on
- but since the clk signal is driving something else it is not exactly what is going on

differential pair constraint file and verilog
- though there could be a chip on the fpga that combines the two and feeds it to the fpga

check around pin 17 down
- make sure power is on
- also check if you need configuration/setup

---------------------
dc coupling shows both ac and dc signals
ac coupling only shows ac signals

$\omega = 2\pi f$

sampled at 255 with the pic
- now lets see if that is the case floating -> always test against a baseline case so ask yourself what is the baseline case
- sure enough it does see 255 
- so when we measure the miso line we see that the line is naturally high

ddr is double data rate

there are differnet voltage logic standards
- like LVDS, CMOS etc
- CMOS is noisy when sampling with the outside world but greater when just used in isolation like computer chips
- these care IO digital standards which include USB etc
- ie differential pairs and stuff

+/- pairs are usually lvds
- since cmos is usually not differential
- these differential pairs are just different electircal standards
- like lvds might have a different votlage standard from usb differential