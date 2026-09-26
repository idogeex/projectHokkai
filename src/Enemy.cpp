#include <cmath>

#include "Enemy.h"

Enemy::Enemy(float startX, float startY)
{
    x = startX;
    y = startY;
    speed = 50.0f;
    damageTimer = 0.0f;
    health = 100;
}

void Enemy::render(SDL_Renderer* renderer)
{
    SDL_FRect rectangle;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = 50.0f;
    rectangle.h = 50.0f;
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);

    Enemy::renderHealthBar(renderer);
}

void Enemy::update(float deltaTime, float targetX, float targetY)
{
    float directionX, directionY;
    directionX = targetX - x;
    directionY = targetY - y;
    
    float length = std::sqrt(directionX * directionX + directionY * directionY);

    if (length > 0)
    {
        directionX /= length;
        directionY /= length;
    }  

    x += directionX * speed * deltaTime;
    y += directionY * speed * deltaTime;

    damageTimer += deltaTime;
}

bool Enemy::isColliding(float playerX, float playerY)
{
    return playerX < x + 50.0f && playerX + 50.0f > x && playerY < y + 50.0f && playerY + 50.0f > y;
}

void Enemy::takeDamage(int damage)
{
    if((health -= damage) < 0)
        health = 0;
}

void Enemy::renderHealthBar(SDL_Renderer* renderer)
{
    SDL_FRect rectangle;
    SDL_FRect rectangleHP;

    rectangle.x = getX();
    rectangle.y = getY() - 10.0f;
    rectangle.w = 50.0f;
    rectangle.h = 7.0f;
    
    float healthPercent = (float)Enemy::getHealth() / 100.0f;

    rectangleHP.x = getX();
    rectangleHP.y = getY() - 10.0f;
    rectangleHP.w = healthPercent * 50; // 1.0f = 100% 100% = 50px 1 px = 0.5%
    rectangleHP.h = 7.0f;

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &rectangle);

    SDL_SetRenderDrawColor(renderer, 196, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangleHP);
}