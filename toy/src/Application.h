#pragma once

#include "core/Types.h"
#include <SDL2/SDL.h>

namespace toy
{

class Game;

struct ApplicationProps
{
    String title;
    u32 width;
    u32 height;
};

class Application final
{
public:
    Application(ApplicationProps props);
    ~Application();
    void run(Game* game);
    void quit();

    static void set(Application*);
    static Application& get();

private:
    bool running = true;
    SDL_Window* window;

    static Application* instance;
};

}