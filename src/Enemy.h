#pragma once
#include <iostream>
#include <SDL3/SDL.h>

class Enemy
{
public:
    Enemy(float startX, float startY);

    void render(SDL_Renderer* renderer);
    void renderHealthBar(SDL_Renderer* renderer);
    void update(float deltaTime, float targetX, float targetY);
    bool isColliding(float playerX, float playerY);

    void reset() { x = 400, y = 400, health = 100;}
    void takeDamage(int damage);

    float getX() const { return x; }
    float getY() const { return y; }
    float getDamageTimer() const { return damageTimer; }

    int getHealth() const { return health; }
    bool isAlive() const { return health > 0; }

    void setDamageTimer() { damageTimer = 0.0f;}

private:
    float x;
    float y;
    float speed;
    float damageTimer;

    int health;
};