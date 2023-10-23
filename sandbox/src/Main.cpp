#include "Toy.h"

using Input = toy::Input;

class Sandbox : public toy::Game
{
    void update() override
    {
        if (Input::isKeyDown(SDLK_ESCAPE)) {
            throw toy::RuntimeError("An error happened");
            toy::Application::get().quit();
        }
    }

    void render() override
    {

    }
};

int main(int argc, char *argv[])
{
    toy::Application::execute({ .title = "sandbox", .width = 1280, .height = 720 }, new Sandbox());
    return 0;
}