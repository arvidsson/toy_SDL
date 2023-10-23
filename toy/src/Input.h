#pragma once

#include "core/Types.h"
#include <SDL2/SDL.h>

namespace toy
{

class Input
{
public:
    static void clear();

    static bool isKeyDown(u32 key);
    static bool isKeyPressed(u32 key);
    static bool isKeyReleased(u32 key);

    static bool isMouseButtonDown(u32 button);
    static bool isMouseButtonPressed(u32 button);
    static bool isMouseButtonReleased(u32 button);

    static f32 getMouseX();
    static f32 getMouseY();
    static f32 getMouseWheel();

private:
    struct Keyboard
    {
        static const int MAX_KEYS = SDL_NUM_SCANCODES;
        bool keysDown[MAX_KEYS];
        bool keysPressed[MAX_KEYS];
        bool keysReleased[MAX_KEYS];
    };

    struct Mouse
    {
        static const int MAX_BUTTONS = 3;
        bool buttonsDown[MAX_BUTTONS];
        bool buttonsPressed[MAX_BUTTONS];
        bool buttonsReleased[MAX_BUTTONS];
        f32 x, y, w;
    };

    static Keyboard keyboard;
    static Mouse mouse;

    static void pressKey(u32 key);
    static void releaseKey(u32 key);
    static void pressMouseButton(u32 button);
    static void releaseMouseButton(u32 button);
    static void moveMouse(f32 x, f32 y);
    static void scrollMouseWheel(f32 w);

    friend class Application;
};

}