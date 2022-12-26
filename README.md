
# Console Engine
Program framework for a Curses-style interface, including:
- OpenGL-based character grid rendering from a 256 character spritesheet with 16 foreground and background colour options.
- program loop with FPS-control and separate threads for logic/drawing and window events/input.
- keyboard and mouse input handling.
- UI element API. _**(TODO)**_

## Build Instructions
The engine is built with `CMake` using `MSVC` or `MSYS2-MinGW64`.
```
git clone https://github.com/Hop311/Console.git
cd Console
git submodule update --init --recursive
cmake -S . -B build
cd build
cmake --build .
```

## Dependencies
- [GLFW](https://www.glfw.org/) (Graphics Library Framework) - window and context creation
- [GLEW](https://glew.sourceforge.net/) (OpenGL Extension Wrangler Library) - OpenGL function pointers ([CMake version](https://github.com/Perlmint/glew-cmake))
- [pthread-win32](https://github.com/GerHobbelt/pthread-win32) - multi-threading
- [lodepng](https://lodev.org/lodepng/) - PNG decoding
