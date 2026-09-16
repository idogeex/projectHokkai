#include <cmath>

#include "Projectile.h"

Projectile::Projectile(float startX, float startY, float speed, int damage, float directionX, float directionY)
{
    x = startX;
    y = startY;
    Projectile::speed = speed;
    active = true;
    size = 10.0f;
    lifeTime = 3.0f;
    lifeTimer = 0.0f;
    Projectile::damage = damage;
    // damageTimer = 0.0f;

    this->directionX = directionX;
    this->directionY = directionY;
}

void Projectile::update(float deltaTime)
{
    float length = std::sqrt(directionX * directionX + directionY * directionY);

    if (length > 0)
    {
        directionX /= length;
        directionY /= length;
    }  

    x += directionX * speed * deltaTime;
    y += directionY * speed * deltaTime;

    lifeTimer += deltaTime;
    if(lifeTimer >= lifeTime)
        active = false;
    
    // damageTimer += deltaTime;
}

void Projectile::render(SDL_Renderer* renderer)
{
    SDL_FRect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = rectangle.h = size;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);
}

bool Projectile::isColliding(float enemyX, float enemyY) const
{
    if (x < enemyX + 50.0f && x + size > enemyX && 
        y < enemyY + 50.0f && y + size > enemyY)
    {
        return true;
    }

    return false;
}

void Projectile::deactivate()
{
    active = false;
}