#include <SDL3/SDL.h>
#include <iostream>

struct Player
{
    float x;
    float y;
    int health;
};

void printPlayerPosition(const Player& player)
{
    std::cout << "Player: " << player.x << ", " << player.y << std::endl;
}

void updatePlayer(Player& player, float deltaTime)
{
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    float speed;

    if (keyboardState[SDL_SCANCODE_W])
        player.y -= speed * deltaTime;
    
    if (keyboardState[SDL_SCANCODE_S])
        player.y += speed * deltaTime;

    if (keyboardState[SDL_SCANCODE_A])
        player.x -= speed * deltaTime;
        
    if (keyboardState[SDL_SCANCODE_D])
        player.x += speed * deltaTime;    
        
    if (player.x < 0)
        player.x = 0;

    if (player.x > 750)
        player.x = 750;

    if (player.y < 0)
        player.y = 0;
    
    if (player.y > 550)
        player.y = 550;
}

void renderPlayer(SDL_Renderer* renderer, const Player& player)
{
    SDL_FRect rectangle;

    rectangle.x = static_cast<float>(player.x);
    rectangle.y = static_cast<float>(player.y); 
    rectangle.w = 50.0f;
    rectangle.h = 50.0f;
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rectangle);
}

int main()
{
    Player player{0, 0, 100};
    float deltaTime;

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

    while (gameIsRunning)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameIsRunning = false;
            }
        }

        updatePlayer(player, deltaTime);

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        renderPlayer(renderer, player);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}