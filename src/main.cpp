#include <SDL3/SDL.h>
#include <iostream>
#include <cmath>

#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

enum class GameState
{
    Playing,
    GameOver
};

void updateGame(Player& player, Enemy& enemy, Projectile& projectile, float deltaTime, GameState& gameState)
{
    if(gameState == GameState::Playing)
    {
        player.update(deltaTime);
        enemy.update(deltaTime, player.getX(), player.getY());
        projectile.update(deltaTime);

        if(projectile.isColliding(enemy.getX(), enemy.getY()))
        {
            enemy.takeDamage(50);
            std::cout << enemy.getHealth() << std::endl;
                if(!enemy.isAlive())
                    std::cout << "Enemy is dead." << std::endl;
        }

        if(enemy.isColliding(player.getX(), player.getY()))
        {
            if(enemy.getDamageTimer() >= 1.0f)
            {
                player.takeDamage(25);
                std::cout << player.getHealth() << std::endl;
                enemy.setDamageTimer();
                if(!player.isAlive())
                {
                    std::cout << "Player is dead." << std::endl;
                    gameState = GameState::GameOver;
                }
            }
        }
    }
}

void resetGame(Player& player, Enemy& enemy, GameState& gameState)
{
    player.reset();
    enemy.reset();

    gameState = GameState::Playing;
}

void handleEvents(bool& gameIsRunning, Player& player, Enemy& enemy, GameState& gameState)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            gameIsRunning = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            if(event.key.scancode == SDL_SCANCODE_R && gameState == GameState::GameOver)
                resetGame(player, enemy, gameState);
            else if(event.key.scancode == SDL_SCANCODE_SPACE)
                player.attack();
        }
    }
}

void renderGame(SDL_Renderer* renderer, Player& player, Enemy& enemy, Projectile& projectile,GameState& gameState)
{
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    player.render(renderer);
    enemy.render(renderer);
    projectile.render(renderer);

    if(gameState == GameState::GameOver)
    {
        SDL_FRect gameOverRectangle;

        gameOverRectangle.x = 250.0f;
        gameOverRectangle.y = 200.0f;
        gameOverRectangle.w = 300.0f;
        gameOverRectangle.h = 200.0f;

        SDL_SetRenderDrawColor(renderer, 100, 0, 100, 255);
        SDL_RenderFillRect(renderer, &gameOverRectangle);
    }

    SDL_RenderPresent(renderer);
}

int main()
{
    float deltaTime;
    Player player(0.0f, 100.0f);
    Enemy enemy(400.0f, 100.0f);
    Projectile projectile(100.0f, 100.0f, 1.0f, 0.0f);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Project Hokkai", 800, 600, 0);

    if (window == nullptr)
    {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (renderer == nullptr)
    {
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool gameIsRunning = true;
    Uint64 previousTime = SDL_GetTicksNS();

    int maxFPS = 60;

    const Uint64 targetFrameTime = 1000000000.0f / maxFPS;

    GameState gameState = GameState::Playing;

    while (gameIsRunning)
    {
        Uint64 currentTime = SDL_GetTicksNS();
        Uint64 frameTime;
        deltaTime = (currentTime - previousTime) / 1000000000.0f;

        handleEvents(gameIsRunning, player, enemy, gameState);

        updateGame(player, enemy, projectile, deltaTime, gameState);

        renderGame(renderer, player, enemy, projectile, gameState);

        frameTime = SDL_GetTicksNS() - currentTime;
        if (frameTime < targetFrameTime)
            SDL_DelayPrecise(targetFrameTime - frameTime);
        previousTime = currentTime;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}