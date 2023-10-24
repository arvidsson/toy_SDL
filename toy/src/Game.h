#pragma once

namespace toy
{

class Game
{
public:
    Game() = default;
    virtual ~Game() = default;
    virtual void init() = 0;
    virtual void shutdown() = 0;
    virtual void update() = 0;
    virtual void gui() = 0;
    virtual void render() = 0;
};

}