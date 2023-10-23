#include "Application.h"
#include "Input.h"
#include "Game.h"
#include <SDL2/SDL.h>
#include <cassert>

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
                    Input::moveMouse(event.motion.x, event.motion.y);
                    break;

                case SDL_MOUSEWHEEL:
                    Input::scrollMouseWheel(event.wheel.y);
                    break;

                case SDL_QUIT:
                    quit();
                    break;
            }
        }

        game->run();
    }
}

void Application::quit()
{
    running = false;
}

void Application::set(Application* application)
{
    instance = application;
}

Application& Application::get()
{
    assert(instance != nullptr);
    return *instance;
}

}