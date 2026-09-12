#pragma once
#include <iostream>
#include <SDL3/SDL.h>

class Player
{
private:
    float x;
    float y;
    int health;
    float speed;

public:
    Player(float startX, float startY);

    float getX() const { return x; }
    float getY() const { return y; }

    void update(float deltaTime);

    void printPlayerPosition()
    {
        std::cout << "Player: " << x << ", " << y << std::endl;
    }

};