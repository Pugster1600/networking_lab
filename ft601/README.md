## 0. Prereqs
```
WSL
FT601 driver - https://ftdichip.com/drivers/d3xx-drivers/ (windows for driver)
FT601 dynamic library - https://ftdichip.com/drivers/d3xx-drivers/ (linux for dynamic linking)
Place the .h files in inc, the 2 .so* files in lib
```

## 1. Build in FT601

```
mkdir build
cd build
cmake ..
make
```