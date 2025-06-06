A Simple implementation of Pong Game using C++ and Raylib

Setup for M-Series MacOS:

use thise in CMakeLists.txt:

```cmake
list list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/arm64-osx")"
```

und add this to the CMakeLists.txt file:

```cmake
find_package(glwf3 CONFIG REQUIRED)

target_link_libraries(pong PRIVATE glfw3)

```
