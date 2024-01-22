//standard c/c++ headers
#include <list>
#include <iostream>
#include <math.h>
#include <string>
#include <stdio.h>

//raylib headers
#include "raylib.h"




//bredden och höjden på skärmen
const int screenWidth = 1080;
const int screenHeight = 720;

//deltas for drawing
int yShift = 8;
int xShift = 16;

//sprites
Texture2D blockTextures;
Texture2D buildingTextures[2];
Texture2D destroyingTextures[2];

//custom colors
Color skyColor = GetColor(0x02a7c4ff);
Color loadingScreenColor = GetColor(0x327da8ff);

//sound
Music backgroundMusic;
Sound walkingSFX;
Sound buildingSFX;
Sound breakingSFX;

//misc
bool showOutline = false;
Rectangle blockAtlasRectangle = { 0.0f, 0.0f, 16.0f, 16.0f};
int gamestate = 1;
int halfBlocks[4] = {20,21,23,33};
int nonSolidBlocks[7] = {0,27,29,30,31,32,35};

struct {
    float x = 7.0f;
    float y = 7.0f;
    float z = 1.0f;
    float zVelocity = 0;
    int selectedBlock = 1;
    Texture2D texture;
    Texture2D outlineTexture;
    
} Player;

struct {
    int xOffset = 0;
    int yOffset = 0;
    int zOffset = 0;
    int selectedBlock = 0;
    bool placing = true;

} Cursor;

int map[16][16][16] = 
{
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1},
        {1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,2,2,2,2,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    },
    {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
    },
    {
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    },
    {
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    }
};


Color adjustColor(Color c, float shade)
{
    Color newColor;
    newColor.a = c.a;
    newColor.r = c.r * shade;
    newColor.g = c.g * shade;
    newColor.b = c.b * shade;
    return newColor;
}

bool intInArray(int array[], int value, int arrayLength)
{
    for (int i = 0; i < arrayLength; i++)
    {
        if(array[i] == value){return true;}
    }
    return false;
    
}

void saveData()
{
    FILE *fp;
    char block;
    fp = fopen("world.icw", "w");
    putc((char)(int)floor(Player.x), fp);
    putc((char)(int)floor(Player.y), fp);
    putc((char)(int)floor(Player.z), fp);
    putc((char)Player.selectedBlock, fp);
    for (int z = 0; z < 16; z++)
    {
        for (int y = 0; y < 16; y++)
        {
            for (int x = 0; x < 16; x++)
            {
                block = (char)map[z][y][x];
                putc(block, fp);
            }
        }
    }
    fclose(fp);
    
}

void loadData()
{
    FILE *fp;
    int block;
    fp = fopen("world.icw", "r");

    if(fp != NULL)
    {    
        Player.x = getc(fp);
        Player.y = getc(fp);
        Player.z = getc(fp);
        Player.selectedBlock = getc(fp);
        Player.zVelocity = 0.0f;
        for (int z = 0; z < 16; z++)
        {
            for (int y = 0; y < 16; y++)
            {
                for (int x = 0; x < 16; x++)
                {
                    block = getc(fp);
                    if(block == EOF){break;}
                    map[z][y][x] = block;
                }
            }
        }
        fclose(fp);

    }
    
}

void loadAssets()
{
    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);

    EndDrawing();

    WaitTime(1.0);

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading player..", 540 - MeasureText("Loading player..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    WaitTime(0.2);

    //player
    Player.texture = LoadTexture("assets/player.png");
    Player.outlineTexture = LoadTexture("assets/outline.png");

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading misc. sprites..", 540 - MeasureText("Loading misc. sprites..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    WaitTime(0.2);

    //cursor
    buildingTextures[0] = LoadTexture("assets/placing.png");
    destroyingTextures[0] = LoadTexture("assets/destroying.png");

    buildingTextures[1] = LoadTexture("assets/placing1.png");
    destroyingTextures[1] = LoadTexture("assets/destroying1.png");

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading blocks..", 540 - MeasureText("Loading blocks..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    WaitTime(0.2);

    //blocks
    blockTextures = LoadTexture("assets/spritesheet.png");
    //add more textures

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading sounds..", 540 - MeasureText("Loading sounds..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    WaitTime(0.2);

    //sound
    backgroundMusic = LoadMusicStream("assets/crickets.mp3");
    walkingSFX = LoadSound("assets/walking1.wav");
    breakingSFX = LoadSound("assets/pumpkin_break_01.ogg");
    buildingSFX = LoadSound("assets/rock_break.ogg");
    SetSoundVolume(walkingSFX, 0.025f);

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading world..", 540 - MeasureText("Loading world..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    WaitTime(0.2);

    
}

void unloadAssets()
{
    //player
    UnloadTexture(Player.texture);
    UnloadTexture(Player.outlineTexture);

    //cursor
    UnloadTexture(buildingTextures[0]);
    UnloadTexture(destroyingTextures[0]);

    UnloadTexture(buildingTextures[1]);
    UnloadTexture(destroyingTextures[1]);

    //blocks
    UnloadTexture(blockTextures);
    //add more textures

    //sound
    UnloadMusicStream(backgroundMusic);
    UnloadSound(walkingSFX);
    UnloadSound(buildingSFX);
    UnloadSound(breakingSFX);
}

void movePlayer(float deltaX, float deltaY)
{

    float mathX = Player.x + deltaX; 
    float mathY = Player.y + deltaY;

    if(mathX < 0){mathX = 0.0f;}
    if(mathX > 15){mathX = 15.0f;}
    if(mathY < 0){mathY = 0.0f;}
    if(mathY > 15){mathY = 15.0f;}

    if(IsSoundPlaying(walkingSFX) == false)
    {
        PlaySound(walkingSFX);
    }

    if(intInArray(nonSolidBlocks, map[(int)ceil(Player.z) + 1][(int)floor(Player.y)][(int)floor(mathX)], 7) || intInArray(halfBlocks, map[(int)ceil(Player.z) + 1][(int)floor(Player.y)][(int)floor(mathX)], 4))
    {
        Player.x = mathX; 
    }

    if(intInArray(nonSolidBlocks, map[(int)ceil(Player.z) + 1][(int)floor(mathY)][(int)floor(Player.x)], 7) || intInArray(halfBlocks, map[(int)ceil(Player.z) + 1][(int)floor(mathY)][(int)floor(Player.x)], 4))
    {
        Player.y = mathY;
    }

    
    if(intInArray(halfBlocks, map[(int)ceil(Player.z) + 1][(int)floor(Player.y)][(int)floor(mathX)], 4))
    {
        Player.z = ceil(Player.z) + 0.7f;
    }

    return;

}

void adjustCursor()
{
    if(floor(Player.x) + Cursor.xOffset >= 15){Cursor.xOffset -= (floor(Player.x) + Cursor.xOffset) - 15;}
    if(floor(Player.x) + Cursor.xOffset < 0){Cursor.xOffset -= floor(Player.x) + Cursor.xOffset;}
    
    if(floor(Player.y) + Cursor.yOffset > 15){Cursor.yOffset -= (floor(Player.y) + Cursor.yOffset) - 15;}
    if(floor(Player.y) + Cursor.yOffset < 0){Cursor.yOffset -= floor(Player.y) + Cursor.yOffset;}

    if(ceil(Player.z) + Cursor.zOffset > 15){Cursor.zOffset -= (ceil(Player.z) + Cursor.zOffset) - 15;}
    if(ceil(Player.z) + Cursor.zOffset < 0){Cursor.zOffset -= ceil(Player.z) + Cursor.zOffset;}
}

void replaceBlock(int x, int y, int z, int blocktype)
{

    if(x < 0){x = 0;}
    if(x > 15){x = 15;}
    if(y < 0){y = 0;}
    if(y > 15){y = 15;}
    if(z < 0){z = 0;}
    if(z > 15){z = 15;}

    if(intInArray(nonSolidBlocks, map[z][y][x], 7) || blocktype == 0)
    {
        map[z][y][x] = blocktype;
        if(blocktype == 0){PlaySound(breakingSFX);}
        else{PlaySound(buildingSFX);}
    }
}



int main()
{
    
    //öppna ett nytt fönster
    InitWindow(screenWidth, screenHeight, "IsometriCraft");
    InitAudioDevice();
    SetTargetFPS(60);

    loadAssets();
    loadData();

    PlayMusicStream(backgroundMusic);

    //huvudloop
    while (!WindowShouldClose())
    {
        UpdateMusicStream(backgroundMusic);

        
        if(gamestate == 1)
        {
            
            if(IsKeyDown(KEY_A)){movePlayer(-5 * GetFrameTime(), 5 * GetFrameTime());}
            if(IsKeyDown(KEY_D)){movePlayer(5 * GetFrameTime(), -5 * GetFrameTime());}
            if(IsKeyDown(KEY_W)){movePlayer(-5 * GetFrameTime(), -5 * GetFrameTime());}
            if(IsKeyDown(KEY_S)){movePlayer(5 * GetFrameTime(), 5 * GetFrameTime());}

            if(IsKeyPressed(KEY_O)){showOutline = !showOutline;}
            if(IsKeyPressed(KEY_S) && IsKeyDown(KEY_LEFT_CONTROL)){saveData();}
            if(IsKeyPressed(KEY_O) && IsKeyDown(KEY_LEFT_CONTROL)){loadData();}
            if(IsKeyPressed(KEY_Q)){gamestate = 2;}

            if(IsKeyPressed(KEY_E)){replaceBlock(floor(Player.x) + Cursor.xOffset,floor(Player.y) + Cursor.yOffset,ceil(Player.z) + Cursor.zOffset, Player.selectedBlock * (int)Cursor.placing);}
            if(IsKeyPressed(KEY_UP)){Cursor.yOffset -= 1;}
            if(IsKeyPressed(KEY_DOWN)){Cursor.yOffset += 1;}
            if(IsKeyPressed(KEY_LEFT)){Cursor.xOffset -= 1;}
            if(IsKeyPressed(KEY_RIGHT)){Cursor.xOffset += 1;}
            if(IsKeyPressed(KEY_HOME)){Cursor.zOffset += 1;}
            if(IsKeyPressed(KEY_END)){Cursor.zOffset -= 1;}
            if(IsKeyPressed(KEY_RIGHT_CONTROL)){Cursor.placing = !Cursor.placing;}
            adjustCursor();

            
            Player.z += Player.zVelocity * GetFrameTime();
            Player.zVelocity -= 19.64f * GetFrameTime();
            if(intInArray(nonSolidBlocks, map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)], 7) == false)
            {
                if(Player.zVelocity < 0)
                {
                    Player.zVelocity = 0;
                }
                if(Player.zVelocity == 0)
                {
                    Player.z = ceil(Player.z);
                    if(intInArray(halfBlocks, map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)], 4) == true)
                    {
                        Player.z -= 0.3f;
                    }
                    
                    if(IsKeyPressed(KEY_SPACE)){Player.zVelocity = 5.41f;}
                }
            }
            else
            {
                if(IsKeyPressed(KEY_SPACE) && (map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)] == 32 || map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)] == 29)){Player.zVelocity = 5.41f;}
            }

            if(Player.z < 0){Player.z = 0.0f;}
            if(Player.z > 15){Player.z = 15.0f;}
            
            BeginDrawing();
            ClearBackground(RAYWHITE);
            
            //draw background sky and void
            DrawRectangle(0,0,1080,500, skyColor);
            DrawRectangleGradientV(0,500,1080,340,skyColor,BLACK);

            //draw blocks
            Vector2 screenPosition;
            int currentBlockType = 0;

            int x = 0;
            int y = 0;
            
            for(int i = 0; i < 16; i++)
            {
                for(int j = 0; j < (i + 1); j++)
                {

                    for (int z = 0; z < 16; z++)
                    {
                        y = j;
                        x = i - j;
                        currentBlockType = map[z][y][x];
                        blockAtlasRectangle.y = 16 * (currentBlockType - 1);

                        if(currentBlockType != 0)
                        {
                            screenPosition.x = (x - y) * xShift;
                            screenPosition.y = (x + y) * yShift - 15 * z;
                            DrawTexturePro(
                                blockTextures,
                                blockAtlasRectangle,
                                {(float)(412 + screenPosition.x), (float)(232 + screenPosition.y), 32, 32},
                                {0, 0},
                                0,
                                WHITE
                            );
                            
                        }
                        if(z == ceil(Player.z) + Cursor.zOffset && y == floor(Player.y) + Cursor.yOffset && x == floor(Player.x) + Cursor.xOffset)
                        {
                            screenPosition.x = ((floor(Player.x) + Cursor.xOffset) - (floor(Player.y) + Cursor.yOffset)) * xShift - 16;
                            screenPosition.y = ((floor(Player.x) + Cursor.xOffset) + (floor(Player.y) + Cursor.yOffset)) * yShift - 8;
                            screenPosition.y -= 15 * (ceil(Player.z) + Cursor.zOffset);
                            if(Cursor.placing == true)
                            {DrawTextureEx(buildingTextures[(int)(currentBlockType != 0)], {(float)(428 + screenPosition.x), (float)(239 + screenPosition.y)}, 0, 2, WHITE);}
                            else
                            {DrawTextureEx(destroyingTextures[(int)(currentBlockType != 0)], {(float)(428 + screenPosition.x), (float)(239 + screenPosition.y)}, 0, 2, WHITE);}
                        }
                        if(z == ceil(Player.z) && y == floor(Player.y) && x == floor(Player.x))
                        {
                            screenPosition.x = (Player.x - Player.y) * xShift - 16;
                            screenPosition.y = (Player.x + Player.y) * yShift - 32;
                            screenPosition.y -= 15 * (Player.z);
                            DrawTextureEx(Player.texture, {(float)(428 + screenPosition.x), (float)(240 + screenPosition.y)}, 0, 2, WHITE);
                        }
                        
                    }
                }
                
            }
            
                    
            for(int i = 1; i < (16 + 1); i++)
            {
                for(int j = 0; j < (16 - i); j++)
                {
                    for (int z = 0; z < 16; z++)
                    {
                        y = i + j;
                        x = 16 - j - 1;
                        currentBlockType = map[z][y][x];
                        blockAtlasRectangle.y = 16 * (currentBlockType - 1);

                        if(currentBlockType != 0)
                        {
                            screenPosition.x = (x - y) * xShift;
                            screenPosition.y = (x + y) * yShift - 15 * z;
                            
                            DrawTexturePro(
                                blockTextures,
                                blockAtlasRectangle,
                                {(float)(412 + screenPosition.x), (float)(232 + screenPosition.y), 32, 32},
                                {0, 0},
                                0,
                                WHITE
                            );
                        }
                        if(z == ceil(Player.z) + Cursor.zOffset && y == floor(Player.y) + Cursor.yOffset && x == floor(Player.x) + Cursor.xOffset)
                        {
                            screenPosition.x = ((floor(Player.x) + Cursor.xOffset) - (floor(Player.y) + Cursor.yOffset)) * xShift - 16;
                            screenPosition.y = ((floor(Player.x) + Cursor.xOffset) + (floor(Player.y) + Cursor.yOffset)) * yShift - 8;
                            screenPosition.y -= 15 * (ceil(Player.z) + Cursor.zOffset);
                            if(Cursor.placing == true)
                            {DrawTextureEx(buildingTextures[(int)(currentBlockType != 0)], {(float)(428 + screenPosition.x), (float)(239 + screenPosition.y)}, 0, 2, WHITE);}
                            else
                            {DrawTextureEx(destroyingTextures[(int)(currentBlockType != 0)], {(float)(428 + screenPosition.x), (float)(239 + screenPosition.y)}, 0, 2, WHITE);}
                        }
                        if(z == ceil(Player.z) && y == floor(Player.y) && x == floor(Player.x))
                        {
                            screenPosition.x = (Player.x - Player.y) * xShift - 16;
                            screenPosition.y = (Player.x + Player.y) * yShift - 32;
                            screenPosition.y -= 15 * (Player.z);
                            DrawTextureEx(Player.texture, {(float)(428 + screenPosition.x), (float)(240 + screenPosition.y)}, 0, 2, WHITE);
                        }
                    }
                }
                
            }

                
            
            screenPosition.x = (Player.x - Player.y) * xShift - 16;
            screenPosition.y = (Player.x + Player.y) * yShift - 32;
            screenPosition.y -= 15 * (Player.z);

            if(showOutline == true)
            {
                DrawTextureEx(Player.outlineTexture, {(float)(428 + screenPosition.x), (float)(240 + screenPosition.y)}, 0, 2, WHITE);
            }
            
            std::string printText = "X: " + std::to_string(Player.x);
            DrawText(printText.c_str(), 10, 30, 24, WHITE);
            printText = "Y: " + std::to_string(Player.y);
            DrawText(printText.c_str(), 10, 50, 24, WHITE);
            printText = "Z: " + std::to_string(Player.z);
            DrawText(printText.c_str(), 10, 70, 24, WHITE);
            printText = "Z velocity: " + std::to_string(Player.zVelocity);
            DrawText(printText.c_str(), 10, 90, 24, WHITE);

            
            DrawFPS(10, 10);
            EndDrawing();
        }
        else if(gamestate == 2)
        {
            if(IsKeyPressed(KEY_Q)){gamestate = 1;}
            BeginDrawing();
            ClearBackground(adjustColor(DARKGRAY, 0.5f));

            Vector2 mousePos = GetMousePosition();
            for(int y = 0; y < 5; y++)
            {
                for(int x = 0; x < 7; x++)
                {
                    blockAtlasRectangle.y = 16 * (x + y*7);
                    if((x + y*7) < 35)
                    {
                        DrawTexturePro(
                            blockTextures,
                            blockAtlasRectangle,
                            {(float)(64 + x * 144), (float)(64 + y * 104), 96, 96},
                            {0, 0},
                            0,
                            WHITE
                        );
                        if((x + y*7) + 1 == Player.selectedBlock)
                        {
                            DrawRectangleLines(64 + x * 144, 64 + y * 104, 96, 96, YELLOW);
                        }
                    }

                }
            }

            mousePos.x -= 64;
            mousePos.y -= 64;

            mousePos.x = floor(mousePos.x/144);
            mousePos.y = floor(mousePos.y/104);

            

            if(mousePos.x >= 0 && mousePos.x < 7 && mousePos.y >= 0 && mousePos.y < 5)
            {
                DrawRectangleLines(64 + mousePos.x * 144, 64 + mousePos.y * 104, 96, 96, WHITE);
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){Player.selectedBlock = (mousePos.x + mousePos.y*7) + 1;}
            }

            DrawFPS(10, 10);
            EndDrawing();
        }

    }

    unloadAssets();

    CloseWindow();
    
}

