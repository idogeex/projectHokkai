#include "Player.h"

Player::Player(float startX, float startY)
{
    x = startX;
    y = startY;
    health = 100;
    speed = 200.0f;
}

void Player::update(float deltaTime)
{
    float directionX = 0.0f;
    float directionY = 0.0f;

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);

    if (keyboardState[SDL_SCANCODE_W])
        directionY += 1;
        
    if (keyboardState[SDL_SCANCODE_S])
        directionY += 1;

    if (keyboardState[SDL_SCANCODE_A])
        directionX += 1;
        
    if (keyboardState[SDL_SCANCODE_D])
        directionX += 1;    

    float length = std::sqrt(directionX * directionX + directionY * directionY);

    if (length > 0)
    {
        directionX = directionX / length;
        directionY = directionY / length;
    }        

    if (keyboardState[SDL_SCANCODE_W])
        y -= directionY * speed * deltaTime;

    if (keyboardState[SDL_SCANCODE_S])
        y += directionY * speed * deltaTime;

    if (keyboardState[SDL_SCANCODE_A])
        x -= directionX * speed * deltaTime;
        
    if (keyboardState[SDL_SCANCODE_D])
        x += directionX * speed * deltaTime;
        
    if (x < 0)
        x = 0;

    if (x > 750)
        x = 750;

    if (y < 0)
        y = 0;
    
    if (y > 550)
        y = 550;
}