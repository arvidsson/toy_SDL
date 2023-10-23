#include "Toy.h"

using Input = toy::Input;

class Sandbox : public toy::Game
{
    void run() override
    {
        if (Input::isKeyDown(SDLK_ESCAPE)) {
            toy::Application::get().quit();
        }
    }
};

int main(int argc, char *argv[])
{
    toy::Application* app = new toy::Application({.title = "sandbox", .width = 1280, .height = 720});
    toy::Application::set(app);
    app->run(new Sandbox());
    return 0;
}