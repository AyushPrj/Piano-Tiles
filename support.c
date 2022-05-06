#include "stm32f0xx.h"
#include <stdint.h>
#include <stdlib.h>
#include "lcd.h"

// The four coordinates of a tile
struct tiles {
    int x1;
    int y1;
    int x2;
    int y2;
};

// 8 Tiles, one for each column
struct tiles t1;
struct tiles t2;
struct tiles t3;
struct tiles t4;
struct tiles t5;
struct tiles t6;
struct tiles t7;
struct tiles t8;

// Wait for certain nano seconds
void nano_wait(unsigned int n) {
    asm(    "       mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "       bgt repeat\n" : : "r"(n) : "r0", "cc");
}

// Move a tile given the tile number
void move_tiles(int tile_num)
{
    // move the first tile down one pixel and update it's position
    if (tile_num == 1){
        LCD_DrawFillRectangle(t1.x1,t1.y2+1,t1.x2,t1.y2+1,WHITE);
        LCD_DrawFillRectangle(t1.x1,t1.y1,t1.x2,t1.y1,BLACK);
        t1.y1 += 1;
        t1.y2 += 1;

    }

    // move the second tile down one pixel and update it's position
    if (tile_num == 2){
        LCD_DrawFillRectangle(t2.x1,t2.y2+1,t2.x2,t2.y2+1,YELLOW);
        LCD_DrawFillRectangle(t2.x1,t2.y1,t2.x2,t2.y1,BLACK);
        t2.y1 += 1;
        t2.y2 += 1;
    }

    // move the third tile down one pixel and update it's position
    if (tile_num == 3){
        LCD_DrawFillRectangle(t3.x1,t3.y2+1,t3.x2,t3.y2+1,MAGENTA);
        LCD_DrawFillRectangle(t3.x1,t3.y1,t3.x2,t3.y1,BLACK);
        t3.y1 += 1;
        t3.y2 += 1;
    }
    
    // move the fourth tile down one pixel and update it's position
    if (tile_num == 4){
        LCD_DrawFillRectangle(t4.x1,t4.y2+1,t4.x2,t4.y2+1,WHITE);
        LCD_DrawFillRectangle(t4.x1,t4.y1,t4.x2,t4.y1,BLACK);
        t4.y1 += 1;
        t4.y2 += 1;
    }

    // move the fifth tile down one pixel and update it's position
    if (tile_num == 5){
        LCD_DrawFillRectangle(t5.x1,t5.y2+1,t5.x2,t5.y2+1,RED);
        LCD_DrawFillRectangle(t5.x1,t5.y1,t5.x2,t5.y1,BLACK);
        t5.y1 += 1;
        t5.y2 += 1;
    }
    
    // move the sixth tile down one pixel and update it's position
    if (tile_num == 6){
        LCD_DrawFillRectangle(t6.x1,t6.y2+1,t6.x2,t6.y2+1,CYAN);
        LCD_DrawFillRectangle(t6.x1,t6.y1,t6.x2,t6.y1,BLACK);
        t6.y1 += 1;
        t6.y2 += 1;
    }

    // move the seventh tile down one pixel and update it's position
    if (tile_num == 7){
        LCD_DrawFillRectangle(t7.x1,t7.y2+1,t7.x2,t7.y2+1,BROWN);
        LCD_DrawFillRectangle(t7.x1,t7.y1,t7.x2,t7.y1,BLACK);
        t7.y1 += 1;
        t7.y2 += 1;
    }

    // move the eighth tile down one pixel and update it's position
    if (tile_num == 8){
        LCD_DrawFillRectangle(t8.x1,t8.y2+1,t8.x2,t8.y2+1,GREEN);
        LCD_DrawFillRectangle(t8.x1,t8.y1,t8.x2,t8.y1,BLACK);
        t8.y1 += 1;
        t8.y2 += 1;
    }
}

// Initialize all of the tiles
void init_tiles(void){
    LCD_Clear(0);

    // set the x and y values of the tiles
    t1.x1 = 0;
    t2.x1 = 30;
    t3.x1 = 60;
    t4.x1 = 90;
    t5.x1 = 120;
    t6.x1 = 150;
    t7.x1 = 180;
    t8.x1 = 210;

    t1.x2 = 30;
    t2.x2 = 60;
    t3.x2 = 90;
    t4.x2 = 120;
    t5.x2 = 150;
    t6.x2 = 180;
    t7.x2 = 210;
    t8.x2 = 240;

    t1.y1 = 0;
    t2.y1 = 0;
    t3.y1 = 0;
    t4.y1 = 0;
    t5.y1 = 0;
    t6.y1 = 0;
    t7.y1 = 0;
    t8.y1 = 0;

    t1.y2 = 20;
    t2.y2 = 20;
    t3.y2 = 20;
    t4.y2 = 20;
    t5.y2 = 20;
    t6.y2 = 20;
    t7.y2 = 20;
    t8.y2 = 20;

    // create the tiles
    LCD_DrawFillRectangle(t1.x1, t1.y1, t1.x2, t1.y2, GRAY);
    LCD_DrawFillRectangle(t2.x1, t2.y1, t2.x2, t2.y2, YELLOW);
    LCD_DrawFillRectangle(t3.x1, t3.y1, t3.x2, t3.y2, MAGENTA);
    LCD_DrawFillRectangle(t4.x1, t4.y1, t4.x2, t4.y2, WHITE);
    LCD_DrawFillRectangle(t5.x1, t5.y1, t5.x2, t5.y2, RED);
    LCD_DrawFillRectangle(t6.x1, t6.y1, t6.x2, t6.y2, CYAN);
    LCD_DrawFillRectangle(t7.x1, t7.y1, t7.x2, t7.y2, BROWN);
    LCD_DrawFillRectangle(t8.x1, t8.y1, t8.x2, t8.y2, GREEN);
}
