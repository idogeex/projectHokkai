#include <SDL3/SDL.h>
#include <iostream>
#include <cmath>

#include "Player.h"

void renderPlayer(SDL_Renderer* renderer, const Player& player)
{
    SDL_FRect rectangle;

    rectangle.x = player.getX();
    rectangle.y = player.getY();
    rectangle.w = 50.0f;
    rectangle.h = 50.0f;
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);
}

int main()
{
    float deltaTime;
    Player player(0, 0);

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

    const Uint64 targetFrameTime = 1000000000 / 60;

    while (gameIsRunning)
    {
        SDL_Event event;
        Uint64 currentTime = SDL_GetTicksNS();
        Uint64 frameTime;
        deltaTime = (currentTime - previousTime) / 1000000000.0f;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameIsRunning = false;
            }
        }

        player.update(deltaTime);

        int FPS;
        std::cout << "deltaTime: " << deltaTime << std::endl;
        FPS = 1 / deltaTime;
        std::cout << "FPS: " << FPS << std::endl;

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        renderPlayer(renderer, player);

        SDL_RenderPresent(renderer);
        previousTime = currentTime;
        frameTime = SDL_GetTicksNS() - currentTime;
        if (frameTime < targetFrameTime)
            SDL_DelayPrecise(targetFrameTime - frameTime);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}