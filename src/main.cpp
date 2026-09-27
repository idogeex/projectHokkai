#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

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

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, float x, float y)
{
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text.size(), textColor);
    
    if(surface == nullptr)
    {
        std::cout << "text failed" << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    if(texture == nullptr)
    {
        std::cout << "texture failed" << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return;
    }

    SDL_DestroySurface(surface);

    float textureWidth, textureHeight;

    if (!SDL_GetTextureSize(texture, &textureWidth, &textureHeight))
    {
        std::cout << "Getting texture size failed: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(texture);
        return;
    }

    SDL_FRect destination;
    destination.x = x;
    destination.y = y;
    destination.w = textureWidth;
    destination.h = textureHeight;

    SDL_RenderTexture(renderer, texture, nullptr, &destination);
    SDL_DestroyTexture(texture);
}

void updateGame(Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, float deltaTime, 
    GameState& gameState, int& currentWave, float& waveTimer, int& enemiesKilled, int& bestWave)
{
    if(gameState == GameState::Playing)
    {
        player.update(deltaTime);
        for(int i = 0; i < enemies.size(); i++)
        {
            if(enemies[i].isAlive())
                enemies[i].update(deltaTime, player.getX(), player.getY());
                enemies[i].separateFromOthers(enemies, i);
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
                        {    
                            std::cout << "Enemy is dead." << std::endl;
                            enemiesKilled++;
                        }
                        break;
                    }
                }
            }  
        }

        for(Enemy& enemy: enemies)
        {
            if(enemy.isAlive() && enemy.isColliding(player.getX(), player.getY()))
            {
                if(enemy.getDamageTimer() >= 1.0f)
                {
                    player.takeDamage(10);
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
            
        std::erase_if(projectiles, [](const Projectile& projectile) { return !projectile.isActive(); });
        std::erase_if(enemies, [](const Enemy& enemy) { return !enemy.isAlive(); });
        
        const float waveCooldown = 3.0f;

        if(enemies.empty())
        {
            waveTimer -= deltaTime;
            if(waveTimer <= 0.0f)
            {   
                bestWave = currentWave;
                currentWave++;
                spawnWave(enemies, player, currentWave);
                waveTimer = waveCooldown;
            }
        }
    }
}

void resetGame(Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, int& currentWave,
     GameState& gameState, float& waveTimer, int& enemiesKilled)
{
    const float waveCooldown = 3.0f;

    currentWave = 1;
    waveTimer = waveCooldown;
    enemiesKilled = 0;

    player.reset();
    enemies.clear();
    projectiles.clear();

    spawnWave(enemies, player, currentWave);

    gameState = GameState::Playing;
}

void handleEvents(bool& gameIsRunning, Player& player, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, 
    GameState& gameState, int& currentWave, float& waveTimer, int& enemiesKilled)
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
            if(event.key.scancode == SDL_SCANCODE_R && !event.key.repeat)
                resetGame(player, enemies, projectiles, currentWave, gameState, waveTimer, enemiesKilled);

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
                    Projectile projectile(player.getX() + 25.0f, player.getY() + 25.0f, 100.0f, 20.0f,
                        dirX, dirY);
                    projectiles.push_back(projectile);
                }
            }
            
            if(event.key.scancode == SDL_SCANCODE_LSHIFT && gameState == GameState::Playing)
            {
                if(player.attack())
                {
                    Projectile projectile(player.getX() + 25.0f, player.getY() + 25.0f, 300.0f, 40.0f,
                        dirX, dirY);
                    projectiles.push_back(projectile);
                }
            }
        }
    }
}

void renderGame(SDL_Renderer* renderer, Player& player, TTF_Font* font, std::vector<Enemy>& enemies, std::vector<Projectile>& projectiles, 
    GameState& gameState, int& currentWave, float& waveTimer, int& enemiesKilled, int& bestWave)
{
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
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

    renderText(renderer, font, "Wave: " + std::to_string(currentWave), 20.0f, 20.0f);
    renderText(renderer, font, "Enemies Killed: " + std::to_string(enemiesKilled), 20.0f, 50.0f);
    renderText(renderer, font, "Best Wave: " + std::to_string(bestWave), 20.0f, 80.0f);

    if (enemies.empty() && gameState == GameState::Playing)
    {
        int secondsLeft = static_cast<int>(std::ceil(waveTimer));
        renderText(renderer, font, "Next wave in: " + std::to_string(secondsLeft), 20.0f, 105.0f);
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

        renderText(renderer, font, "Game Over", 300, 250);
        renderText(renderer, font, "Last wave: " + std::to_string(currentWave), 300, 270);
        renderText(renderer, font, "Enemies Killed: " + std::to_string(enemiesKilled), 300, 300);
        renderText(renderer, font, "Press R to restart", 300, 330);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
    float deltaTime = 0.0f;
    float waveTimer = 3.0f;
    Player player(350.0f, 250.0f);
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    int currentWave = 0;
    int enemiesKilled = 0;
    int bestWave = 1;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!TTF_Init())
    {
        std::cout << "TTF initialization failed" << SDL_GetError() << std::endl;
        return 1;
        SDL_Quit();
    }

    SDL_Window* window = SDL_CreateWindow("Project Hokkai", 800, 600, 0);

    if (window == nullptr)
    {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    if (renderer == nullptr)
    {
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;

        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("font.ttf", 36);
    if(font == nullptr)
    {
        std::cout << "Font loading failed: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
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

        handleEvents(gameIsRunning, player, enemies, projectiles, gameState, currentWave, waveTimer, enemiesKilled);

        updateGame(player, enemies, projectiles, deltaTime, gameState, currentWave, waveTimer, 
                    enemiesKilled, bestWave);

        renderGame(renderer, player, font, enemies, projectiles, gameState, currentWave, waveTimer, 
                    enemiesKilled, bestWave);

        frameTime = SDL_GetTicksNS() - currentTime;
        if (frameTime < targetFrameTime)
            SDL_DelayPrecise(targetFrameTime - frameTime);
        previousTime = currentTime;
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}