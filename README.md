# OOPS-2026-Project

C++ OOP and ImGui based hamburger factory simulation project.

## Project Structure

- `include/model`, `src/model`: factory data and process logic
- `include/controller`, `src/controller`: connection between UI input and model updates
- `include/view`, `src/view`: ImGui rendering code
- `libs/imgui`: Dear ImGui submodule
- `ui-gifs`: demo GIFs
- `uml`: UML notes
- `docs`: presentation and diagram documents

## Requirements

- CMake 3.15 or newer
- C++17 compiler
- SDL2
- OpenGL

### macOS

Install dependencies with Homebrew:

```sh
brew install cmake pkg-config sdl2
```

Build:

```sh
git submodule update --init --recursive
cmake --preset macos
cmake --build --preset macos
```

Run:

```sh
./build_macos/imgui_app
```

### Windows MSYS2 MinGW

Install the MinGW SDL2 and CMake packages, then build with:

```sh
git submodule update --init --recursive
cmake --preset windows
cmake --build --preset windows
```

The executable is created in `build_windows`.

## Notes

- Build folders and local ImGui window settings are ignored by Git.
- The code follows an MVC structure: Model has no ImGui code, View only renders UI, and Controller connects user actions to model state changes.
