Base code OpenGL Assignment
===========================

### Building

This is a standard CMake project. Building it consits in running:

```
cd <path-to-BaseGL-directory>
mkdir build
cd build
cmake ..
cmake --build .
```

The resuling BaseGL executable is automatically sopied to the root BaseGL directory, so that shaders can be loaded. 

### Running

To run the program
```
cd <path-to-BaseGL-directory>
./BaseGL
```

When starting to edit the source code, rerun cmake --build build to recompile (and copy) the binary
