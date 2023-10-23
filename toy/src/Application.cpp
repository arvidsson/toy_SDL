#include "Application.h"
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

void Application::run()
{
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit();
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit();
                }
            }
        }
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