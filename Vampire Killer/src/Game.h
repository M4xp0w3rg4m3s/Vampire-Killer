#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState {
    INTRO_UPC, TRANSITION_1,
    INTRO_BACKGROUND, TRANSITION_2,
    INTRODUCTION,
    MAIN_MENU,
    MENU_PLAY,
    PLAYING, 
    GAME_WIN };

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

    const Texture2D* img_introduction;
    const Texture2D* img_intro_bat;
    const Texture2D* img_intro_cloud;
    const Texture2D* img_intro_gui;
    const Texture2D* img_intro_simon;

    int timerWin;
    int timerLose;
    int timerPlay;
    int timerIntroduction;

    int panAnimation;

    float playerAnim;
    

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};