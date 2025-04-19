#include "SAL.h"

class Game : public SDLEngine {
public:
    Game() : SDLEngine("Title", 640, 480) {}

protected:

    // Called every frame (move objects, ect...)
    void update(float dt) override {

    }

    // Called every frame after update
    void render() override {
        // Clear with default background (calls base implementation)
        SDLEngine::render();
    }

    void onKeyDown(const char* key) override {}
    void onKeyUp(const char* key) override {}
};

int main(int argc, char* argv[]) {
    Game game;
    if (!game.init()) {
        return 1;
    }
    game.run();
    return 0;
}
