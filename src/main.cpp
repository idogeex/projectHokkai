#include <SDL3/SDL.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

enum class GameState
{
    Playing,
    GameOver
};

void spawnEnemies(std::vector<Enemy>& enemies, float x, float y)
{
    enemies.emplace_back(x, y);
}

void spawnWave(std::vector<Enemy>& enemies, Player& player, int wave)
{
    int enemiesToSpawn = wave + 2;

    for(int i = 0; i < enemiesToSpawn; i++)
    {
        float angle = 2.0f * 3.14159 * i / enemiesToSpawn;
        float x = player.getX() + std::cos(angle) * 250.0f;
        float y = player.getY() + std::sin(angle) * 250.0f;
        spawnEnemies(enemies, x, y);
    }
}

void updateGame(Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, float deltaTime, 
    GameState& gameState, int& currentWave)
{
    if(gameState == GameState::Playing)
    {
        player.update(deltaTime);
        for(Enemy& enemy: enemies)
        {
            if(enemy.isAlive())
                enemy.update(deltaTime, player.getX(), player.getY());
        }

        for(Projectile& projectile: projectiles)
        {
            if(!projectile.isActive())
                continue;

            projectile.update(deltaTime);

            for(Enemy& enemy: enemies)
            {
                if(projectile.isActive())
                {
                    if(enemy.isAlive() && projectile.isColliding(enemy.getX(), enemy.getY()))
                    {
                        enemy.takeDamage(projectile.getDamage());
                        projectile.deactivate();
                        std::cout << enemy.getHealth() << std::endl;
                        if(!enemy.isAlive())
                            std::cout << "Enemy is dead." << std::endl;
                        break;
                    }
                }
            }  
        }
            
        std::erase_if(projectiles, [](const Projectile& projectile) { return !projectile.isActive(); });
        std::erase_if(enemies, [](const Enemy& enemy) { return !enemy.isAlive(); });
        
        if(enemies.empty())
        {
            currentWave++;
            spawnWave(enemies, player, currentWave);
        }

        for(Enemy& enemy: enemies)
        {
            if(enemy.isColliding(player.getX(), player.getY()) && enemy.isAlive())
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
}

void resetGame(Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, int& currentWave,
     GameState& gameState)
{
    currentWave = 1;

    player.reset();
    enemies.clear();
    projectiles.clear();

    spawnWave(enemies, player, currentWave);

    gameState = GameState::Playing;
}

void handleEvents(bool& gameIsRunning, Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, 
    GameState& gameState, int& currentWave)
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
            if(event.key.scancode == SDL_SCANCODE_R)
                resetGame(player, enemies, projectiles, currentWave, gameState);

            float mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            float dirX, dirY;
            dirX = mouseX - (player.getX() + 25.0f);
            dirY = mouseY - (player.getY() + 25.0f);

            float length = std::sqrt(dirX * dirX + dirY * dirY);

            if (length > 0)
            {
                dirX /= length;
                dirY /= length;
            }

            if(event.key.scancode == SDL_SCANCODE_SPACE && gameState == GameState::Playing)
            {
                if(player.attack())
                {
                    Projectile projectile(player.getX() + 25.0f, player.getY() + 25.0f, 100.0f, 25.0f,
                        dirX, dirY);
                    projectiles.push_back(projectile);
                }
            }
            
            if(event.key.scancode == SDL_SCANCODE_LSHIFT && gameState == GameState::Playing)
            {
                if(player.attack())
                {
                    Projectile projectile(player.getX() + 25.0f, player.getY() + 25.0f, 300.0f, 50.0f,
                        dirX, dirY);
                    projectiles.push_back(projectile);
                }
            }
        }
    }
}

void renderGame(SDL_Renderer* renderer, Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, 
    GameState& gameState)
{
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    player.render(renderer);

    for(Enemy& enemy: enemies)
    {
        if(enemy.isAlive())
            enemy.render(renderer);
    }

    for(Projectile& projectile : projectiles)
    {
        if(projectile.isActive())
            projectile.render(renderer);
    }

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
    Player player(350.0f, 250.0f);
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    int currentWave = 1;

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

        handleEvents(gameIsRunning, player, enemies, projectiles, gameState, currentWave);

        updateGame(player, enemies, projectiles, deltaTime, gameState, currentWave);

        renderGame(renderer, player, enemies, projectiles, gameState);

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