Overview
--------

**SAL (SDL Abstraction Library)** is a lightweight C++ wrapper around SDL2 that simplifies window creation, rendering loop, event handling, and high‑precision timing. With minimal boilerplate, you can focus on your game or app logic by subclassing `SDLEngine` and implementing just `update()` (and optionally `render()`, `onKeyDown()`, `onKeyUp()`).

Features
--------

-   **Automatic initialization** of SDL2 video and timer subsystems

-   **Hardware‑accelerated renderer** with VSync enabled

-   **High-resolution delta‑time** calculation via SDL performance counters

-   **Built‑in game loop** (run until window close)

-   **Key‑name event callbacks** (`onKeyDown`, `onKeyUp`) with string lookup

-   **Easy subclassing**: override a few virtual methods and call `run()`

Getting Started
---------------

### Prerequisites

-   C++17 (or later) compiler

-   [SDL2](https://www.libsdl.org/) development libraries installed

On Debian/Ubuntu:

```
sudo apt update
sudo apt install build-essential libsdl2-dev
```

On Fedora:

```
sudo dnf install gcc-c++ SDL2-devel
```

### Installation

1.  Clone this repository:

    ```
    git clone https://github.com/<your‑username>/SAL.git
    cd SAL
    ```

2.  Copy `SAL.h` into your project's include directory (or reference it directly).

Usage
-----

### 1\. Subclass `SDLEngine`

Create a new C++ source file, e.g. `MyGame.cpp`:

``` cpp
#include "SAL.h"

class MyGame : public SDLEngine {
public:
    MyGame() : SDLEngine("My SAL Game", 640, 480) {}

protected:
    void update(float dt) override {
        // Game logic, e.g. move objects by dt
        if (isKeyDown("LEFT"))  { /* move left */ }
        if (isKeyDown("RIGHT")) { /* move right */ }
    }

    void render() override {
        // Clear to default color, then draw shapes
        SDLEngine::render();
        // Add your rendering code here, e.g. SDL_RenderFillRect
    }

    void onKeyDown(const char* key) override {
        if (std::strcmp(key, "SPACE") == 0) {
            // Handle spacebar press
        }
    }

    void onKeyUp(const char* key) override {
        // Handle key release
    }
};

int main(int argc, char* argv[]) {
    MyGame game;
    if (!game.init()) {
        return 1;
    }
    game.run();
    return 0;
}

```

### 2\. Build

Create a simple `Makefile`:

``` make
CXX = g++
CXXFLAGS = -std=c++17 -I/path/to/SAL
LDFLAGS = -lSDL2

SRCS = MyGame.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = MyGame

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

```

Then:

```
make
./MyGame
```

API Reference
-------------

### `SDLEngine` Constructor

``` cpp
SDLEngine(
    const char* title = "SDL2 Game",
    int width = 800,
    int height = 600,
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
);

```

-   **title**: Window title

-   **width**, **height**: Window dimensions

-   **flags**: SDL window flags

### Virtual Methods

-   `bool init()`\
    Initializes SDL, creates window & renderer. Returns false on failure.

-   `void run()`\
    Enters the main loop, calling `update(dt)`, `render()`, and managing events.

-   `void cleanup()`\
    Shuts down SDL and destroys window/renderer.

-   `virtual void update(float dt) = 0;`\
    **Must override.** Game logic step, where `dt` is time since last frame in seconds.

-   `virtual void render();`\
    Override to draw your scene. Default clears to cornflower‑blue.

-   `virtual void onKeyDown(const char* key);`\
    Called when a key is pressed. `key` is the SDL scancode name (e.g. `"A"`, `"SPACE"`).

-   `virtual void onKeyUp(const char* key);`\
    Called when a key is released.

-   `bool isKeyDown(const char* name) const;`\
    Query current state of any key by name.

Contributing
------------

1.  Fork the repo

2.  Create a feature branch (`git checkout -b feature/YourFeature`)

3.  Commit your changes (`git commit -m "Add YourFeature"`)

4.  Push to the branch (`git push origin feature/YourFeature`)

5.  Open a Pull Request

License
-------

This project is licensed under the MIT License. Feel free to use and modify!
