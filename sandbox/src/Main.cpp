#include "Toy.h"
using namespace toy;

/*
    TODO
    - render a sprite
    - random numbers
    - tweens
    - singleton
    - coroutines somehow
    - nuklear gui
    - yaml
*/

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

class Sandbox : public toy::Game
{
public:
    void init() override
    {
        shader.compile(toy::File::load("data/shader.vs").unwrap(), toy::File::load("data/shader.fs").unwrap());

        vb = toy::Buffer::create_vertex_buffer(vertices, sizeof(vertices));

        va = toy::VertexArray::create(vb, toy::BufferLayout{
                {toy::ShaderData::Type::Float3, "a_Position"},
            }
        );
    }

    void update() override
    {
        if (Input::isKeyDown(SDLK_ESCAPE)) {
            Application::get().quit();
        }
    }

    void render() override
    {
        shader.bind();
        va.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

private:
    toy::Shader shader;
    toy::Buffer vb;
    toy::VertexArray va;
};

int main(int argc, char *argv[])
{
    Application::execute({ .title = "sandbox", .width = 1280, .height = 720, .clearColor = {0.0f, 0.0f, 0.0f, 1.0f } }, new Sandbox());
    return 0;
}