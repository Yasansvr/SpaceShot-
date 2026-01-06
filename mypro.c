#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

typedef enum GameScreen {
    SCREEN_MENU = 0,
    SCREEN_GAME,
    SCREEN_PAUSE,
    SCREEN_SETTINGS,
    SCREEN_GAMEOVER
} GameScreen;

void initials(){
    
}

int main(void)
{
    int score= 0;
    int limit = 10;
    int highscore = 0;
    int health = 100;
    const int screenWidth = 800;
    const int screenHeight = 539;
    InitWindow(screenWidth, screenHeight, "SpaceShot");
    InitAudioDevice();
    Texture2D background = LoadTexture("assets/space.png");
    Texture2D rocket = LoadTexture("assets/rocket2.png");
    Texture2D bullet = LoadTexture("assets/bullet1.png");
    Texture2D enemy = LoadTexture("assets/enemy1.png");
    Texture2D explo = LoadTexture("assets/explosion.png");
    Music background_music = LoadMusicStream("assets/music1.mp3");
    Sound damage = LoadSound("assets/damage.mp3");
    SetTargetFPS(60);
    HideCursor();
    PlayMusicStream(background_music);
    Vector2 bluePos = { 0 };
    bool blueActive = false;
    float blueSpeed = 700.0f; 
    Vector2 redpos = {redpos.x, 150};
    bool redActive = true;
    float redSpeed = 400.0f;
    Vector2 exploo;
    float explosionTimer = 0.25f;
    float dt = GetFrameTime();
    bool getexpo = false;
    bool coll = false;
    bool mute = false;
    bool gameover = false;
    float gotext = 0.25f;
   

    Rectangle rect = {200,50,400,12};

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();
        float dt = GetFrameTime();
        UpdateMusicStream(background_music);
        SetSoundVolume(damage, 0.40f);
        

        if (IsKeyPressed(KEY_M) && mute == false)
        {
            mute = true;
            StopMusicStream(background_music);
        }       
        else if (IsKeyPressed(KEY_M) && mute == true)
        {
            PlayMusicStream(background_music);
            mute = false;
        }   

        


        // RELEASE: spawn blue circle
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            DrawRectangle(mouse.x -20 , mouse.y-20 , 40,40,DARKBLUE);
            bluePos = mouse;
            blueActive = true;
        }

        // UPDATE BLUE CIRCLE
        if (blueActive)
        {
            bluePos.y -= blueSpeed * dt;
            if (bluePos.y < -20) blueActive = false;
        }

        // UPDATE RED BALL (falling)
        if (redActive)
        {
            redpos.y += redSpeed * dt;
            if (redpos.y > 600)   // bottom of screen
            {
                limit--;
                redpos.x =  rand() % (800 + 1);
                redpos.y = -20;   // respawn at top
            }
        }

        if (bluePos.x-12<= redpos.x+16 && bluePos.x+12 >= redpos.x-16 && bluePos.y-12 <= redpos.y +16 && bluePos.y+12 >= redpos.y-16)
        {
            score+=1;
            redpos.y = -20; 
            redpos.x =  16 + rand() % (16 - 720 + 1);
            exploo = bluePos;
            blueActive = false;
            getexpo = true;
            explosionTimer = 0.0f;
            PlaySound(damage);
            redSpeed = redSpeed + 5;
        }

        if (mouse.x-12<= redpos.x+16 && mouse.x+12 >= redpos.x-16 && mouse.y-12 <= redpos.y +16 && mouse.y+12 >= redpos.y-16)
        {
            health -= 10;
            redpos.y = -20; 
            redpos.x =  16 + rand() % (16 - 720 + 1);
            rect.width -= 40;
            
        }

        if (limit == 0 )
        {
            highscore = score;
            limit = 10;
            score = 0;
            redSpeed = 400.0f;
        }

        if (health == 0)
        {
            rect.width = 400;
            health = 100;
            gameover =true;
            
        }
        

        BeginDrawing();
        
        ClearBackground(DARKGRAY);
        DrawTexture(background, 0, 0, WHITE);
        DrawTextEx(GetFontDefault(),TextFormat("Score : %d",score),(Vector2){20,20},24,3,RED );
        DrawTextEx(GetFontDefault(),TextFormat("Fault : %d",limit),(Vector2){650,20},24,3,RED );
        DrawTextEx(GetFontDefault(),TextFormat("HighScore : %d",highscore),(Vector2){310,20},24,3,RED );
        DrawRectangleLines(199,49,403,15,WHITE); //200 50 400 12
        DrawRectangle(rect.x , rect.y , rect.width,rect.height,GREEN);
        DrawTextEx(GetFontDefault(),TextFormat("HP : %d",health),(Vector2){650,50},24,3,RED );
        DrawTextEx(GetFontDefault(),"MUTE/UNMUTE : M",(Vector2){20,519},12,3,WHITE);

        if (blueActive)
            DrawTexture(bullet,bluePos.x - 18 , bluePos.y -18,WHITE);

        if (redActive)
            DrawTexture(enemy,redpos.x - 18 , redpos.y -18,WHITE);

        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            DrawTexture(rocket,mouse.x-40, mouse.y-40 ,WHITE);

        if (getexpo)
        {
            DrawTexture(explo,exploo.x-40,exploo.y-40,WHITE);
            explosionTimer += dt;
            if (explosionTimer >= 0.2f)
                getexpo = false;
        } 


        if (gameover)
        {
           DrawText("GAME OVER",230,235,50,RED);
            gotext += dt;
            if (gotext >= 2.2f)
            {
                gameover = false;
            }
            
        }
        
        EndDrawing();
    }   
    StopMusicStream(background_music);
	UnloadMusicStream(background_music);
    UnloadTexture(background);
    CloseWindow();
    return 0;
}
