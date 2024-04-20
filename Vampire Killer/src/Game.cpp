#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::INTRO_UPC;
    scene = nullptr;
    img_menu = nullptr;
    img_menu_play = nullptr;
    img_menu_empty = nullptr;
    img_intro_upc = nullptr;
    img_intro_background = nullptr;
    img_game_win = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Vampire killer");

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "images/Spritesheets/Introduction/Intro Space.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);
    if (data.LoadTexture(Resource::IMG_MENU_PLAY, "images/Spritesheets/Introduction/IntroPlay.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_play = data.GetTexture(Resource::IMG_MENU_PLAY);
    if (data.LoadTexture(Resource::IMG_MENU_EMPTY, "images/Spritesheets/Introduction/Intro Empty.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu_empty = data.GetTexture(Resource::IMG_MENU_EMPTY);
    if (data.LoadTexture(Resource::IMG_INTRO_UPC, "images/Spritesheets/Introduction/IntroUPC.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_intro_upc = data.GetTexture(Resource::IMG_INTRO_UPC);
    if (data.LoadTexture(Resource::IMG_INTRO_BACKGROUND, "images/Spritesheets/Introduction/IntroBackground.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_intro_background = data.GetTexture(Resource::IMG_INTRO_BACKGROUND);
    if (data.LoadTexture(Resource::IMG_GAME_WIN, "images/Spritesheets/Introduction/GameWin.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_game_win = data.GetTexture(Resource::IMG_GAME_WIN);
    
    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;

    switch (state)
    {
        case GameState::INTRO_UPC:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                state = GameState::TRANSITION_1;
            }
            break;
        case GameState::TRANSITION_1:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            WaitTime(0.5);
            state = GameState::INTRO_BACKGROUND;
            break;
        case GameState::INTRO_BACKGROUND:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            WaitTime(2);
            state = GameState::TRANSITION_2;
            break;
        case GameState::TRANSITION_2:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            WaitTime(0.5);
            state = GameState::MAIN_MENU;
            break;
        case GameState::MAIN_MENU: 
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if(BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::PLAYING;
            }
            break;
        case GameState::GAME_WIN:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            WaitTime(5);
            state = GameState::MAIN_MENU;
            break;

        case GameState::PLAYING:  
            if (IsKeyPressed(KEY_ESCAPE))
            {
                FinishPlay();
                state = GameState::MAIN_MENU;
            }
            else if (scene->PlayerIsDead())
            {
                FinishPlay();
                WaitTime(5);
                state = GameState::MAIN_MENU;
            }
            else if (scene->PlayerHasWon())
            {
                FinishPlay();
                WaitTime(1);
                state = GameState::GAME_WIN;
            }
            else
            {
                //Game logic
                scene->Update();
            }
            break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {
        case GameState::MAIN_MENU:
            DrawTexture(*img_menu, 0, 0, WHITE);
            break;

        case GameState::INTRO_UPC:
            DrawTexture(*img_intro_upc, 0, 0, WHITE);
            break;

        case GameState::INTRO_BACKGROUND:
            DrawTexture(*img_intro_background, 0, 0, WHITE);
            
            break;

        case GameState::MENU_PLAY:
            DrawTexture(*img_menu_play, 0, 0, WHITE);
            break;

        case GameState::GAME_WIN:
            DrawTexture(*img_game_win, 0, 0, WHITE);
            break;

        case GameState::PLAYING:
            scene->Render();
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_MENU_PLAY);
    data.ReleaseTexture(Resource::IMG_MENU_EMPTY);
    data.ReleaseTexture(Resource::IMG_INTRO_UPC);
    data.ReleaseTexture(Resource::IMG_INTRO_BACKGROUND);
    data.ReleaseTexture(Resource::IMG_GAME_WIN);

    UnloadRenderTexture(target);
}