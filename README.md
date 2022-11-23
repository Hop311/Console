
# Console Engine
Program framework for a Curses-style interface, including:
- OpenGL-based character grid rendering from a 256 character spritesheet with 16 foreground and background colour options.
- program loop with FPS-control and separate threads for logic/drawing and window events/input.
- keyboard and mouse input handling. _**(TODO)**_
- UI element API. _**(TODO)**_

## Build Instructions
The engine is built with `CMake` using `MSVC` or `MSYS2-MinGW64`.
```
cmake -S . -B build
cd build
cmake --build .
```

## Dependencies
- [GLFW](https://www.glfw.org/) (Graphics Library Framework) - window and context creation
- [GLEW](https://glew.sourceforge.net/) (OpenGL Extension Wrangler Library) - OpenGL function pointers
- [pthread4w](https://github.com/jwinarske/pthreads4w) - multi-threading
- [lodepng](https://lodev.org/lodepng/) - PNG decoding
