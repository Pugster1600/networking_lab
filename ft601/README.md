## 0. Prereqs
```
WSL

```

## 1. Build in FT601

```
mkdir build
cd build
cmake ..
make
```

note: dont just blindy do it, ask what hte reason is and the reasons you are faililng

NOTE: git might not be able to add symbol files for linking (symbols resolved at link time BUT definitions are resolved at run time)
- function resolution, symbol resolution then definition resolution