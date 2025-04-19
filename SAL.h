#ifndef SAL
#define SAL

#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>

class SDLEngine {
public:
    SDLEngine(const char* title = "SDL2 Game",
              int width = 800,
              int height = 600,
              Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
      : title_(title),
        width_(width),
        height_(height),
        flags_(flags),
        window_(nullptr),
        renderer_(nullptr),
        isRunning_(false),
        perfFreq_(0),
        lastCounter_(0)
    {}

    virtual ~SDLEngine() {
        cleanup();
    }

    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
            return false;
        }

        window_ = SDL_CreateWindow(
            title_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width_, height_, flags_
        );
        if (!window_) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
            return false;
        }

        // Enable hardware acceleration + VSync
        renderer_ = SDL_CreateRenderer(
            window_,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
        if (!renderer_) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
            return false;
        }

        // Initialize SDL performance counters
        perfFreq_    = SDL_GetPerformanceFrequency();
        lastCounter_ = SDL_GetPerformanceCounter();

        isRunning_ = true;
        return true;
    }

    void run() {
        while (isRunning_) {
            // compute dt in seconds
            Uint64 now = SDL_GetPerformanceCounter();
            float dt = static_cast<float>(now - lastCounter_) / perfFreq_;
            lastCounter_ = now;

            handleEvents();
            update(dt);
            render();
            SDL_RenderPresent(renderer_);

            // no need for extra delay: VSync caps the framerate
        }
    }

    void cleanup() {
        if (renderer_) SDL_DestroyRenderer(renderer_);
        if (window_)   SDL_DestroyWindow(window_);
        SDL_Quit();
    }

protected:
    SDL_Window*   window_;
    SDL_Renderer* renderer_;
    bool          isRunning_;

    virtual void update(float dt) = 0;

    virtual void render() {
        SDL_SetRenderDrawColor(renderer_, 100, 149, 237, 255);
        SDL_RenderClear(renderer_);
    }

    virtual void onKeyDown(const char* key) {}
    virtual void onKeyUp  (const char* key) {}

    bool isKeyDown(const char* name) const {
        if (!name || std::strlen(name) == 0) {
            return false;  // no name → not down
        }

        // SDL expects uppercase names, so we'll make a local copy
        char upperName[32];
        std::size_t len = std::min<std::size_t>(std::strlen(name), sizeof(upperName)-1);
        for (std::size_t i = 0; i < len; ++i) {
            upperName[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(name[i])));
        }
        upperName[len] = '\0';

        // Look up the scancode by name
        SDL_Scancode sc = SDL_GetScancodeFromName(upperName);
        if (sc == SDL_SCANCODE_UNKNOWN) {
            // if SDL doesn't recognize it (e.g. you passed "foo"), treat as not pressed
            return false;
        }

        // Query current keyboard state
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        return state[sc] != 0;
    }

private:
    const char* title_;
    int         width_, height_;
    Uint32      flags_;

    Uint64      perfFreq_;     // SDL performance frequency
    Uint64      lastCounter_;  // counter at end of last frame

    void handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    isRunning_ = false;
                    break;
                case SDL_KEYDOWN:
                    onKeyDown(SDL_GetScancodeName(e.key.keysym.scancode));
                    break;
                case SDL_KEYUP:
                    onKeyUp(SDL_GetScancodeName(e.key.keysym.scancode));
                    break;
                default:
                    break;
            }
        }
    }
};

#endif // SAL
