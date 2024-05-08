#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


int main()
{
    //array with Window dimensions
    int windowDimensions [2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    //Initialize window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

    // acceleration due to gravity (pixels/s)/s
    const int gravity = 1'000;

    // jump velocity (pixels/seconds)
    const int jumpVel = -600;

    // is scarfy in the air?
    bool isInAir;

    int velocity = 0;

    // nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    AnimData nebData 
    {
        {0.0, 0.0, nebula.width/8, nebula.height/8}, // Rectangle rec
        {windowDimensions[0], windowDimensions[1] - nebula.height/8}, // Vector2 pos
        0, // int frame
        1.0/12.0, // float updateTime
        0 // float runningTime
    };

    AnimData neb2Data 
    {
        {0.0, 0.0, nebula.width/8, nebula.height/8}, // Rectangle rec
        {windowDimensions[0] + 300, windowDimensions[1] - nebula.height/8}, // Vector2 pos
        0, // int frame
        1.0/16.0, // float updateTime
        0 // float runningTime
    };

    AnimData nebule[2] = {nebData, neb2Data};

    // nebula x velocity (pixels per second)
    int nebVel = -200;

    // scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0; // animation frame
    scarfyData.updateTime = 1.0/12.0; // amount of time before we update the animation frame
    scarfyData.runningTime = 0.0; // how much time has passed since last frame update

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        const float dT = GetFrameTime();

        // scarfy in on the ground
        if (scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height)
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
        nebule[0].pos.x += nebVel * dT;
        nebule[1].pos.x +=  nebVel * dT;

        //update position of scarfy based on velocity and time elapsed since last second
        scarfyData.pos.y += velocity * dT;

        if (!isInAir)
        {
            // update scarfy running time
            scarfyData.runningTime += dT;
            
            //update scarfy animation frame
            if (scarfyData.runningTime >= scarfyData.updateTime)
            {
                scarfyData.runningTime = 0.0;
                
                // change texture for next frame
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                scarfyData.frame++;
                if (scarfyData.frame > 5)
                {
                    scarfyData.frame = 0;
                }
            }
        }

        // update nebula running time
        nebule[0].runningTime += dT;

        // update nebula animation frame
        if (nebule[0].runningTime >= nebule[0].updateTime)
        {
            nebule[0].runningTime = 0.0;

            // change texture for next frame
            nebule[0].rec.x = nebule[0].frame * nebule[0].rec.width;
            nebule[0].frame++;
            if (nebule[0].frame > 7)
            {
                nebule[0].frame = 0;
            }
        }

        // update nebula running time
        nebule[1].runningTime += dT;

        // update 2nd nebula animation frame
        if (nebule[1].runningTime >= nebule[1].updateTime)
        {
            nebule[1].runningTime = 0.0;

            // change texture for next frame
            nebule[1].rec.x = nebule[1].frame * nebule[1].rec.width;
            nebule[1].frame++;
            if (nebule[1].frame > 7)
            {
                nebule[1].frame = 0;
            }
        }

        // draw nebula
        DrawTextureRec(nebula, nebule[0].rec, nebule[0].pos, WHITE);

        // draw 2nd nebula
        DrawTextureRec(nebula, nebule[1].rec, nebule[1].pos, RED);

        // draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        //stop drawing
        EndDrawing();
    }

    // Free texture data from memory
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    
    CloseWindow();
}