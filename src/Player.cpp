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
    }        

    if (keyboardState[SDL_SCANCODE_W])
        y += directionY * speed * deltaTime;

    if (keyboardState[SDL_SCANCODE_S])
        y += directionY * speed * deltaTime;

    if (keyboardState[SDL_SCANCODE_A])
        x += directionX * speed * deltaTime;
        
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

void Player::render(SDL_Renderer* renderer)
{
    SDL_FRect rectangle;

    rectangle.x = getX();
    rectangle.y = getY();
    rectangle.w = 50.0f;
    rectangle.h = 50.0f;
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);
}

void Player::takeDamage(int damage)
{
    if((health -= damage) < 0)
        health = 0;
}

bool Player::attack()
{   
    std::cout<< "Attack!" << std::endl;
    return true;
}