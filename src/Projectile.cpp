#include "Projectile.h"

Projectile::Projectile(float startX, float startY, float directionX, float directionY)
{
    x = startX;
    y = startY;
    speed = 400.0f;

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
}

void Projectile::render(SDL_Renderer* renderer)
{
    SDL_FRect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = rectangle.h = 10.0f;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);
}

bool Projectile::isColliding(float enemyX, float enemyY) const
{
    if (x < enemyX + 50.0f && x + 10.0f > enemyX && 
        y < enemyY + 50.0f && y + 10.0f > enemyY)
    {
        return true;
    }

    return false;
}