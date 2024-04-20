#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { TRANSITION_1, TRANSITION_2, GAME_WIN, MAIN_MENU, INTRO_UPC, INTRO_BACKGROUND, MENU_PLAY, PLAYING, SETTINGS, CREDITS };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D *img_menu;
    const Texture2D* img_menu_play;
    const Texture2D* img_menu_empty;
    const Texture2D* img_intro_upc;
    const Texture2D* img_intro_background;
    const Texture2D* img_game_win;
    

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};