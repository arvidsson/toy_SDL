#include "Application.h"
#include "Input.h"
#include "Game.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "vendor/nuklear_sdl_gl3.h"

namespace toy
{

Application* Application::instance = nullptr;

Application::Application(ApplicationProps props) : props(props)
{
    SDL_Log("Starting application...");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        throw RuntimeError("Failed to init SDL2");
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(props.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, props.width, props.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        throw RuntimeError("Failed to create window");
    }

     glContext = SDL_GL_CreateContext(window);

     int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
     printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

     guiContext = nk_sdl_init(window);
     {
        struct nk_font_atlas* atlas;
        nk_sdl_font_stash_begin(&atlas);
        nk_sdl_font_stash_end();
     }
}

Application::~Application()
{
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::run(Game* game)
{
    SDL_Event event;

    game->init();

    while (running) {
        Input::clear();
        nk_input_begin(guiContext);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    Input::pressKey(event.key.keysym.sym);
                    break;

                case SDL_KEYUP:
                    Input::releaseKey(event.key.keysym.sym);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    Input::pressMouseButton(event.button.button);
                    break;

                case SDL_MOUSEBUTTONUP:
                    Input::releaseMouseButton(event.button.button);
                    break;

                case SDL_MOUSEMOTION:
                    Input::moveMouse((f32)event.motion.x, (f32)event.motion.y);
                    break;

                case SDL_MOUSEWHEEL:
                    Input::scrollMouseWheel((f32)event.wheel.y);
                    break;

                case SDL_QUIT:
                    quit();
                    break;
            }

            nk_sdl_handle_event(&event);
        }
        nk_input_end(guiContext);

        game->update();
        game->gui();

        glViewport(0, 0, props.width, props.height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(props.clearColor.r, props.clearColor.g, props.clearColor.b, props.clearColor.a);

        game->render();

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
        nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
        SDL_GL_SwapWindow(window);
    }

    game->shutdown();
}

void Application::quit()
{
    running = false;
}

Application& Application::get()
{
    assert(instance != nullptr);
    return *instance;
}

void Application::execute(ApplicationProps props, Game* game)
{
    Application* app = nullptr;

    // setup logging
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("toy.log", true);
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("log", spdlog::sinks_init_list({ console_sink, file_sink })));
    }

    try {
        app = new Application(props);
        instance = app;
        app->run(game);
    }
    catch (const std::exception& e) {
        LOG_ERROR("Exception: {}", e.what());
    }

    delete app;
}

}