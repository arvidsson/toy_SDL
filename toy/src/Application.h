#pragma once

#include "Core.h"
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
    void quit();

    static Application& get();
    static void execute(ApplicationProps props, Game* game);

private:
    Application(ApplicationProps props);
    ~Application();
    void run(Game* game);

    bool running = true;
    SDL_Window* window;

    static Application* instance;
};

}