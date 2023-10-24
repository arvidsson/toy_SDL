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

    void shutdown() override
    {

    }

    void update() override
    {
        if (Input::isKeyDown(SDLK_ESCAPE)) {
            Application::get().quit();
        }
    }

    void gui() override
    {
        auto guiContext = Application::get().getGuiContext();

        nk_colorf bg;
        bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

        if (nk_begin(guiContext, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
        {
            enum { EASY, HARD };
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(guiContext, 30, 80, 1);
            if (nk_button_label(guiContext, "button"))
                printf("button pressed!\n");
            nk_layout_row_dynamic(guiContext, 30, 2);
            if (nk_option_label(guiContext, "easy", op == EASY)) op = EASY;
            if (nk_option_label(guiContext, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(guiContext, 22, 1);
            nk_property_int(guiContext, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(guiContext, 20, 1);
            nk_label(guiContext, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(guiContext, 25, 1);
            if (nk_combo_begin_color(guiContext, nk_rgb_cf(bg), nk_vec2(nk_widget_width(guiContext), 400))) {
                nk_layout_row_dynamic(guiContext, 120, 1);
                bg = nk_color_picker(guiContext, bg, NK_RGBA);
                nk_layout_row_dynamic(guiContext, 25, 1);
                bg.r = nk_propertyf(guiContext, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
                bg.g = nk_propertyf(guiContext, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
                bg.b = nk_propertyf(guiContext, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
                bg.a = nk_propertyf(guiContext, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
                nk_combo_end(guiContext);
            }
        }
        nk_end(guiContext);
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