#pragma once

#include "Core.h"
#include "SDL.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
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

    nk_context* getGuiContext() { return guiContext; }

private:
    Application(ApplicationProps props);
    ~Application();
    void run(Game* game);

    ApplicationProps props;
    bool running = true;
    SDL_Window* window;
    SDL_GLContext glContext;
    nk_context* guiContext;

    static Application* instance;
};

}