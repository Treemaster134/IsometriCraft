//standard c/c++ headers
#include <list>
#include <iostream>
#include <math.h>
#include <string>
#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

namespace filesys = std::filesystem;

//raylib headers
#include "raylib.h"


const int MAX_MAP_X = 36;
const int MAX_MAP_Y = 36;
const int MAX_MAP_Z = 20;


//bredden och höjden på skärmen
const int screenWidth = 1080;
const int screenHeight = 720;

//deltas for drawing

const int drawSize = 24;

const int yShift = drawSize/4;
const int xShift = drawSize/2;

const int drawCenterX = (screenWidth/2.0f) - (drawSize/2.0f);


//sprites
Texture2D blockTextures;
Texture2D buildingTextures[2];
Texture2D destroyingTextures[2];
Texture2D shadowBelowPlayerTexture;
Texture2D shadowBelowBlockTexture;
Texture2D tileBg;
Texture2D buttonTexture;
Texture2D worldErrorTexture;
Texture2D biomeTextures[4];
Texture2D liquidAtlas;

Texture2D waterBucketTexture;
Texture2D lavaBucketTexture;
Texture2D pickaxeTexture;


//custom colors
Color skyColor = GetColor(0x02a7c4ff);
Color hellSkyColor = GetColor(0x8f0f00ff);
Color loadingScreenColor = GetColor(0x327da8ff);

//sound
Music backgroundMusic;
Sound walkingSFX;
Sound buildingSFX;
Sound breakingSFX;


//misc
const int halfBlockCount = 5;
const int nonSolidBlockCount = 10;
const int totalBlockCount = 68;

const int maxLiquidLevel = 8;
const int maxLiquidNonSourceLevel = 7;

bool showOutline = false;
bool isGameRunning = true;
Rectangle blockAtlasRectangle = { 0.0f, 0.0f, 16.0f, 16.0f};
Rectangle liquidRectangle = {0.0f,0.0f,16.0f,16.0f};
int gamestate = 0;

enum BLOCK_IDS{
    Air = 0,
    Bedrock = 1,
    Grass = 2,
    Cobblestone = 3,
    Workbench = 4,
    Leaves = 5,
    Wood = 6,
    Stone = 7,
    Glass = 8,
    Sand = 9,
    Dirt = 10,
    Planks = 11,
    Iron_Ore = 12,
    Furnace = 13,
    Chest = 14,
    Diamond_Ore = 15,
    Gold_Ore = 16,
    Gold_Block = 17,
    Diamond_Block = 18,
    Iron_Block = 19,
    Wood_slab = 20,
    Cobblestone_Slab = 21,
    Gravel = 22,
    Stone_Slab = 23,
    Brick = 24,
    Cactus = 25,
    Bookshelf = 26,
    Door = 27,
    Mossy_Cobblestone = 28,
    Water = 29,
    Tree = 30,
    Flower = 31,
    Lava = 32,
    Bed = 33,
    Dynamite = 34,
    Torch = 35,

    White_Wool = 36,
    Grey_Wool = 37,
    Black_Wool = 38,
    Red_Wool = 39,
    Orange_Wool = 40,
    Yellow_Wool = 41,
    Light_Green_Wool = 42,
    Green_Wool = 43,
    Teal_Wool = 44,
    Light_Blue_Wool = 45,
    Blue_Wool = 46,
    Purple_Wool = 47,
    Magenta_Wool = 48,
    Pink_Wool = 49,
    Brown_Wool = 50,
    Obsidian = 51,
    Stone_Bricks = 52,
    Brown_Bricks_In_Minecraft = 53,
    Yellow_Flower = 54,
    Dead_Bush = 55,
    Nether_Reactor = 56,
    Fire = 57,
    Command_Block = 58,
    Beacon = 59,
    Magma_Block = 60,
    Netherrack = 61,
    Cake = 62,

    Sandstone = 63,
    Snowy_Grass = 64,
    Melon = 65,
    Pumpkin = 66,
    Carved_Pumpkin = 67,
    Ice = 68
};

int halfBlocks[halfBlockCount] = {
    BLOCK_IDS::Wood_slab,
    BLOCK_IDS::Cobblestone_Slab,
    BLOCK_IDS::Stone_Slab,
    BLOCK_IDS::Bed, 
    BLOCK_IDS::Cake
};

int nonSolidBlocks[nonSolidBlockCount] = {
    BLOCK_IDS::Air,
    BLOCK_IDS::Door,
    BLOCK_IDS::Water,
    BLOCK_IDS::Tree,
    BLOCK_IDS::Flower,
    BLOCK_IDS::Lava,
    BLOCK_IDS::Torch,
    BLOCK_IDS::Yellow_Flower,
    BLOCK_IDS::Dead_Bush,
    BLOCK_IDS::Fire
};

int movingDirection = 0;
int activeworldtype = 0;

std::string blockNames[totalBlockCount] = {
    "Bedrock",
    "Grass",
    "Cobblestone",
    "Workbench",
    "Leaves",
    "Wood",
    "Stone",
    "Glass",
    "Sand",
    "Dirt",
    "Planks",
    "Iron Ore",
    "Furnace",
    "Chest",
    "Diamond Ore",
    "Gold Ore",
    "Gold Block",
    "Diamond Block",
    "Iron Block",
    "Wood slab",
    "Cobblestone Slab",
    "Gravel",
    "Stone Slab",
    "Brick",
    "Cactus",
    "Bookshelf",
    "Door",
    "Mossy Cobblestone",
    "Water",
    "Tree",
    "Flower",
    "Lava",
    "Bed",
    "Dynamite",
    "Torch",

    "White Wool",
    "Grey Wool",
    "Black Wool",
    "Red Wool",
    "Orange Wool",
    "Yellow Wool",
    "Light Green Wool",
    "Green Wool",
    "Teal Wool",
    "Light Blue Wool",
    "Blue Wool",
    "Purple Wool",
    "Magenta Wool",
    "Pink Wool",
    "Brown Wool",
    "Obsidian",
    "Stone Bricks",
    "Brown Bricks In Minecraft",
    "Yellow Flower",
    "Dead Bush",
    "Nether Reactor",
    "Fire",
    "Command_Block",
    "Beacon",
    "Magma_Block",
    "Netherrack",
    "Cake",

    "Sandstone",
    "Snowy Grass",
    "Melon",
    "Pumpkin",
    "Carved Pumpkin",
    "Ice"
};

struct SaveFileNameStruct{
    std::string name;
    std::string path;
    std::string modifyTime;
    int worldType;

    SaveFileNameStruct(std::string _p, std::string _t, int _w)
    {
        path = _p;
        int startIndex = _p.find('\\') + 1;
        name = _p.substr(startIndex, _p.length() - (startIndex + 4));

        std::string year = _t.substr(_t.length() - 5, 4);
        std::string month = _t.substr(4, 3);
        std::string day = _t.substr(8, 2);
        std::string timeOfDay = _t.substr(12, 8);


        modifyTime = year + "/" + month + "/" + day + "  " + timeOfDay;

        worldType = _w;
    }
};

std::vector<SaveFileNameStruct> saveFilenames;

std::string splashnames[11] = {
    "Randomized splash!",
    "erm... what the heck?",
    "Try the buzzfeed quiz!",
    "100\% awesome sauce",
    "Congratulations!",
    "Random worlds!",
    "HELP HELP HELP HELP",
    "177013",
    ":3 :3 x3",
    "1337",
    "Peter what are you doing"
};

//commands
bool gravityOn = true;
bool liquidFlowOn = true;
bool blockGravityOn = true;

//main stuff
enum{
    MAIN_MENU = 0,
    NEW_WORLD = 1,
    IN_GAME = 2,
    BLOCK_MENU = 3,
    LOAD_MENU = 4,
    SAVE_MENU = 5,
    COMMAND_MENU = 6
};

struct {
    float x = 7.0f;
    float y = 7.0f;
    float z = 1.0f;
    float zVelocity = 0;
    int selectedBlock = 1;
    Texture2D textures[3];
    Texture2D outlineTextures[2];
    
} Player;

struct {
    int xOffset = 0;
    int yOffset = 0;
    int zOffset = 0;
    int selectedBlock = 0;
    bool placing = true;

} Cursor;

struct Block{
    int blockId = 0;
    int blockData = 0;

    Block(int _id, int _data)
    {
        blockId = _id;
        blockData = _data;
    }

    Block()
    {
        blockId = 0;
        blockData = 0;
    }
};

Block map[MAX_MAP_Z][MAX_MAP_Y][MAX_MAP_X];


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

bool vector2InStdVector(std::vector<Vector2> &sv, Vector2 v)
{
    if(sv.empty() == true){return false;}
    for (int i = 0; i < sv.size(); i++)
    {
        if (sv[i].x == v.x && sv[i].y == v.y)
        {
            return true;
        }
        
    }

    return false;
    
}

void saveData(std::string filename)
{
    FILE *fp;
    char block;
    fp = fopen(filename.c_str(), "w");
    putc((char)activeworldtype, fp);
    putc((char)(int)floor(Player.x), fp);
    putc((char)(int)floor(Player.y), fp);
    putc((char)(int)floor(Player.z), fp);
    putc((char)Player.selectedBlock, fp);
    for (int z = 0; z < MAX_MAP_Z; z++)
    {
        for (int y = 0; y < MAX_MAP_Y; y++)
        {
            for (int x = 0; x < MAX_MAP_X; x++)
            {
                block = (char)map[z][y][x].blockId;
                putc(block, fp);
                block = (char)map[z][y][x].blockData;
                putc(block, fp);
            }
        }
    }
    fclose(fp);
    
}

void loadData(std::string filename)
{
    FILE *fp;
    int block;
    int blockdata;
    fp = fopen(filename.c_str(), "r");

    if(fp != NULL)
    {    
        activeworldtype = getc(fp);
        Player.x = getc(fp);
        Player.y = getc(fp);
        Player.z = getc(fp);
        Player.selectedBlock = getc(fp);
        Player.zVelocity = 0.0f;
        for (int z = 0; z < MAX_MAP_Z; z++)
        {
            for (int y = 0; y < MAX_MAP_Y; y++)
            {
                for (int x = 0; x < MAX_MAP_X; x++)
                {
                    block = getc(fp);
                    if(block == EOF){break;}
                    blockdata = getc(fp);
                    if(block == EOF){break;}

                    map[z][y][x] = Block(block, blockdata);
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

    //WaitTime(1.0);

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading player..", 540 - MeasureText("Loading player..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    //WaitTime(0.2);

    //player
    Player.textures[0] = LoadTexture("assets/entities/player.png");
    Player.outlineTextures[0] = LoadTexture("assets/entities/outline.png");

    Player.textures[1] = LoadTexture("assets/entities/sidewaysplayer.png");
    Player.outlineTextures[1] = LoadTexture("assets/entities/sidewaysoutline.png");

    Player.textures[2] = LoadTexture("assets/entities/movingupplayer.png");

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading misc. sprites..", 540 - MeasureText("Loading misc. sprites..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    //WaitTime(0.2);

    //cursor
    buildingTextures[0] = LoadTexture("assets/terrain/placing.png");
    destroyingTextures[0] = LoadTexture("assets/terrain/destroying.png");

    buildingTextures[1] = LoadTexture("assets/terrain/placing1.png");
    destroyingTextures[1] = LoadTexture("assets/terrain/destroying1.png");

    shadowBelowPlayerTexture = LoadTexture("assets/entities/shadow_below_player.png");
    shadowBelowBlockTexture = LoadTexture("assets/terrain/shadow_below_block.png");

    tileBg = LoadTexture("assets/menus/tilebg.png");

    buttonTexture = LoadTexture("assets/menus/button3.png");

    worldErrorTexture = LoadTexture("assets/menus/loadBiomeError.png");

    biomeTextures[0] = LoadTexture("assets/menus/regularBiome.png");
    biomeTextures[1] = LoadTexture("assets/menus/hellBiome.png");
    biomeTextures[2] = LoadTexture("assets/menus/flatBiome.png");
    biomeTextures[3] = LoadTexture("assets/menus/parkourBiome.png");

    waterBucketTexture = LoadTexture("assets/menus/waterbucket.png");
    lavaBucketTexture = LoadTexture("assets/menus/lavabucket.png");
    pickaxeTexture = LoadTexture("assets/menus/pickaxe.png");

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading blocks..", 540 - MeasureText("Loading blocks..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    //WaitTime(0.2);

    //blocks
    blockTextures = LoadTexture("assets/terrain/spritesheet2.png");
    
    liquidAtlas = LoadTexture("assets/terrain/liquids.png");
    //add more textures

    BeginDrawing();

    ClearBackground(loadingScreenColor);
    DrawText("IsometriCraft", 540 - MeasureText("IsometriCraft", 72)/2, 340, 72, WHITE);
    DrawText("Loading sounds..", 540 - MeasureText("Loading sounds..", 36)/2, 400, 36, WHITE);

    EndDrawing();

    //WaitTime(0.2);

    //sound
    backgroundMusic = LoadMusicStream("assets/sounds/crickets.mp3");
    walkingSFX = LoadSound("assets/sounds/walking1.wav");
    breakingSFX = LoadSound("assets/sounds/pumpkin_break_01.ogg");
    buildingSFX = LoadSound("assets/sounds/rock_break.ogg");
    SetSoundVolume(walkingSFX, 0.025f);

    //WaitTime(0.2);

    
}

void loadFileNames()
{
    saveFilenames.clear();

    for (const auto & file : filesys::directory_iterator("saves"))
    {
        std::string p = file.path().string();
        if(p[p.length() - 3] == 'i' && p[p.length() - 2] == 'c' && p[p.length() - 1] == 'w')
        {
            
            time_t modifyTime = GetFileModTime(p.c_str());
            // Convert the time to a human-readable format
            struct tm *localTime = localtime(&modifyTime);

            int worldType = -1;

            FILE *fp;
            int block;
            fp = fopen(p.c_str(), "r");

            if(fp != NULL)
            {    
                worldType = getc(fp);
                fclose(fp);
            }


            saveFilenames.push_back(SaveFileNameStruct(p,(std::string)asctime(localTime), worldType));
        }
    }
    
}

void unloadAssets()
{
    //player
    UnloadTexture(Player.textures[0]);
    UnloadTexture(Player.outlineTextures[0]);

    UnloadTexture(Player.textures[1]);
    UnloadTexture(Player.outlineTextures[1]);

    UnloadTexture(Player.textures[2]);

    //cursor
    UnloadTexture(buildingTextures[0]);
    UnloadTexture(destroyingTextures[0]);

    UnloadTexture(buildingTextures[1]);
    UnloadTexture(destroyingTextures[1]);

    UnloadTexture(tileBg);

    UnloadTexture(buttonTexture);

    UnloadTexture(worldErrorTexture);
    UnloadTexture(biomeTextures[0]);
    UnloadTexture(biomeTextures[1]);
    UnloadTexture(biomeTextures[2]);
    UnloadTexture(biomeTextures[3]);

    UnloadTexture(waterBucketTexture);
    UnloadTexture(lavaBucketTexture);
    UnloadTexture(pickaxeTexture);

    //blocks
    UnloadTexture(blockTextures);
    UnloadTexture(liquidAtlas);
    //add more textures
    UnloadTexture(shadowBelowPlayerTexture);
    UnloadTexture(shadowBelowBlockTexture);

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
    if(mathX > MAX_MAP_X - 1){mathX = MAX_MAP_X - 1;}
    if(mathY < 0){mathY = 0.0f;}
    if(mathY > MAX_MAP_Y - 1){mathY = MAX_MAP_Y - 1;}

    if(IsSoundPlaying(walkingSFX) == false)
    {
        PlaySound(walkingSFX);
    }

    if(intInArray(nonSolidBlocks, map[(int)ceil(Player.z) + 1][(int)floor(Player.y)][(int)floor(mathX)].blockId, nonSolidBlockCount) || intInArray(halfBlocks, map[(int)ceil(Player.z) + 1][(int)floor(Player.y)][(int)floor(mathX)].blockId, halfBlockCount))
    {
        Player.x = mathX; 
    }

    if(intInArray(nonSolidBlocks, map[(int)ceil(Player.z) + 1][(int)floor(mathY)][(int)floor(Player.x)].blockId, nonSolidBlockCount) || intInArray(halfBlocks, map[(int)ceil(Player.z) + 1][(int)floor(mathY)][(int)floor(Player.x)].blockId, halfBlockCount))
    {
        Player.y = mathY;
    }

    
    if(intInArray(halfBlocks, map[(int)ceil(Player.z) + 1][(int)floor(Player.y)][(int)floor(mathX)].blockId, halfBlockCount))
    {
        Player.z = ceil(Player.z) + 0.7f;
    }

    return;

}

void adjustCursor()
{
    if(floor(Player.x) + Cursor.xOffset >= MAX_MAP_X - 1){Cursor.xOffset -= (floor(Player.x) + Cursor.xOffset) - (MAX_MAP_X - 1);}
    if(floor(Player.x) + Cursor.xOffset < 0){Cursor.xOffset -= floor(Player.x) + Cursor.xOffset;}
    
    if(floor(Player.y) + Cursor.yOffset > MAX_MAP_Y - 1){Cursor.yOffset -= (floor(Player.y) + Cursor.yOffset) - (MAX_MAP_Y - 1);}
    if(floor(Player.y) + Cursor.yOffset < 0){Cursor.yOffset -= floor(Player.y) + Cursor.yOffset;}

    if(ceil(Player.z) + Cursor.zOffset > MAX_MAP_Z - 1){Cursor.zOffset -= (ceil(Player.z) + Cursor.zOffset) - (MAX_MAP_Z - 1);}
    if(ceil(Player.z) + Cursor.zOffset < 0){Cursor.zOffset -= ceil(Player.z) + Cursor.zOffset;}
}

int randomInt(int min, int max)
{
	int randrange = max-min + 1;
	return rand()%randrange + min;
}

Vector3 getBlockBelowPlayer()
{

    for (int i = ceil(Player.z); i > 0; i--)
    {
        if(map[i][(int)floor(Player.y)][(int)floor(Player.x)].blockId != 0)
        {
            return {Player.x,Player.y,i};
        }
    }
    


    return {0.0f,0.0f,0.0f};
}

std::vector<std::string> splitString(std::string oldString)
{
    std::vector<std::string> newString = {};
    for(int i = 0; i < oldString.length(); i += 2)
    {
        newString.push_back(oldString.substr(i, 2));
    }
    
    return newString;
}

bool parseCommand(std::string cmd)
{
    std::vector splitCommand = splitString(cmd);
    int cmdSize = splitCommand.size();
    bool isCommandValid = false;

    if(cmdSize < 0){return isCommandValid;}

    switch (stoi(splitCommand[0], 0, 16))
    {
    case 0:
    {
        if(cmdSize != 2){break;}

        if(stoi(splitCommand[1], 0, 16) == 0)
        {gravityOn = false;}
        else
        {gravityOn = true;}
        isCommandValid = true;
        break;
    }
    case 1:
    {
        if(cmdSize != 8){break;}

        Vector3 minPosition = {
            (float)stoi(splitCommand[1],0,16),
            (float)stoi(splitCommand[2],0,16),
            (float)stoi(splitCommand[3],0,16)
        };

        Vector3 maxPosition = {
            (float)stoi(splitCommand[4],0,16),
            (float)stoi(splitCommand[5],0,16),
            (float)stoi(splitCommand[6],0,16)
        };

        int blockToPlace = stoi(splitCommand[7],0,16);

        for (int z = 0; z < MAX_MAP_Z; z++)
        {
            for (int y = 0; y < MAX_MAP_Y; y++)
            {
                for (int x = 0; x < MAX_MAP_X; x++)
                {
                    if(x >= (int)minPosition.x && x <= (int)maxPosition.x &&
                       y >= (int)minPosition.y && y <= (int)maxPosition.y &&
                       z >= (int)minPosition.z && z <= (int)maxPosition.z
                    )
                    {
                        if(blockToPlace == BLOCK_IDS::Water || blockToPlace == BLOCK_IDS::Lava)
                        {
                            map[z][y][x] = Block(blockToPlace,maxLiquidLevel);
                        }
                        else
                        {
                            map[z][y][x] = Block(blockToPlace,0);
                        }
                    }
                }
            }
            
        }
        
        isCommandValid = true;
        break;
    }
    case 2:
    {
        if(cmdSize != 4){break;}
        Player.x = (float)stoi(splitCommand[1],0,16);
        Player.y = (float)stoi(splitCommand[2],0,16);
        Player.z = (float)stoi(splitCommand[3],0,16);
        isCommandValid = true;
        break;
    }
    case 3:
    {
        if(cmdSize != 2){break;}

        if(stoi(splitCommand[1], 0, 16) == 0)
        {liquidFlowOn = false;}
        else
        {liquidFlowOn = true;}

        isCommandValid = true;
        break;
    }
    case 4:
    {
        if(cmdSize != 2){break;}

        if(stoi(splitCommand[1], 0, 16) == 0)
        {blockGravityOn = false;}
        else
        {blockGravityOn = true;}

        isCommandValid = true;
        break;
    }
    case 5:
    {
        if(cmdSize != 2){break;}

        activeworldtype = stoi(splitCommand[1], 0, 16);

        isCommandValid = true;
        break;
    }
    case 6:
    {
        if(cmdSize != 1){break;}

        Player.selectedBlock = randomInt(1,totalBlockCount);

        isCommandValid = true;
        break;
    }
    default:
        break;
    }

    return isCommandValid;
}

bool isLiquidTouchingSource(int z, int y, int x)
{
    if(map[z][y][x].blockData == maxLiquidLevel){return true;}
    
    bool touching = false;
    if(map[z][y][x].blockId == BLOCK_IDS::Water || map[z][y][x].blockId == BLOCK_IDS::Lava)
    {

        if(x != 0)
        {
            if(map[z][y][x - 1].blockId == map[z][y][x].blockId && map[z][y][x - 1].blockData > map[z][y][x].blockData)
            {
                touching = true;
            }
        }
        if(x != MAX_MAP_X - 1 && touching == false)
        {
            if(map[z][y][x + 1].blockId == map[z][y][x].blockId && map[z][y][x + 1].blockData > map[z][y][x].blockData)
            {
                touching = true;
            }
        }

        if(y != 0 && touching == false)
        {
            if(map[z][y - 1][x].blockId == map[z][y][x].blockId && map[z][y - 1][x].blockData > map[z][y][x].blockData)
            {
                touching = true;
            }
        }
        if(y != MAX_MAP_Y - 1 && touching == false)
        {
            if(map[z][y + 1][x].blockId == map[z][y][x].blockId && map[z][y + 1][x].blockData > map[z][y][x].blockData)
            {
                touching = true;
            }
        }

        if(z != MAX_MAP_Z - 1 && touching == false)
        {
            if(map[z+1][y][x].blockId == map[z][y][x].blockId)
            {
                touching = true;
            }
        }
    }

    return touching;
}

void liquidFlow()
{

    std::vector<Vector3> liquidCoords = {};

    for (int z = 0; z < MAX_MAP_Z; z++)
    {
        for (int y = 0; y < MAX_MAP_Y; y++)
        {
            for (int x = 0; x < MAX_MAP_X; x++)
            {
                if(z != 0)
                {
                    if(map[z][y][x].blockId == BLOCK_IDS::Lava && map[z][y][x].blockData > 1 && map[z - 1][y][x].blockId != BLOCK_IDS::Lava && map[z - 1][y][x].blockId != BLOCK_IDS::Air)
                    {
                        if(isLiquidTouchingSource(z,y,x) == true)
                        {
                            liquidCoords.push_back((Vector3){x,y,z});
                        }
                    }
                    else if(map[z][y][x].blockId == BLOCK_IDS::Water && map[z][y][x].blockData > 1 && map[z - 1][y][x].blockId != BLOCK_IDS::Water && map[z - 1][y][x].blockId != BLOCK_IDS::Air)
                    {
                        if(isLiquidTouchingSource(z,y,x) == true)
                        {
                            liquidCoords.push_back((Vector3){x,y,z});
                        }      
                    }


                }

                if(z != MAX_MAP_Z - 1)
                {
                    //std::cout << "WHAT\n";
                    if(map[z + 1][y][x].blockId == BLOCK_IDS::Water && map[z][y][x].blockId == BLOCK_IDS::Air)
                    {
                        map[z][y][x] = Block(BLOCK_IDS::Water,maxLiquidNonSourceLevel);
                    } 
                    else if(map[z + 1][y][x].blockId == BLOCK_IDS::Lava && map[z][y][x].blockId == BLOCK_IDS::Air)
                    {
                        map[z][y][x] = Block(BLOCK_IDS::Lava,maxLiquidNonSourceLevel);
                    }

                    else if(map[z + 1][y][x].blockId == BLOCK_IDS::Water && map[z][y][x].blockId == BLOCK_IDS::Water)
                    {
                        if(map[z][y][x].blockData < map[z + 1][y][x].blockData - 1)
                        {
                            map[z][y][x] = Block(BLOCK_IDS::Water,maxLiquidNonSourceLevel);
                        }
                    } 
                    else if(map[z + 1][y][x].blockId == BLOCK_IDS::Lava && map[z][y][x].blockId == BLOCK_IDS::Lava)
                    {
                        if(map[z][y][x].blockData < map[z + 1][y][x].blockData - 1)
                        {
                            map[z][y][x] = Block(BLOCK_IDS::Lava,maxLiquidNonSourceLevel);
                        }
                    } 


                    

                }
                
            }
        }
    }
    
    if(liquidCoords.empty() == false)
    {
        for (int i = 0; i < (int)liquidCoords.size(); i++)//map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x]
        {
            
            if((int)liquidCoords[i].x != 0)
            {
                if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x - 1].blockId == BLOCK_IDS::Air)
                {
                    map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x - 1] = Block(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId,map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1); 
                }
                else if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x - 1].blockId == map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId)
                {
                    if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x - 1].blockData < map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1)
                    {
                        map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x - 1] = Block(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId,map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1); 
                    }
                }

            }
            if((int)liquidCoords[i].x != MAX_MAP_X - 1)
            {
                if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x + 1].blockId == BLOCK_IDS::Air)
                {
                    map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x + 1] = Block(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId,map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1); 
                }
                else if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x + 1].blockId == map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId)
                {
                    if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x + 1].blockData < map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1)
                    {
                        map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x + 1] = Block(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId,map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1); 
                    }
                }
            }

            if((int)liquidCoords[i].y != 0)
            {
                if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y - 1][(int)liquidCoords[i].x].blockId == BLOCK_IDS::Air)
                {
                    map[(int)liquidCoords[i].z][(int)liquidCoords[i].y - 1][(int)liquidCoords[i].x] = Block(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId,map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1); 
                }
                else if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y - 1][(int)liquidCoords[i].x].blockId == map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId)
                {
                    if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y - 1][(int)liquidCoords[i].x].blockData < map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1)
                    {
                        map[(int)liquidCoords[i].z][(int)liquidCoords[i].y - 1][(int)liquidCoords[i].x] = Block(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId,map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1); 
                    }
                }
            }
            if((int)liquidCoords[i].y != MAX_MAP_Y - 1)
            {
                if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y + 1][(int)liquidCoords[i].x].blockId == BLOCK_IDS::Air)
                {
                    map[(int)liquidCoords[i].z][(int)liquidCoords[i].y + 1][(int)liquidCoords[i].x] = Block(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId,map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1); 
                }
                else if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y + 1][(int)liquidCoords[i].x].blockId == map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId)
                {
                    if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y + 1][(int)liquidCoords[i].x].blockData < map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1)
                    {
                        map[(int)liquidCoords[i].z][(int)liquidCoords[i].y + 1][(int)liquidCoords[i].x] = Block(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId,map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockData - 1); 
                    }
                }
            }

            if(liquidCoords[i].z != 0)
            {
                if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId == BLOCK_IDS::Lava && map[(int)liquidCoords[i].z - 1][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId == BLOCK_IDS::Water)
                {
                    map[(int)liquidCoords[i].z - 1][(int)liquidCoords[i].y][(int)liquidCoords[i].x] = Block(BLOCK_IDS::Cobblestone,0);
                }
                else if(map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId == BLOCK_IDS::Water && map[(int)liquidCoords[i].z - 1][(int)liquidCoords[i].y][(int)liquidCoords[i].x].blockId == BLOCK_IDS::Lava)
                {
                    map[(int)liquidCoords[i].z - 1][(int)liquidCoords[i].y][(int)liquidCoords[i].x] = Block(BLOCK_IDS::Cobblestone,0);
                }
            }
        

        }
        
    }

    
    liquidCoords.clear();

    for (int z = 0; z < MAX_MAP_Z; z++)
    {
        for (int y = 0; y < MAX_MAP_Y; y++)
        {
            for (int x = 0; x < MAX_MAP_X; x++)
            {
                if((map[z][y][x].blockId == BLOCK_IDS::Water || map[z][y][x].blockId == BLOCK_IDS::Lava) && map[z][y][x].blockData != maxLiquidLevel)
                {
                    if(isLiquidTouchingSource(z,y,x) == false)
                    {
                        liquidCoords.push_back((Vector3){x,y,z});
                    }
                }
            }
        }
    }
    
    
    if(liquidCoords.empty() == false)
    {
        for (int i = 0; i < (int)liquidCoords.size(); i++)//map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x]
        {
            map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x] = Block(0,0);
        }
    }
    
}

void gravityBlocksFall()
{

    std::vector<Vector3> sandCoords = {};

    for (int z = 0; z < MAX_MAP_Z; z++)
    {
        for (int y = 0; y < MAX_MAP_Y; y++)
        {
            for (int x = 0; x < MAX_MAP_X; x++)
            {
                if(z != 0)
                {
                    
                    if(map[z][y][x].blockId == BLOCK_IDS::Sand || map[z][y][x].blockId == BLOCK_IDS::Gravel)
                    {
                        if(map[z - 1][y][x].blockId == BLOCK_IDS::Air || map[z - 1][y][x].blockId == BLOCK_IDS::Water || map[z - 1][y][x].blockId == BLOCK_IDS::Lava)
                        {
                            sandCoords.push_back((Vector3){x,y,z});
                        }
                        
                    }


                }
                
            }
        }
    }
    
    if(sandCoords.empty() == false)
    {
        for (size_t i = 0; i < sandCoords.size(); i++)//map[(int)liquidCoords[i].z][(int)liquidCoords[i].y][(int)liquidCoords[i].x]
        {
            if(map[(int)sandCoords[i].z][(int)sandCoords[i].y][(int)sandCoords[i].x].blockId == BLOCK_IDS::Sand || map[(int)sandCoords[i].z][(int)sandCoords[i].y][(int)sandCoords[i].x].blockId == BLOCK_IDS::Gravel)
            {
                map[(int)sandCoords[i].z - 1][(int)sandCoords[i].y][(int)sandCoords[i].x] = Block(map[(int)sandCoords[i].z][(int)sandCoords[i].y][(int)sandCoords[i].x].blockId,0);
                map[(int)sandCoords[i].z][(int)sandCoords[i].y][(int)sandCoords[i].x] = Block(BLOCK_IDS::Air,0);   
            }

        }
        
    }
}

void WorldGen(int worldType)
{
    gamestate = IN_GAME;
    activeworldtype = worldType;
    std::cout << worldType << "\n";

    
    
    for (int z = 0; z < MAX_MAP_Z; z++)
    {
        for (int y = 0; y < MAX_MAP_Y; y++)
        {
            for (int x = 0; x < MAX_MAP_X; x++)
            {
                if(z != 0)
                {
                    map[z][y][x] = Block(BLOCK_IDS::Air,0);
                }
                else
                {
                    map[z][y][x] = Block(BLOCK_IDS::Bedrock,0);
                }
            }
        }
    }

    Player.x = MAX_MAP_X/2.0f;
    Player.y = MAX_MAP_Y/2.0f;
    Player.z = MAX_MAP_Z - 2;


    int currentHeight = 0; 

    int groundBlock = 0;
    int liquidBlock = 0;
    int seaFloorBlock = 0;
    int topBlock = 0;

    /*
    
    world types:

    1 regular: vanlig gen med vatten när z <= 5
    2 hell: som regular fast med dirt och lava istället för gräs och vatten
    3 flat: 
    4 parkour
    5 Snow 

    */

    if(worldType == 1 || worldType == 2 || worldType == 5)
    {
        Image pelrinnojs = GenImagePerlinNoise(MAX_MAP_X,MAX_MAP_Y,randomInt(1,100000),randomInt(1,100000),1.0f);
        if(worldType == 1)
        {
            groundBlock = BLOCK_IDS::Dirt;
            liquidBlock = BLOCK_IDS::Water;
            seaFloorBlock = BLOCK_IDS::Sand;
            topBlock = BLOCK_IDS::Grass;
        }
        else if(worldType == 2)
        {
            groundBlock = BLOCK_IDS::Netherrack;
            liquidBlock = BLOCK_IDS::Lava;
            seaFloorBlock = BLOCK_IDS::Magma_Block;
            topBlock = BLOCK_IDS::Netherrack;
        }
        else if(worldType == 5)
        {
            groundBlock = BLOCK_IDS::Dirt;
            liquidBlock = BLOCK_IDS::Water;
            seaFloorBlock = BLOCK_IDS::Gravel;
            topBlock = BLOCK_IDS::Snowy_Grass;
        }

        for (size_t y = 0; y < MAX_MAP_Y; y++)
        {
            for (size_t x = 0; x < MAX_MAP_X; x++)
            {
                Color c = GetImageColor(pelrinnojs,x,y);
                
                currentHeight = round((c.r/255.0f) * 12);

                for (int z = 0; z < currentHeight; z++)
                {
                    if(z == currentHeight - 1)
                    {
                        bool isNextToWater = false;

                        if(currentHeight <= 5)//map[z + 1][y + 1][x] == 29 || map[z + 1][y][x + 1] == 29 ||)
                        {
                            map[z + 1][y][x] = Block(seaFloorBlock,0);
                        }
                        else
                        {
                            map[z + 1][y][x] = Block(topBlock,0);
                        }


                    }
                    else
                    {
                        map[z + 1][y][x] = Block(groundBlock,0);
                    }
                }

                if(currentHeight <= 5)
                {
                    for (int z = currentHeight; z <= 5; z++)
                    {
                        if(worldType == 5 && z == 5)
                        {
                            map[z + 1][y][x] = Block(BLOCK_IDS::Ice,0);
                        }
                        else
                        {
                            map[z + 1][y][x] = Block(liquidBlock,maxLiquidLevel);
                        }
                    }
                }
                

            }
        }
        UnloadImage(pelrinnojs);
    }
    else if(worldType == 3)
    {
        for (int z = 0; z < MAX_MAP_Z; z++)
        {
            for (int y = 0; y < MAX_MAP_Y; y++)
            {
                for (int x = 0; x < MAX_MAP_X; x++)
                {
                    if(z == 0)
                    {
                        map[z][y][x] = Block(BLOCK_IDS::Bedrock,0);
                    }
                    else if(z == 1)
                    {
                        map[z][y][x] = Block(BLOCK_IDS::Grass,0);
                    }
                    else
                    {
                        map[z][y][x] = Block(BLOCK_IDS::Air,0);
                    }
                }
            }
        }
    }
    else if(worldType == 4)
    {
        for (int z = 0; z < MAX_MAP_Z; z++)
        {
            for (int y = 0; y < MAX_MAP_Y; y++)
            {
                for (int x = 0; x < MAX_MAP_X; x++)
                {
                    if(z == 1 && y % 2 == 0 && x % 2 == 0)
                    {
                        map[z][y][x] = Block(BLOCK_IDS::Grass,0);
                    }
                    else
                    {
                        map[z][y][x] = Block(BLOCK_IDS::Air,0);
                    }
                }
            }
        }
    }
    

    return;
}

Color fadeColor(Color c, float f)
{
    Color newColor = c;
    newColor.a = c.a * f;
    return newColor;
}

Color AverageColor(Color c1, Color c2)
{
    Color newColor;
    newColor.a = c1.a/2 + c2.a/2;
    newColor.r = c1.r/2 + c2.r/2;
    newColor.g = c1.g/2 + c2.g/2;
    newColor.b = c1.b/2 + c2.b/2;
    return newColor;
}

bool Button(int x, int y, int w, int h, Texture2D tex, Color tint, std::string text, Vector2 mousePosition, int fontSize)
{
    bool clicked = false;
    if(mousePosition.x > x && mousePosition.x < x+w && mousePosition.y > y && mousePosition.y < y+h)
    {
        DrawTextureNPatch(tex, (NPatchInfo){{0.0f,0.0f,(float)tex.width,(float)tex.height}, 17,17,17,17,NPATCH_NINE_PATCH}, (Rectangle){x,y,w,h}, {0.0f,0.0f}, 0.0f, tint);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){clicked = true;}
    }
    else
    {
        DrawTextureNPatch(tex, (NPatchInfo){{0.0f,0.0f,(float)tex.width,(float)tex.height}, 17,17,17,17,NPATCH_NINE_PATCH}, (Rectangle){x,y,w,h}, {0.0f,0.0f}, 0.0f, WHITE);
    }

    DrawText(text.c_str(), x + w/2 - MeasureText(text.c_str(), fontSize)/2, y + h/2 - MeasureTextEx(GetFontDefault(), text.c_str(), fontSize,fontSize/10.0f).y/2,fontSize,WHITE);

    return clicked;
}

bool isAnotherBlockCastingAShadowOntoThisBlock(Vector3 position, int bType)
{

    if(position.z == MAX_MAP_Z - 1){return false;}
    if (map[(int)position.z + 1][(int)position.y][(int)position.x].blockId != 0){return false;}
    

    for (int i = (int)position.z + 1; i < MAX_MAP_Z; i++)
    {
        if(map[i][(int)position.y][(int)position.x].blockId != 0)
        {
            return true;
        }
    }

    if((int)position.x != 0)
    {
        if(map[(int)position.z + 1][(int)position.y][(int)position.x - 1].blockId != 0 && intInArray(nonSolidBlocks, map[(int)position.z + 1][(int)position.y][(int)position.x - 1].blockId, nonSolidBlockCount) == false)
        {
            return true;
        }
    }

    if((int)position.y != 0)
    {
        if(map[(int)position.z + 1][(int)position.y - 1][(int)position.x].blockId != 0 && intInArray(nonSolidBlocks, map[(int)position.z + 1][(int)position.y - 1][(int)position.x].blockId, nonSolidBlockCount) == false)
        {
            return true;
        }
    }
    

    return false;
}

void replaceBlock(int x, int y, int z, int blocktype)
{

    if(x < 0){x = 0;}
    if(x > MAX_MAP_X - 1){x = MAX_MAP_X - 1;}
    if(y < 0){y = 0;}
    if(y > MAX_MAP_Y - 1){y = MAX_MAP_Y - 1;}
    if(z < 0){z = 0;}
    if(z > MAX_MAP_Z - 1){z = MAX_MAP_Z - 1;}

    if(intInArray(nonSolidBlocks, map[z][y][x].blockId, nonSolidBlockCount) || blocktype == BLOCK_IDS::Air)
    {

        if(blocktype == BLOCK_IDS::Water || blocktype == BLOCK_IDS::Lava)
        {
            map[z][y][x] = Block(blocktype,maxLiquidLevel);
        }
        else if(map[z][y][x].blockId == BLOCK_IDS::Ice)
        {
            map[z][y][x] = Block(BLOCK_IDS::Water,maxLiquidLevel);
        }
        else
        {
            map[z][y][x] = Block(blocktype,0);
        }

        if(blocktype == BLOCK_IDS::Air){PlaySound(breakingSFX);}
        else{PlaySound(buildingSFX);}
    }
}

void drawTheWorld(int startHeight, int endHeight)
{
    Vector2 screenPosition;
    int currentBlockType = 0;

    int x = 0;
    int y = 0;
    
    for(int i = 0; i < MAX_MAP_X; i++)
    {
        for(int j = 0; j < (i + 1); j++)
        {

            for (int z = startHeight; z < endHeight; z++)
            {
                y = j;
                x = i - j;
                currentBlockType = map[z][y][x].blockId;
                int drawingblockType = currentBlockType;
                if(drawingblockType > 35){drawingblockType -= 35; blockAtlasRectangle.x = 16.0f;}
                else{blockAtlasRectangle.x = 0.0f;}
                
                blockAtlasRectangle.y = 16 * (drawingblockType - 1);
                
                if(currentBlockType == BLOCK_IDS::Water || currentBlockType == BLOCK_IDS::Lava && map[z][y][x].blockData > 0)
                {
                    int liquidHeight = map[z][y][x].blockData;
                    if(liquidHeight == maxLiquidNonSourceLevel){liquidHeight = maxLiquidLevel;}

                    liquidRectangle.x = 16 * (int)(currentBlockType == BLOCK_IDS::Lava);
                    liquidRectangle.y = (maxLiquidLevel - liquidHeight) * 16;
                    screenPosition.x = (x - y) * xShift;
                    screenPosition.y = (x + y) * yShift - ((xShift - 1) * z);
                    DrawTexturePro(
                        liquidAtlas,
                        liquidRectangle,
                        {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y), drawSize, drawSize},
                        {0, 0},
                        0,
                        WHITE
                    );
                }
                else if(currentBlockType != 0)
                {
                    screenPosition.x = (x - y) * xShift;
                    screenPosition.y = (x + y) * yShift - ((xShift - 1) * z);
                    DrawTexturePro(
                        blockTextures,
                        blockAtlasRectangle,
                        {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y), drawSize, drawSize},
                        {0, 0},
                        0,
                        WHITE
                    );

                    if(isAnotherBlockCastingAShadowOntoThisBlock({(float)x,(float)y,(float)z}, currentBlockType) && intInArray(nonSolidBlocks,currentBlockType, nonSolidBlockCount) == false && intInArray(halfBlocks,currentBlockType, halfBlockCount) == false)
                    {
                        DrawTexturePro(
                            shadowBelowBlockTexture, 
                            {0.0f,0.0f,(float)shadowBelowBlockTexture.width,(float)shadowBelowBlockTexture.height},
                            {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y), drawSize, drawSize},
                            {0, 0},
                            0,
                            WHITE
                        );
                    }
                    
                }
                if(z == ceil(Player.z) + Cursor.zOffset && y == floor(Player.y) + Cursor.yOffset && x == floor(Player.x) + Cursor.xOffset)
                {
                    screenPosition.x = ((floor(Player.x) + Cursor.xOffset) - (floor(Player.y) + Cursor.yOffset)) * xShift;// - xShift;
                    screenPosition.y = ((floor(Player.x) + Cursor.xOffset) + (floor(Player.y) + Cursor.yOffset)) * yShift;// - yShift;
                    screenPosition.y -= (xShift - 1) * (ceil(Player.z) + Cursor.zOffset);
                    if(Cursor.placing == true)
                    {DrawTextureEx(buildingTextures[(int)(currentBlockType != 0)], {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y)}, 0, drawSize/16.0f, WHITE);}
                    else
                    {DrawTextureEx(destroyingTextures[(int)(currentBlockType != 0)], {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y)}, 0, drawSize/16.0f, WHITE);}
                }

                

                if(z == ceil(Player.z) + 1 && y == floor(Player.y) && x == floor(Player.x))
                {
                    screenPosition.x = (Player.x - Player.y) * xShift;// - (xShift);
                    screenPosition.y = (Player.x + Player.y) * yShift - (yShift * 3);
                    screenPosition.y -= (xShift - 1) * (Player.z);
                    DrawTextureEx(Player.textures[movingDirection], {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y)}, 0, drawSize/16.0f, WHITE);
                }
                
            }
        }
        
    }
                
    for(int i = 1; i < (MAX_MAP_X + 1); i++)
    {
        for(int j = 0; j < (MAX_MAP_X - i); j++)
        {
            for (int z = startHeight; z < endHeight; z++)
            {
                y = i + j;
                x = MAX_MAP_X - j - 1;
                currentBlockType = map[z][y][x].blockId;
                int drawingblockType = currentBlockType;
                if(drawingblockType > 35){drawingblockType -= 35; blockAtlasRectangle.x = 16.0f;}
                else{blockAtlasRectangle.x = 0.0f;}
                blockAtlasRectangle.y = 16 * (drawingblockType - 1);

                if(currentBlockType == BLOCK_IDS::Water || currentBlockType == BLOCK_IDS::Lava && map[z][y][x].blockData > 0)
                {
                    int liquidHeight = map[z][y][x].blockData;
                    if(liquidHeight == maxLiquidNonSourceLevel){liquidHeight = maxLiquidLevel;}

                    liquidRectangle.x = 16 * (int)(currentBlockType == BLOCK_IDS::Lava);
                    liquidRectangle.y = (maxLiquidLevel - liquidHeight) * 16;
                    screenPosition.x = (x - y) * xShift;
                    screenPosition.y = (x + y) * yShift - ((xShift - 1) * z);
                    DrawTexturePro(
                        liquidAtlas,
                        liquidRectangle,
                        {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y), drawSize, drawSize},
                        {0, 0},
                        0,
                        WHITE
                    );
                }
                else if(currentBlockType != 0)
                {
                    screenPosition.x = (x - y) * xShift;
                    screenPosition.y = (x + y) * yShift - (xShift - 1) * z;
                    
                    DrawTexturePro(
                        blockTextures,
                        blockAtlasRectangle,
                        {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y), drawSize, drawSize},
                        {0, 0},
                        0,
                        WHITE
                    );

                    if(isAnotherBlockCastingAShadowOntoThisBlock({(float)x,(float)y,(float)z}, currentBlockType) && intInArray(nonSolidBlocks,currentBlockType, nonSolidBlockCount) == false && intInArray(halfBlocks,currentBlockType, halfBlockCount) == false)
                    {
                        DrawTexturePro(
                            shadowBelowBlockTexture, 
                            {0.0f,0.0f,(float)shadowBelowBlockTexture.width,(float)shadowBelowBlockTexture.height},
                            {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y), drawSize, drawSize},
                            {0, 0},
                            0,
                            WHITE
                        );
                    }
                }
                if(z == ceil(Player.z) + Cursor.zOffset && y == floor(Player.y) + Cursor.yOffset && x == floor(Player.x) + Cursor.xOffset)
                {
                    screenPosition.x = ((floor(Player.x) + Cursor.xOffset) - (floor(Player.y) + Cursor.yOffset)) * xShift;// - xShift;
                    screenPosition.y = ((floor(Player.x) + Cursor.xOffset) + (floor(Player.y) + Cursor.yOffset)) * yShift;// - yShift;
                    screenPosition.y -= (xShift - 1) * (ceil(Player.z) + Cursor.zOffset);
                    if(Cursor.placing == true)
                    {DrawTextureEx(buildingTextures[(int)(currentBlockType != 0)], {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y)}, 0, drawSize/16.0f, WHITE);}
                    else
                    {DrawTextureEx(destroyingTextures[(int)(currentBlockType != 0)], {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y)}, 0, drawSize/16.0f, WHITE);}
                }

                

                if(z == ceil(Player.z) + 1 && y == floor(Player.y) && x == floor(Player.x))
                {
                    screenPosition.x = (Player.x - Player.y) * xShift;// - xShift;
                    screenPosition.y = (Player.x + Player.y) * yShift - (yShift * 3);
                    screenPosition.y -= (xShift - 1) * (Player.z);
                    DrawTextureEx(Player.textures[movingDirection], {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y)}, 0, drawSize/16.0f, WHITE);
                }
            }
        }
        
    }

    //DrawRectangle(screenWidth - 114, 40, 74, 74, fadeColor(BLACK, 0.6f));
    DrawTexturePro(buttonTexture, {0,0,(float)buttonTexture.width,(float)buttonTexture.height}, {screenWidth - 119, 35, 84,84}, {0,0}, 0.0f,WHITE);

    int drawingblockType = Player.selectedBlock;

    if(Cursor.placing == false)
    {
        DrawTexturePro(
            pickaxeTexture,
            {0,0,16,16},
            {(float)(screenWidth - 109), 45.0f, 64, 64},
            {0, 0},
            0,
            WHITE
        );
    }
    else if(drawingblockType == BLOCK_IDS::Water)
    {
        DrawTexturePro(
            waterBucketTexture,
            {0,0,16,16},
            {(float)(screenWidth - 109), 45.0f, 64, 64},
            {0, 0},
            0,
            WHITE
        );
    }
    else if(drawingblockType == BLOCK_IDS::Lava)
    {
        DrawTexturePro(
            lavaBucketTexture,
            {0,0,16,16},
            {(float)(screenWidth - 109), 45.0f, 64, 64},
            {0, 0},
            0,
            WHITE
        );
    }
    else
    {
        if(drawingblockType > 35){drawingblockType -= 35; blockAtlasRectangle.x = 16.0f;}
        else{blockAtlasRectangle.x = 0.0f;}
        blockAtlasRectangle.y = 16 * (drawingblockType - 1);
        DrawTexturePro(
            blockTextures,
            blockAtlasRectangle,
            {(float)(screenWidth - 109), 45.0f, 64, 64},
            {0, 0},
            0,
            WHITE
        );
    }

}

int main()
{
    
    //öppna ett nytt fönster
    InitWindow(screenWidth, screenHeight, "IsometriCraft");
    InitAudioDevice();
    SetTargetFPS(60);

    loadAssets();
    //loadData();

    PlayMusicStream(backgroundMusic);

    int frames = 0;
    int selectedWorldType = 1;
    int selectedSave = -1;
    std::string commandEntry = "";
    bool validCommandEntry = true;

    srand(time(NULL));

    int randomSplash = randomInt(1,11);
    Vector2 playerVelocity = {0.0f,0.0f};

    //huvudloop
    while (isGameRunning == true && !WindowShouldClose())
    {
        UpdateMusicStream(backgroundMusic);

        frames += 1;
        
        Vector2 mousePos = GetMousePosition();

        if (gamestate == MAIN_MENU)
        {
            BeginDrawing();
            ClearBackground(BLACK);

            for (int x = 0; x < 34; x++)
            {
                for (int y = 0; y < 17; y++)
                {
                    DrawTextureEx(tileBg,{32.0f * x,44.0f * y},0,2,WHITE);
                }
            }
            


            DrawText("IsometriCraft", screenWidth/2 - MeasureText("IsometriCraft", 96)/2, 128, 96, WHITE);

            /*DrawText("PRESS ENTER",
            screenWidth/2.0f - MeasureText("PRESS ENTER", 64)/2,
            600,
            64,
            fadeColor(WHITE,abs(cos(2.3f * frames * PI/180)))
            );
            */

            if(Button(screenWidth/2 - 250, 300, 500, 75, buttonTexture, GREEN, "New World", mousePos, 48))
            {
                gamestate = NEW_WORLD;
                selectedWorldType = 1;
            }

            if(Button(screenWidth/2 - 250, 395, 500, 75, buttonTexture, GREEN, "Load World", mousePos, 48))
            {
                loadFileNames();
                gamestate = LOAD_MENU;
                selectedSave = -1;
            }

            if(Button(screenWidth/2 - 250, 490, 500, 75, buttonTexture, GREEN, "Quit", mousePos, 48))
            {
                isGameRunning = false;
            }

            DrawTextPro(GetFontDefault(), splashnames[randomSplash - 1].c_str(), 
                {(float)screenWidth/2 + MeasureText("IsometriCraft", 96)/2, 128 + MeasureTextEx(GetFontDefault(), "IsometriCraft", 96, 9.6f).y}, 
                {(float)MeasureText(splashnames[randomSplash - 1].c_str(), 32)/2, MeasureTextEx(GetFontDefault(), splashnames[randomSplash - 1].c_str(), 32, 3.2f).y/2},
                sin(2 * frames * PI/180) * 5.0f - 5.0f, 
                32,
                3.2f,
                YELLOW
            );


            EndDrawing();

        }
        else if(gamestate == NEW_WORLD)
        {
            BeginDrawing();
            ClearBackground(BLACK);

            for (int x = 0; x < 34; x++)
            {
                for (int y = 0; y < 17; y++)
                {
                    DrawTextureEx(tileBg,{32.0f * x,44.0f * y},0,2,WHITE);
                }
            }

            std::string worldTypeName;

            switch (selectedWorldType)
            {
            case 1:
                worldTypeName = "Regular";
                break;
            case 2:
                worldTypeName = "Ultrakill reference";
                break;
            case 3:
                worldTypeName = "Pancake";
                break;
            case 4:
                worldTypeName = "Parkour";
                break;
            case 5:
                worldTypeName = "Winter";
                break;
            
            default:
                break;
            }

            DrawText(TextFormat("World Type: %s", worldTypeName.c_str()), screenWidth/2 - MeasureText("World Type: E X AMP L E", 48)/2, 250, 48, WHITE);

            if(Button(screenWidth/2 - 300, 300, 600, 75, buttonTexture, GREEN, "Change World Type", mousePos, 48))
            {
                selectedWorldType += 1;
                if(selectedWorldType > 5){selectedWorldType = 1;}
            }

            if(Button(screenWidth/4 - 250, 500, 500, 75, buttonTexture, GREEN, "Generate World", mousePos, 48))
            {
                WorldGen(selectedWorldType);
            }

            if(Button(screenWidth*0.75f - 250, 500, 500, 75, buttonTexture, GREEN, "Back", mousePos, 48))
            {
                gamestate = MAIN_MENU;
                randomSplash = randomInt(1,11);
            }


            EndDrawing();
        }
        else if(gamestate == IN_GAME)
        {

            if(frames % 12 == 0 && liquidFlowOn == true){liquidFlow();}
            if(frames % 6 == 0 && blockGravityOn == true){gravityBlocksFall();}

            Vector3 playerShadowLocation = getBlockBelowPlayer();

            if((map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)].blockId != BLOCK_IDS::Ice && Player.zVelocity == 0.0f) || IsKeyDown(KEY_W) ||IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D))
            {playerVelocity = {0.0f,0.0f};}
            
            if(IsKeyDown(KEY_A)){playerVelocity.x -= 5.0f; playerVelocity.y += 5.0f; movingDirection = 1;}
            if(IsKeyDown(KEY_D)){playerVelocity.x += 5.0f; playerVelocity.y -= 5.0f; movingDirection = 1;}

            if(IsKeyDown(KEY_W)){playerVelocity.x -= 5.0f; playerVelocity.y -= 5.0f; movingDirection = 2;}
            if(IsKeyDown(KEY_S)){playerVelocity.x += 5.0f; playerVelocity.y += 5.0f; movingDirection = 0;}

            if(playerVelocity.x < -10.0f){playerVelocity.x = -10.0f;}
            else if(playerVelocity.x > 10.0f){playerVelocity.x = 10.0f;}

            if(playerVelocity.y < -10.0f){playerVelocity.y = -10.0f;}
            else if(playerVelocity.y > 10.0f){playerVelocity.y = 10.0f;}

            movePlayer(playerVelocity.x * GetFrameTime(), playerVelocity.y * GetFrameTime());

            if(IsKeyPressed(KEY_O)){showOutline = !showOutline;}
            if(IsKeyPressed(KEY_Q)){gamestate = BLOCK_MENU;}
            if(IsKeyPressed(KEY_T)){gamestate = COMMAND_MENU;}

            if(IsKeyPressed(KEY_E)){replaceBlock(floor(Player.x) + Cursor.xOffset,floor(Player.y) + Cursor.yOffset,ceil(Player.z) + Cursor.zOffset, Player.selectedBlock * (int)Cursor.placing);}
            if(IsKeyPressed(KEY_UP)){Cursor.yOffset -= 1;}
            if(IsKeyPressed(KEY_DOWN)){Cursor.yOffset += 1;}
            if(IsKeyPressed(KEY_LEFT)){Cursor.xOffset -= 1;}
            if(IsKeyPressed(KEY_RIGHT)){Cursor.xOffset += 1;}
            if(IsKeyPressed(KEY_HOME)){Cursor.zOffset += 1;}
            if(IsKeyPressed(KEY_END)){Cursor.zOffset -= 1;}
            if(IsKeyPressed(KEY_RIGHT_CONTROL)){Cursor.placing = !Cursor.placing;}
            adjustCursor();

            if(gravityOn == false)
            {
                if(IsKeyDown(KEY_SPACE))
                {
                    Player.z += 7.0f * GetFrameTime();
                }
                if(IsKeyDown(KEY_LEFT_SHIFT))
                {
                    Player.z -= 7.0f * GetFrameTime();
                }
            }
            else
            {
                Player.z += Player.zVelocity * GetFrameTime();
                Player.zVelocity -= 19.64f * GetFrameTime();
                if(intInArray(nonSolidBlocks, map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)].blockId, nonSolidBlockCount) == false)
                {
                    if(Player.zVelocity < 0)
                    {
                        Player.zVelocity = 0;
                    }
                    if(Player.zVelocity == 0)
                    {
                        Player.z = ceil(Player.z);
                        if(intInArray(halfBlocks, map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)].blockId, halfBlockCount) == true)
                        {
                            Player.z -= 0.3f;
                        }
                        
                        if(IsKeyPressed(KEY_SPACE)){Player.zVelocity = 5.41f;}
                    }
                }
                else
                {
                    if(IsKeyPressed(KEY_SPACE) && (map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)].blockId == BLOCK_IDS::Lava || map[(int)ceil(Player.z)][(int)floor(Player.y)][(int)floor(Player.x)].blockId == BLOCK_IDS::Water)){Player.zVelocity = 5.41f;}
                }
            }

            

            if(Player.z < 0){Player.z = 0.0f;}
            if(Player.z > MAX_MAP_Z - 2){Player.z = MAX_MAP_Z - 2;}
            
            BeginDrawing();
            ClearBackground(RAYWHITE);
            
            //draw background sky and void
            if(activeworldtype == 2)
            {
                DrawRectangle(0,0,1080,500, hellSkyColor);
                DrawRectangleGradientV(0,000,1080,340,BLACK,hellSkyColor);
                DrawRectangleGradientV(0,500,1080,340,hellSkyColor,BLACK);
            }
            else
            {
                DrawRectangle(0,0,1080,500, skyColor);
                DrawRectangleGradientV(0,500,1080,340,skyColor,BLACK);
            }

            //draw blocks
            int layer1DrawEnd = round(Player.z) + 1;
            int layer2DrawStart = round(Player.z) + 1;
            
            drawTheWorld(0,layer1DrawEnd);

            drawTheWorld(layer2DrawStart,MAX_MAP_Z);
            
            Vector2 screenPosition;
            screenPosition.x = (Player.x - Player.y) * xShift;// - xShift;
            screenPosition.y = (Player.x + Player.y) * yShift - (yShift * 3);
            screenPosition.y -= (xShift - 1) * (Player.z);

            if(showOutline == true)
            {
                DrawTextureEx(Player.outlineTextures[(int)(movingDirection == 1)], {(float)(drawCenterX + screenPosition.x), (float)(239 + screenPosition.y)}, 0, drawSize/16.0f, WHITE);
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
        else if(gamestate == BLOCK_MENU)
        {
            if(IsKeyPressed(KEY_Q)){gamestate = IN_GAME;}
            BeginDrawing();

            for (int x = 0; x < 34; x++)
            {
                for (int y = 0; y < 17; y++)
                {
                    DrawTextureEx(tileBg,{32.0f * x,44.0f * y},0,2,WHITE);
                }
            }

            DrawRectangle(40, 40, screenWidth - 80, screenHeight - 160, fadeColor(BLACK, 0.6f));

            for(int y = 0; y < 6; y++)
            {
                for(int x = 0; x < 12; x++)
                {
                    int drawingblockType = x + y*12;
                    if(drawingblockType > 34){drawingblockType -= 35; blockAtlasRectangle.x = 16.0f;}
                    else{blockAtlasRectangle.x = 0.0f;}
                    blockAtlasRectangle.y = 16 * (drawingblockType);
                    if((x + y*12) < totalBlockCount)
                    {
                        if((x + y*12) + 1 == BLOCK_IDS::Water)
                        {
                            DrawTexturePro(
                                waterBucketTexture,
                                {0,0,16,16},
                                {(float)(64 + x * 80), (float)(64 + y * 80), 72, 72},
                                {0, 0},
                                0,
                                WHITE
                            );
                        }
                        else if((x + y*12) + 1 == BLOCK_IDS::Lava)
                        {
                            DrawTexturePro(
                                lavaBucketTexture,
                                {0,0,16,16},
                                {(float)(64 + x * 80), (float)(64 + y * 80), 72, 72},
                                {0, 0},
                                0,
                                WHITE
                            );
                        }
                        else
                        {
                            DrawTexturePro(
                                blockTextures,
                                blockAtlasRectangle,
                                {(float)(64 + x * 80), (float)(64 + y * 80), 72, 72},
                                {0, 0},
                                0,
                                WHITE
                            );
                        }
                        if((x + y*12) + 1 == Player.selectedBlock)
                        {
                            DrawRectangleLines(64 + x * 80, 64 + y * 80, 72, 72, YELLOW);
                        }
                    }

                }
            }

            

            if(Button(screenWidth/2 - 250, 620, 500, 75, buttonTexture, GREEN, "Quit and Save", mousePos, 48))
            {
                gamestate = SAVE_MENU;
                loadFileNames();
                selectedSave = -1;
            }

            mousePos.x -= 64;
            mousePos.y -= 64;

            mousePos.x = floor(mousePos.x/80);
            mousePos.y = floor(mousePos.y/80);


            if(mousePos.x >= 0 && mousePos.x < 12 && mousePos.y >= 0 && mousePos.y < 6 && (mousePos.x + mousePos.y*12) < totalBlockCount)
            {
                DrawRectangleLines(64 + mousePos.x * 80, 64 + mousePos.y * 80, 72, 72, WHITE);

                int hoverId = mousePos.x + mousePos.y*12;

                mousePos = GetMousePosition();

                Vector2 nameSize = MeasureTextEx(GetFontDefault(), blockNames[hoverId].c_str(), 32, 3.2f);

                if(mousePos.x + nameSize.x + 30 > screenWidth)
                {
                    DrawRectangle((int)mousePos.x - (nameSize.x + 30),(int)mousePos.y, nameSize.x + 20, nameSize.y, fadeColor(BLACK, 0.6f));
                    DrawRectangleLines((int)mousePos.x - (nameSize.x + 30),(int)mousePos.y, nameSize.x + 20, nameSize.y, WHITE);
                    DrawText(blockNames[hoverId].c_str(), (int)mousePos.x - (nameSize.x + 20), (int)mousePos.y, 32, WHITE);
                }
                else
                {
                    DrawRectangle((int)mousePos.x + 10,(int)mousePos.y, nameSize.x + 20, nameSize.y, fadeColor(BLACK, 0.6f));
                    DrawRectangleLines((int)mousePos.x + 10,(int)mousePos.y, nameSize.x + 20, nameSize.y, WHITE);
                    DrawText(blockNames[hoverId].c_str(), (int)mousePos.x + 20, (int)mousePos.y, 32, WHITE);
                }
                
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){Player.selectedBlock = hoverId + 1;}

            }


            DrawFPS(10, 10);
            EndDrawing();
        }
        else if(gamestate == LOAD_MENU)
        {

            BeginDrawing();
            ClearBackground(BLACK);

            for (int x = 0; x < 34; x++)
            {
                for (int y = 0; y < 17; y++)
                {
                    DrawTextureEx(tileBg,{32.0f * x,44.0f * y},0,2,WHITE);
                }
            }

            DrawRectangle(40, 40, screenWidth - 80, screenHeight - 140, fadeColor(BLACK, 0.6f));
            Vector2 nameSize = {0.0f,0.0f};
            Vector2 dateSize = {0.0f,0.0f};
            int drawGap = 120;

            for (int i = 0; i < saveFilenames.size(); i++)
            {
                nameSize = MeasureTextEx(GetFontDefault(), saveFilenames[i].name.c_str(), 64, 6.4f);
                dateSize = MeasureTextEx(GetFontDefault(), saveFilenames[i].modifyTime.c_str(), 24, 2.4f);

                if(saveFilenames[i].worldType < 1)
                {
                    DrawTexturePro(worldErrorTexture, {0,0,16,16}, {60,(float)(50 + drawGap * i), nameSize.y + dateSize.y, nameSize.y + dateSize.y}, {0,0}, 0.0f, WHITE);
                }
                else
                {
                    DrawTexturePro(biomeTextures[saveFilenames[i].worldType - 1], {0,0,16,16}, {60,(float)(50 + drawGap * i), nameSize.y + dateSize.y, nameSize.y + dateSize.y}, {0,0}, 0.0f, WHITE);
                }

                DrawText(saveFilenames[i].name.c_str(), 70 + nameSize.y + dateSize.y, 50 + drawGap * i, 64, WHITE);
                DrawText(saveFilenames[i].modifyTime.c_str(), 70 + nameSize.y + dateSize.y, 50 + (int)nameSize.y + drawGap * i, 24, GRAY);
                
                if(mousePos.y >= 50 + drawGap * i && mousePos.y <= 50 + (int)nameSize.y + dateSize.y + drawGap * i && mousePos.x >= 50 && mousePos.x <= screenWidth-50){
                    DrawRectangleLines(50,50 + drawGap*i, screenWidth-100, nameSize.y + dateSize.y, WHITE);
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        selectedSave = i;
                    }
                }

            }

            mousePos = GetMousePosition();

            if(selectedSave >= 0)
            {
                nameSize = MeasureTextEx(GetFontDefault(), saveFilenames[selectedSave].name.c_str(), 64, 6.4f);
                DrawRectangleLines(50,50 + drawGap*selectedSave, screenWidth-100, nameSize.y + dateSize.y, YELLOW);

                if(Button(screenWidth*0.75f - 200, 620, 400, 75, buttonTexture, GREEN, "Load World", mousePos, 48))
                {
                    loadData(saveFilenames[selectedSave].path);
                    gamestate = IN_GAME;
                }
            }
            else
            {
                Button(screenWidth*0.75f - 200, 620, 400, 75, buttonTexture, BLACK, "Load World", {screenWidth*0.75f - 199, 621}, 48);
            }
            
            if(Button(screenWidth/4 - 200, 620, 400, 75, buttonTexture, GREEN, "Back", mousePos, 48))
            {
                gamestate = MAIN_MENU;
                randomSplash = randomInt(1,11);
            }

            EndDrawing();
        }
        else if(gamestate == SAVE_MENU)
        {

            mousePos.y -= 50;

            mousePos.y = floor(mousePos.y/80.0f);

            BeginDrawing();
            ClearBackground(BLACK);

            for (int x = 0; x < 34; x++)
            {
                for (int y = 0; y < 17; y++)
                {
                    DrawTextureEx(tileBg,{32.0f * x,44.0f * y},0,2,WHITE);
                }
            }

            DrawRectangle(40, 40, screenWidth - 80, screenHeight - 140, fadeColor(BLACK, 0.6f));
            Vector2 nameSize;

            for (int i = 0; i < saveFilenames.size(); i++)
            {
                nameSize = MeasureTextEx(GetFontDefault(), saveFilenames[i].name.c_str(), 64, 6.4f);
                DrawText(saveFilenames[i].name.c_str(), 60, 50 + 80 * i, 64, WHITE);
                
                if(mousePos.y == i){
                    DrawRectangleLines(50,50 + 80*i, nameSize.x + 20, nameSize.y, WHITE);
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        selectedSave = i;
                    }
                }

            }

            mousePos = GetMousePosition();
            if(selectedSave >= 0)
            {
                nameSize = MeasureTextEx(GetFontDefault(), saveFilenames[selectedSave].name.c_str(), 64, 6.4f);
                DrawRectangleLines(50,50 + 80*selectedSave, nameSize.x + 20, nameSize.y, fadeColor(YELLOW, 0.6));

                if(Button(screenWidth*0.75f - 200, 620, 400, 75, buttonTexture, GREEN, "Overwrite", GetMousePosition(), 48))
                {
                    saveData(saveFilenames[selectedSave].path);
                    gamestate = MAIN_MENU;
                }
            }
            else
            {
                Button(screenWidth*0.75f - 200, 620, 400, 75, buttonTexture, BLACK, "Overwrite", {screenWidth*0.75f - 199, 621}, 48);
            }
            
            if(Button(screenWidth/4 - 200, 620, 400, 75, buttonTexture, GREEN, "Create New", mousePos, 48))
            {
                saveData((std::string)TextFormat("saves/save%i.icw", (int)saveFilenames.size()));
                gamestate = MAIN_MENU;
            }

            EndDrawing();
        }
        else if(gamestate == COMMAND_MENU)
        {
            BeginDrawing();
            ClearBackground(BLACK);

            for (int x = 0; x < 34; x++)
            {
                for (int y = 0; y < 17; y++)
                {
                    DrawTextureEx(tileBg,{32.0f * x,44.0f * y},0,2,WHITE);
                }
            }

            DrawText("Enter command:", screenWidth/2 - MeasureText("Enter command:", 48), screenHeight/2 - 100, 48, WHITE);
            Button(screenWidth/8, screenHeight/2 - 50, 6 * screenWidth/8, 100, buttonTexture, DARKGRAY, "", {screenWidth/2,screenHeight/2},0);
            if(validCommandEntry == false){DrawText("Invalid command!", screenWidth/8, screenHeight/2 + 105, 48, RED);}

            DrawText(commandEntry.c_str(), screenWidth/8 + 10, screenHeight/2.0f - MeasureTextEx(GetFontDefault(), commandEntry.c_str(), 72, 7.2f).y/2.0f, 72, WHITE);

            EndDrawing();

            int keyPress = GetKeyPressed();

            switch (keyPress)
            {
            case KEY_ZERO:
                commandEntry += '0';
                break;
            case KEY_ONE:
                commandEntry += '1';
                break;
            case KEY_TWO:
                commandEntry += '2';
                break;
            case KEY_THREE:
                commandEntry += '3';
                break;
            case KEY_FOUR:
                commandEntry += '4';
                break;
            case KEY_FIVE:
                commandEntry += '5';
                break;
            case KEY_SIX:
                commandEntry += '6';
                break;
            case KEY_SEVEN:
                commandEntry += '7';
                break;
            case KEY_EIGHT:
                commandEntry += '8';
                break;
            case KEY_NINE:
                commandEntry += '9';
                break;
            case KEY_A:
                commandEntry += 'A';
                break;
            case KEY_B:
                commandEntry += 'B';
                break;
            case KEY_C:
                commandEntry += 'C';
                break;
            case KEY_D:
                commandEntry += 'D';
                break;
            case KEY_E:
                commandEntry += 'E';
                break;
            case KEY_F:
                commandEntry += 'F';
                break;
            case KEY_BACKSPACE:
                if(commandEntry.length() > 0)
                {
                    commandEntry = commandEntry.substr(0, commandEntry.length() - 1);
                }
                break;
            case KEY_ENTER:
                validCommandEntry = parseCommand(commandEntry);
                commandEntry = "";

                if(validCommandEntry == true){gamestate = IN_GAME;}
                break;
            default:
                break;
            }

            if(IsKeyPressed(KEY_ESCAPE))
            {
                commandEntry = "";
                validCommandEntry = true;
                gamestate = IN_GAME;
            }
            
        }

    }

    unloadAssets();

    CloseWindow();
    
}

