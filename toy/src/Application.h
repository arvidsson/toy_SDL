#pragma once

#include "Core.h"
#include "SDL.h"
#include <nuklear.h>

namespace toy
{

class Game;

struct ApplicationProps
{
    String title;
    u32 width;
    u32 height;
    Color clearColor;
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

    ApplicationProps props;
    bool running = true;
    SDL_Window* window;
    SDL_GLContext context;
    nk_context* ctx;

    static Application* instance;
};

}