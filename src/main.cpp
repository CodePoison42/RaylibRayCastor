#include "STrig.hpp"
#include "raylib.h"
#include "raymath.h"

#include <cstdint>
#include <vector>
#include <math.h>

#define SW          (int)640
#define SH          (int)480
#define texWidth    (int)64
#define texHeight   (int)64
#define mapWidth         24
#define mapHeight        24

int worldMap[mapWidth][mapHeight]=
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

struct Ray2d
{
    double Ang, Len;
};

double posX{22}, posY{11.5};
Vector2 dir{-1.0f, 0.0f};
Vector2 plane{0.0f, 0.66f};

double newtime = 0;
double oldtime = 0;

Color ColorDivide(Color color, double n){
    Color New = {color.r/n, color.g/n, color.b/n, color.a};
    return New;
}
Color ColorMulti(Color color, double n){
    Color New = {color.r*n, color.g*n, color.b*n, color.a};
    return New;
}

void Render2DMap(const int map[mapWidth][mapHeight]) {
    int size = 6;
    for (int row = 0; row < mapWidth; row++) {
        for (int col = 0; col < mapHeight; col++) {
            Color color;
            switch(worldMap[row][col])
            {
                case 1:  color = RED;  break; //red
                case 2:  color = GREEN;  break; //green
                case 3:  color = BLUE;   break; //blue
                case 4:  color = {134, 231, 12, 255};  break; //white
                case 5:  color = YELLOW;  break; //yellow
                default: color = {0, 0, 0, 0}; break; //yellow
            }

            DrawRectangle(col * size, row * size, size, size, color);
        }
    }
}


int main(void) {
    InitWindow(SW, SW, "raycasting");
    //SetTargetFPS(24);
    
    Color buffer[SH][SW]; // y-coordinate first because it works per scanline
    std::vector<Color> texture[8];
    for(int i = 0; i < 8; i++) texture[i].resize(texHeight * texWidth);
    Image tex = LoadImage("./assets/p2.png");


    for(int x = 0; x < texWidth; x++)
    for(int y = 0; y < texHeight; y++)
    {
        Color ColorRand = DARKGRAY;
        texture[0][texWidth * y + x] = {GetImageColor(tex, x, y)}; //flat red texture with black cross
        
        texture[1][texWidth * y + x] = {GetImageColor(tex, x+(texWidth*1), y)}; //sloped greyscale
        
        texture[2][texWidth * y + x] = {GetImageColor(tex, x+(texWidth*2), y)}; //sloped yellow gradient
        
        texture[3][texWidth * y + x] = {GetImageColor(tex, x+(texWidth*3), y)}; //xor greyscale
        
        texture[4][texWidth * y + x] = {GetImageColor(tex, x+(texWidth*4), y)}; //xor green
        
        texture[5][texWidth * y + x] = {GetImageColor(tex, x+(texWidth*5), y)}; //red bricks
        
        texture[6][texWidth * y + x] = {GetImageColor(tex, x+(texWidth*6), y)}; //red gradient
        
        texture[7][texWidth * y + x] = {GetImageColor(tex, x+(texWidth*7), y)}; //flat grey texture
    }

    while (!WindowShouldClose()) {

        double frameTime = (newtime - oldtime); //frameTime is the time this frame has taken, in seconds
        double moveSpeed = frameTime * 4.0; //the constant value is in squares/second
        double rotSpeed = frameTime * 3.0; //the constant value is in radians/second
        
        if (IsKeyDown(KEY_LEFT_BRACKET)){
            plane.x-=0.01f;
        }
        if (IsKeyDown(KEY_RIGHT_BRACKET)){
            plane.x+=0.01f;
        }
        if (IsKeyDown(KEY_UP)) {
            if(worldMap[int(posX + dir.x * moveSpeed)][int(posY)] == false) posX += dir.x * moveSpeed;
            if(worldMap[int(posX)][int(posY + dir.y * moveSpeed)] == false) posY += dir.y * moveSpeed;
        }

        //move backwards if no wall behind you
        if (IsKeyDown(KEY_DOWN)){
            if(worldMap[int(posX - dir.x * moveSpeed)][int(posY)] == false) posX -= dir.x * moveSpeed;
            if(worldMap[int(posX)][int(posY - dir.x * moveSpeed)] == false) posY -= dir.y * moveSpeed;
        }

        //rotate to the right
        if (IsKeyDown(KEY_RIGHT)) {
            //both camera direction and camera plane must be rotated
            double oldDirX = dir.x;
            dir.x = dir.x * cos(-rotSpeed) - dir.y * sin(-rotSpeed);
            dir.y = oldDirX * sin(-rotSpeed) + dir.y * cos(-rotSpeed);
            double oldPlaneX = plane.x;
            plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
            plane.y = oldPlaneX * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
        }

        //rotate to the left
        if (IsKeyDown(KEY_LEFT)) {
            //both camera direction and camera plane must be rotated
            double oldDirX = dir.x;
            dir.x = dir.x * cos(rotSpeed) - dir.y * sin(rotSpeed);
            dir.y = oldDirX * sin(rotSpeed) + dir.y * cos(rotSpeed);
            double oldPlaneX = plane.x;
            plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
            plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);
        }


        
        BeginDrawing();

        ClearBackground(BLACK);

        for(int x = 0; x < SW; x++) {
            //calculate ray position and direction
            double cameraX = 2 * x / double(SW) - 1; //x-coordinate in camera space
            double rayDirX = dir.x + plane.x * cameraX;
            double rayDirY = dir.y + plane.y * cameraX; 
            
            //which box of the map we're in
            int mapX = int(posX);
            int mapY = int(posY);

            //length of ray from current position to next x or y-side
            double sideDistX{};
            double sideDistY{};

            //length of ray from one x or y-side to next x or y-side
            double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
            double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
            double perpWallDist{};

            //what direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            int hit = 0; //was there a wall hit?
            int side; //was a NS or a EW wall hit?

            //calculate step and initial sideDist
            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }
            while(hit == 0) {
                if (sideDistX < sideDistY){
                    sideDistX+=deltaDistX;
                    mapX+=stepX;
                    side = 0;
                }
                else{
                    sideDistY += deltaDistY;
                    mapY+= stepY;
                    side = 1;
                }
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }

            if(side == 0) perpWallDist = (sideDistX - deltaDistX);
            else perpWallDist = (sideDistY - deltaDistY);
        
            //Calculate height of line to draw on screen
            int lineHeight = (int)(SH / perpWallDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + SH / 2;
            if(drawStart < 0)drawStart = 0;
            int drawEnd = lineHeight / 2 + SH / 2;
            if(drawEnd >= SH)drawEnd = SH - 1;
            
            //texturing calculations
            int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

            //calculate value of wallX
            double wallX; //where exactly the wall was hit
            if (side == 0) wallX = posY + perpWallDist * rayDirY;
            else           wallX = posX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            //x coordinate on the texture
            int texX = int(wallX * double(texWidth));
            if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
            if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

            
            // How much to increase the texture coordinate per screen pixel
            double step = 1.0 * texHeight / lineHeight;
            // Starting texture coordinate
            double texPos = (drawStart - SH / 2 + lineHeight / 2) * step;
            for(int y = drawStart; y<drawEnd; y++)
            {
                // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
                int texY = (int)texPos & (texHeight - 1);
                texPos += step;
                Color color = texture[texNum][texHeight * texY + texX];
                //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
                //if(side == 1) color = ColorDivide(color, Clamp(perpWallDist*0.3f, 1.0f, 50.0f));
                buffer[y][x] = color;
            }

            /*
            //choose wall color
            Color color;
            switch(worldMap[mapX][mapY])
            {
                case 1:  color = RED;  break; //red
                case 2:  color = GREEN;  break; //green
                case 3:  color = BLUE;   break; //blue
                case 4:  color = {134, 231, 12, 255};  break; //white
                default: color = YELLOW; break; //yellow
            }

            //give x and y sides different brightness
            if (side == 0) {color = ColorDivide(color, Clamp(perpWallDist*0.3f, 1.0f, 50.0f));}
            if (side == 1) {color = ColorDivide(color, Clamp(perpWallDist*0.3f, 1.0f, 50.0f));}
            */
            
            //draw the pixels of the stripe as a vertical line
            //DrawLine(x, drawStart, x, drawEnd, col);
            
                    //DrawLine(x, drawStart, x, drawEnd, buffer[h][w]);
            for (int y = 0; y < SH; y++) {
                DrawPixelV({(float)x, (float)y}, buffer[y][x]);  
            }
              
            //DrawLine(x, drawStart, x, drawEnd, buffer[1][x]);
                
        }    
             

            //Render2DMap(worldMap);
            DrawFPS(250,10);
            
            DrawText(TextFormat("%lf", posX), 0, 180, 20, GREEN);
            DrawText(TextFormat("%lf", posY), 0, 200, 20, GREEN);
            //for (int i = 0; i < 250; i++)
            //    DrawRectangle(i*3, Graph[i]*3000, 3, 5, GREEN);
            //for (int i = 1; i < 250; i+=2)
            //    DrawLineEx({(float)i*3, (float)Graph[i]*3000}, {(float)i*3-1, (float)Graph[i-2]*3000}, 2, RED);
        
        EndDrawing();
        oldtime = newtime;
        newtime = GetTime();
        for(int y = 0; y < SH; y++) for(int x = 0; x < SW; x++) buffer[y][x] = {0, 0, 0};
    }

    CloseWindow();
    return 0;
}