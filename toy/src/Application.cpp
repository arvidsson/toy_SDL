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

     context = SDL_GL_CreateContext(window);

     int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
     printf("GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

     ctx = nk_sdl_init(window);
     {
         struct nk_font_atlas* atlas;
        nk_sdl_font_stash_begin(&atlas);
        nk_sdl_font_stash_end();
     }
}

Application::~Application()
{
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::run(Game* game)
{
    SDL_Event event;
    nk_colorf bg;
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    game->init();

    while (running) {
        Input::clear();
        nk_input_begin(ctx);
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
        nk_input_end(ctx);

        // GUI

        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
        {
            enum { EASY, HARD };
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                printf("button pressed!\n");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 22, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
                nk_layout_row_dynamic(ctx, 120, 1);
                bg = nk_color_picker(ctx, bg, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
                bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
                bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
                bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
                nk_combo_end(ctx);
            }
        }
        nk_end(ctx);

        game->update();

        glViewport(0, 0, props.width, props.height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(props.clearColor.r, props.clearColor.g, props.clearColor.b, props.clearColor.a);

        game->render();

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024
        nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
        SDL_GL_SwapWindow(window);
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