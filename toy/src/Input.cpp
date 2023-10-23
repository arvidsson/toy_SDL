#include "input.h"
#include <cassert>

namespace toy
{

Input::Keyboard Input::keyboard;
Input::Mouse Input::mouse;

void Input::clear()
{
    memset(keyboard.keysPressed, 0, sizeof(keyboard.keysPressed));
    memset(keyboard.keysReleased, 0, sizeof(keyboard.keysReleased));
    memset(mouse.buttonsPressed, 0, sizeof(mouse.buttonsPressed));
    memset(mouse.buttonsReleased, 0, sizeof(mouse.buttonsReleased));
}

bool Input::isKeyDown(u32 key)
{
    assert(key >= 0 && key < Keyboard::MAX_KEYS);
    return keyboard.keysDown[key];
}

bool Input::isKeyPressed(u32 key)
{
    assert(key >= 0 && key < Keyboard::MAX_KEYS);
    return keyboard.keysPressed[key];
}

bool Input::isKeyReleased(u32 key)
{
    assert(key >= 0 && key < Keyboard::MAX_KEYS);
    return keyboard.keysReleased[key];
}

bool Input::isMouseButtonDown(u32 button)
{
    assert(button >= 0 && button < Mouse::MAX_BUTTONS);
    return mouse.buttonsDown[button];
}

bool Input::isMouseButtonPressed(u32 button)
{
    assert(button >= 0 && button < Mouse::MAX_BUTTONS);
    return mouse.buttonsPressed[button];
}

bool Input::isMouseButtonReleased(u32 button)
{
    assert(button >= 0 && button < Mouse::MAX_BUTTONS);
    return mouse.buttonsReleased[button];
}

f32 Input::getMouseX()
{
    return mouse.x;
}

f32 Input::getMouseY()
{
    return mouse.y;
}

f32 Input::getMouseWheel()
{
    return mouse.w;
}

void Input::pressKey(u32 key)
{
    keyboard.keysDown[key] = true;
    keyboard.keysPressed[key] = true;
}

void Input::releaseKey(u32 key)
{
    keyboard.keysDown[key] = false;
    keyboard.keysReleased[key] = true;
}

void Input::pressMouseButton(u32 button)
{
    mouse.buttonsDown[button] = true;
    mouse.buttonsPressed[button] = true;
}

void Input::releaseMouseButton(u32 button)
{
    mouse.buttonsDown[button] = false;
    mouse.buttonsReleased[button] = true;
}

void Input::moveMouse(f32 x, f32 y)
{
    mouse.x = x;
    mouse.y = y;
}

void Input::scrollMouseWheel(f32 w)
{
    mouse.w = w;
}

}