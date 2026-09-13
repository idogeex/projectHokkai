#pragma once
#include <SDL3/SDL.h>
#include <iostream>

class Projectile
{
public:
    Projectile(float startX, float startY, float directionX, float directionY);
    
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    // float getDamageTimer() const { return damageTimer; }

    bool isColliding(float enemyX, float enemyY) const;
    bool isActive() const { return active; }
    void deactivate() { active = false; }
    // void setDamageTimer() { damageTimer = 0.0f; }

private:
    float x, y, speed, directionX, directionY;
    bool active;
    // float damageTimer;
};