#include "raylib.h"

int main()
{
    //Window dimensions
    const int windowWidth = 512;
    const int windowHeight = 380;

    //Initialize window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher!");

    // acceleration due to gravity (pixels / frame) / frame
    const int gravity = 1;

    const int jumpVel = -22;

    bool isInAir;

    int velocity = 0;

    Texture2D scarfy = LoadTexture("textures/scarfy.png");

    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;

    Vector2 scarfyPos;
    scarfyPos.x = windowWidth/2 - scarfyRec.width/2;
    scarfyPos.y = windowHeight - scarfyRec.height;

    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // rectangle in on the ground
        if (scarfyPos.y >= windowHeight - scarfyRec.height)
        {
            velocity = 0;
            isInAir = false;
        }
        //  rectangle is in mid-air
        else
        {
            // apply gravity
            velocity += gravity;
            isInAir = true;
        }
        
        if (IsKeyPressed(KEY_SPACE)  && !isInAir)
        {
            velocity += jumpVel;
        }

        //update  position of rectangle based on velocity and time elapsed since last frame
        scarfyPos.y += velocity;

        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        //stop drawing
        EndDrawing();
    }

    UnloadTexture(scarfy);       // Free texture data from memory
    CloseWindow();
}