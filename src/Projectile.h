#pragma once
#include <SDL3/SDL.h>
#include <iostream>

class Projectile
{
public:
    Projectile(float startX, float startY, float directionX, float directionY);
    
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    bool isColliding(float enemyX, float enemyY) const;

private:
    float x, y, speed, directionX, directionY;
};