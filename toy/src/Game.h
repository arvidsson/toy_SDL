#pragma once

namespace toy
{

class Game
{
public:
    Game() = default;
    virtual ~Game() = default;
    virtual void run() = 0;
};

}