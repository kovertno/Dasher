#include <raylib.h>

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

void animationUpdate(AnimData *data, float dT, int frames)
{
    data->runningTime += dT;
    if(data->runningTime >= data->updateTime)
    {
        data->runningTime = 0;
        data->rec.x = data->frame * data->rec.width;
        data->frame++;
        if(data->frame > frames)
            data->frame = 0; 
    }
}

int main()
{
    //window dimensions
    const int windowWidth{750};
    const int windowHeight{550};

    //initialize the window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    //acceleration due to gravity (pixels/second)/second
    const int gravity{1000};

    //nebula variables
    Texture2D nebula{LoadTexture("textures/12_nebula_spritesheet.png")};

    const int sizeOfNebulae = 15;
    AnimData nebulae[sizeOfNebulae]{};

    for(int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = (float)nebula.width/8;
        nebulae[i].rec.height = (float)nebula.height/8;
        nebulae[i].pos.y = windowHeight - (float)nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
        nebulae[i].pos.x = windowWidth + (i+1) * 500; 
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    //DrawRectangle()

    //nebula velocity(pixels/second)
    const int nebVel{-300};

    //scarfy variables
    Texture2D scarfy{LoadTexture("textures/scarfy.png")};
    AnimData scarfyData
    {
        //rectangle rec
        {0, 0, (float)scarfy.width/6, (float)scarfy.height},
        //vector2 pos
        {(float)(windowWidth/2 - scarfy.width/6/2), (float)(windowHeight - scarfy.height)},
        //int frame
        0,
        //float updateTime
        1.0/12.0,
        //float runningTime
        0.0
    };

    //jump velovity (pixels/second)
    const int jumpVel{600};

    int velocity{0};

    //background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    bool collision{};

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        const float dT{GetFrameTime()};

        //backgound movement(frames/second)
        bgX -= 20 * dT;

        if(bgX <= -background.width*3)
        {
           bgX = 0.0;
        }

        //draw the background
        Vector2 bg1Pos{bgX, 0.0};
        Vector2 bg2Pos{bgX + (float)background.width*3, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 3.0, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, 3.0, WHITE);

        //midground movement (frames/second)
        mgX -= 40 * dT;

        if(mgX <= - midground.width*3)
        {
            mgX = 0;
        }

        //draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        Vector2 mg2Pos{mgX + (float)midground.width*3, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 3.0, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, 3.0, WHITE);

        //foreground movement (frames/second)
        fgX -= 60 * dT;

        if(fgX <= - foreground.width*3)
        {
            fgX = 0;
        }

        //draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        Vector2 fg2Pos{fgX + (float)foreground.width*3, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 3.0, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, 3.0, WHITE);

        //delta time (time since last frame)

        //ground check
        if(isOnGround(scarfyData, windowHeight))
        {
            velocity = 0;
        }
        else
        {
            velocity += gravity*dT;
        }

        if(IsKeyPressed(KEY_SPACE) && isOnGround(scarfyData, windowHeight))
        {
            
            velocity -= jumpVel;
        }

        //Scarfy position update
        scarfyData.pos.y += velocity*dT; 

        //nebula position updates
        for(int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebVel * dT;
        }

        //update finish line
        finishLine += nebVel * dT;

        for(AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec
            {
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };

            Rectangle scarfyRec
            {
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if(CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

       
        if(collision)
        {
            DrawText("YOU LOSE", 120, windowHeight/2 - 100, 100, WHITE);
        }
        else if(finishLine <= 0)
        {
            DrawText("YOU WIN", 140, windowHeight/2 - 100, 100, WHITE);
        }
        else
        {
            //draw nebula
            for(int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            //draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
        
        if(isOnGround(scarfyData, windowHeight))
        {
            //update animation frame scarfy
            animationUpdate(&scarfyData, dT, 5);
        }

        //upadte animation frame nebula
        for(int i = 0; i < sizeOfNebulae; i++)
        {
            animationUpdate(&nebulae[i], dT, 7);
        }

        //stop drawing
        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
    return 0;
}