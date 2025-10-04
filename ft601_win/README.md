## 0. Prereqs
```
WSL
FT601 driver - https://ftdichip.com/drivers/d3xx-drivers/ (windows for driver)
FT601 dynamic library - https://ftdichip.com/drivers/d3xx-drivers/ (windows for dynamic linking)
Just need FTD3XX.h
```

## 1. Build FT601 in WSL

```
mkdir build
cd build
cmake -G "Ninja" .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ 
ninja
```