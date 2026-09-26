#include <cmath>

#include "Player.h"

Player::Player(float startX, float startY)
{
    x = startX;
    y = startY;
    health = 100;
    speed = 200.0f;
    directionX = 1;
    directionY = 0;
    attackCooldown = 1.0f;
    attackTimer = attackCooldown;
}

void Player::update(float deltaTime)
{
    float directionX = 0.0f;
    float directionY = 0.0f;

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_W])
        directionY -= 1;
        
    if (keyboardState[SDL_SCANCODE_S])
        directionY += 1;

    if (keyboardState[SDL_SCANCODE_A])
        directionX -= 1;
        
    if (keyboardState[SDL_SCANCODE_D])
        directionX += 1;

    float length = std::sqrt(directionX * directionX + directionY * directionY);

    if (length > 0)
    {
        directionX /= length;
        directionY /= length;

        Player::directionX = directionX;
        Player::directionY = directionY;
    }

    y += directionY * speed * deltaTime;
    x += directionX * speed * deltaTime;
        
    if (x < 0)
        x = 0;

    if (x > 750)
        x = 750;

    if (y < 0)
        y = 0;
    
    if (y > 550)
        y = 550;

    attackTimer += deltaTime;
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_FRect rectangle;

    rectangle.x = getX();
    rectangle.y = getY();
    rectangle.w = 50.0f;
    rectangle.h = 50.0f;
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);

    Player::renderHealthBar(renderer);
}

void Player::takeDamage(int damage)
{
    if((health -= damage) < 0)
        health = 0;
}

bool Player::attack()
{   
    if(attackTimer >= attackCooldown)
    {
        attackTimer = 0.0f;
        return true;
    }
    else
        return false;
}

void Player::renderHealthBar(SDL_Renderer* renderer)
{
    SDL_FRect rectangle;
    SDL_FRect rectangleHP;

    rectangle.x = getX();
    rectangle.y = getY() - 10.0f;
    rectangle.w = 50.0f;
    rectangle.h = 7.0f;
    
    float healthPercent = (float)Player::getHealth() / 100.0f;
    if(healthPercent > 1.0f && healthPercent < 0.0f)
        healthPercent = 1.0f;

    rectangleHP.x = getX();
    rectangleHP.y = getY() - 10.0f;
    rectangleHP.w = healthPercent * 50; // 1.0f = 100% 100% = 50px 1 px = 0.5%
    rectangleHP.h = 7.0f;

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &rectangle);

    SDL_SetRenderDrawColor(renderer, 0, 196, 0, 255);
    SDL_RenderFillRect(renderer, &rectangleHP);
}