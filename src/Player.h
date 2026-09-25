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
    float directionX, directionY;
    float attackCooldown, attackTimer;

public:
    Player(float startX, float startY);

    float getX() const { return x; }
    float getY() const { return y; }
    int getHealth() const { return health; }
    float getDirectionX() const { return directionX; }
    float getDirectionY() const { return directionY; }

    void reset() { x = 0, y = 0, health = 100; }

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void takeDamage(int damage);
    bool attack();
    bool isAlive() const { return health > 0; }

    void printPlayerPosition()
    {
        std::cout << "Player: " << x << ", " << y << std::endl;
    }

};