#define DEBUG

#include "lib.h"
#include "game_logic.h"

int main(void)
{
    struct breakoutBall main_ball = {SCREEN_W / 2, SCREEN_H / 2, 1, -1}; // This struct stores ball position and velocity
    unsigned int blocks[SCREEN_H];                                       // declare necessary variables
    int paddle_pos = PAD_START;

    startup(blocks); // startup function sets up the blocks, paddle and ball

    while (1) // Main body of program runs until the ball hits the bottom of the screen
    {
        paddle_pos = check_mmio(paddle_pos); // check for keyboard input
        if (main_ball.y >= SCREEN_H)
        {
            break;
        } // end if ball is at the bottom
        main_ball = wall_bounce(main_ball);               // bounce off walls
        main_ball = break_blocks(main_ball, blocks);      // bounce off and break blocks
        main_ball = paddle_bounce(main_ball, paddle_pos); // bounce off paddle
        updateScreen(paddle_pos, blocks, main_ball);      // draw all game elements
        display_score(calcScore(blocks));                 // draw score to 7-seg
        delay(SLOWDOWN * 2000);                           // slow down if on QEMU
    }
    printstr("*** GAME OVER! ***"); // print end message

    return 0;
}
