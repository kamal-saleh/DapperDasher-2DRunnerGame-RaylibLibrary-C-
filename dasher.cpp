#include "raylib.h"

int main()
{
    //Window dimensions
    const int windowWidth = 512;
    const int windowHeight = 380;

    //Initialize window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher!");

    // acceleration due to gravity (pixels/s)/s
    const int gravity = 1'000;

    // jump velocity (pixels/seconds)
    const int jumpVel = -600;

    // is scarfy in the air?
    bool isInAir;

    int velocity = 0;

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebRec {0.0, 0.0, nebula.width/8, nebula.height/8};
    Vector2 nebPos {windowWidth, windowHeight - nebRec.height};

    // nebula x velocity (pixels per second)
    int nebVel = -200;

    // nebula animation variables
    int nebframe;
    const float nebUpdateTime = 1.0/12.0;
    float nebRunningTime;

    // scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = windowWidth/2 - scarfyRec.width/2;
    scarfyPos.y = windowHeight - scarfyRec.height;

    // animation frame
    int frame;
    // amount of time before we update the animation frame
    const float updateTime = 1.0/12.0;
    // how much time has passed since last frame update
    float runningTime  = 0.0;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        const float dT = GetFrameTime();

        // scarfy in on the ground
        if (scarfyPos.y >= windowHeight - scarfyRec.height)
        {
            velocity = 0;
            isInAir = false;
        }
        // scarfy is in mid-air
        else
        {
            // apply gravity
            velocity += gravity * dT;
            isInAir = true;
        }
        
        if (IsKeyPressed(KEY_SPACE)  && !isInAir)
        {
            velocity += jumpVel;
        }

        //update position of nebula based on velocity and time elapsed since last second
        nebPos.x += nebVel * dT;

        //update position of scarfy based on velocity and time elapsed since last second
        scarfyPos.y += velocity * dT;

        if (!isInAir)
        {
            // update scarfy running time
            runningTime += dT;
            
            //update scarfy animation frame
            if (runningTime >= updateTime)
            {
                runningTime = 0.0;
                
                // change texture for next frame
                scarfyRec.x = frame * scarfyRec.width;
                frame++;
                if (frame > 5)
                {
                    frame = 0;
                }
            }
        }

        // update nebula running time
        nebRunningTime += dT;

        // update nebula animation frame
        if (nebRunningTime >= nebUpdateTime)
        {
            nebRunningTime = 0.0;

            // change texture for next frame
            nebRec.x = nebframe * nebRec.width;
            nebframe++;
            if (nebframe > 7)
            {
                nebframe = 0;
            }
        }

        // draw nebula
        DrawTextureRec(nebula, nebRec, nebPos, WHITE);

        // draw scarfy
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        //stop drawing
        EndDrawing();
    }

    // Free texture data from memory
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    
    CloseWindow();
}