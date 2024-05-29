#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData UpdateAnimationData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;

    // update animation frame
    if (data.runningTime > data.updateTime)
    {
        data.runningTime = 0.0;

        // change texture for next frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }

    return data;
}

int main()
{
    // array with Window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    // Initialize window
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

    const int sizeOfNebulae = 6;
    AnimData nebulae[sizeOfNebulae];

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0.0;
        nebulae[i].updateTime = 1.0 / 12.0;
        nebulae[i].runningTime = 0.0;
    }

    float finishLine = nebulae[sizeOfNebulae - 1].pos.x;

    // nebula x velocity (pixels per second)
    int nebVel = -200;

    // scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;               // animation frame
    scarfyData.updateTime = 1.0 / 12.0; // amount of time before we update the animation frame
    scarfyData.runningTime = 0.0;       // how much time has passed since last frame update

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX = 0.0;
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX = 0.0;
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX = 0.0;

    bool collision = false;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float dT = GetFrameTime();

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // scroll the background
        bgX -= 100 * dT;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }

        // scroll the midground
        mgX -= 40 * dT;
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }

        // scroll the foreground
        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        // draw the background
        Vector2 bgPos{bgX, 0.0};
        DrawTextureEx(background, bgPos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width *2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // draw the background
        Vector2 mgPos{mgX, 0.0};
        DrawTextureEx(midground, mgPos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width *2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // draw the background
        Vector2 fgPos{fgX, 0.0};
        DrawTextureEx(foreground, fgPos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width *2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // scarfy in on the ground
        if (isOnGround(scarfyData, windowDimensions[1]))
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

        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        // update position of nebulea
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebVel * dT;
        }

        // update finish line position
        finishLine += nebVel * dT;

        // update position of scarfy based on velocity and time elapsed since last second
        scarfyData.pos.y += velocity * dT;

        if (!isInAir)
        {
            scarfyData = UpdateAnimationData(scarfyData, dT, 5);
        }

        for (AnimData nebula : nebulae)
        {
            float pad = 50;
            Rectangle nebRec{
                nebula.pos. x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - pad,
                nebula.rec.height - pad
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = UpdateAnimationData(nebulae[i], dT, 7);

            // draw nebula
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }

        if (!collision)
        {
            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
        

        // stop drawing
        EndDrawing();
    }

    // Free texture data from memory
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}