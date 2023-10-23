#include "Toy.h"

int main(int argc, char *argv[])
{
    toy::Application* app = new toy::Application({.title = "sandbox", .width = 1280, .height = 720});
    app->run();
    return 0;
}