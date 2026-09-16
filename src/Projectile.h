#pragma once
#include <SDL3/SDL.h>
#include <iostream>

class Projectile
{
public:
    Projectile(float startX, float startY, float speed, int damage, float directionX, float directionY);
    
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    // float getDamageTimer() const { return damageTimer; }
    int getDamage() const { return damage;}

    bool isColliding(float enemyX, float enemyY) const;
    bool isActive() const { return active; }
    void deactivate();
    // void setDamageTimer() { damageTimer = 0.0f; }

private:
    float x, y, speed, directionX, directionY;
    bool active;
    float size;
    float lifeTime, lifeTimer;
    int damage;
    // float damageTimer;
};