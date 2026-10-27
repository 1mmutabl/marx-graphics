# marx-graphics

A lightweight Linux graphics library with one goal: **make working with windows easy.**

`marx-graphics` is a C++20 graphics library designed to provide a simple interface for window management and 2D rendering.

> **Development status:** The library is still in its early stages and is missing some key components. Features and APIs may change.

## Features

- Lightweight 2D renderer.
- A simple API inspired by the ease of use of SFML.
- Window creation and event polling.
- Basic rendering, including rectangles, circles, lines, and text.

## Requirements

- A C++20-compatible compiler (GCC or Clang).
- GNU Make.
- GLAD.

Additional system dependencies may be required, depending on your build configuration.

## Installation

Clone the repository:

```bash
git clone https://github.com/1mmutabl/marx-graphics.git
cd marx-graphics
```

### Option 1: Use the prebuilt library

The `lib/` directory contains the dynamically linked library.

To use it in your project, include the public headers and link against the library. For example:

```bash
g++ main.cpp -Iinclude -Llib -lmarx-graphics -o example
```

Depending on how the library was built, you may need additional linker flags, libraries, or runtime library paths.

### Option 2: Build from source

You can integrate the library's source code into your project.

Add the `include/` directory to your project's include paths and compile the required implementation files from `src/`.

Check the project's build configuration and dependencies for the complete list of files and compiler flags required.

## Quick Start

The following example creates a window and handles its close event:

```cpp
#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events = graphics::events;

int main() {
    graphics::window window;

    while (window.is_open()) {
        while (const auto event = window.poll()) {
            if (event->is<events::Close>()) {
                window.close();
            }
        }
    }

    return 0;
}
```

This example demonstrates the basic window lifecycle:

1. Create a window.
2. Poll for events.
3. Handle the close event.
4. Close the window.

### Compile the example

```bash
g++ main.cpp -Iinclude -Llib -lmarx-graphics -o example
```

Note that additional linker flags or runtime library paths may be required, depending on your build configuration.

## Usage

See the project's examples for demonstrations of window creation, event handling, and rendering.

As the library develops, this section will include examples of drawing shapes, rendering text, and handling other graphics-related tasks.

## API Reference

The public API is exposed through the following header:

```cpp
#include "marx/graphics.hpp"
```

Browse the [`include/`](include/) directory to explore the public headers and available functionality.

Detailed API documentation can be added as the library grows.

## Project Structure

```text
marx-graphics/
├── lib/           # Dynamically linked library
├── dependencies/  # External dependencies
├── include/       # Public headers
├── src/           # Implementation files
├── templates/     # Templates
├── resources/     # Resources used by templates
├── Makefile       # Build configuration
├── .clang-format  # Code formatting rules
├── build.fish     # Build script
├── LICENSE        # MIT license
└── README.md
```

## Compatibility

- **C++ standard:** C++20
- **Compilers:** GCC and Clang
- **Operating system:** Linux
- **Dependencies:** GLAD

## Contributing

Bug reports and suggestions are welcome through GitHub Issues.

Contributions are not currently accepted.

# AI
AI has been used in some parts of this project, it was used to debug after hour long sessions, completing time wasting tasks, like repetitive typing, especially found in the keyboard input header and to polish this markdown.
Keep in mind the entire project is still human made with human ideas.

## License

This project is licensed under the MIT License. See [`LICENSE`](LICENSE) for details.
