#include "Application.h"
#include "Input.h"
#include "Game.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace toy
{

Application* Application::instance = nullptr;

Application::Application(ApplicationProps props)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow(props.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, props.width, props.height, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
}

Application::~Application()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::run(Game* game)
{
    SDL_Event event;

    while (running) {
        Input::clear();

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
        }

        game->update();
    }
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