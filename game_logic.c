#include "lib.h"
#include "game_logic.h"

/* This function runs on startup. It prints the startup messages (different depending on whether QEMU or logisim is being used)*/
void startup(int *blocks)
{
#ifdef QEMU20180
    while (!readchar())
    {
    } // simply wait for any character to be pressed

#else
    while ((start_button() & 1) == 0)
    {
    }
#endif
    printstr("*** BREAKOUT ***\n");
    printstr(START_MSG);

    for (int i = BLOCK_START; i <= BLOCK_END; i++)
    {
        blocks[i] = 0xFFFFFFFF;
    }
}

// this function just waits so that QEMU runs slower
void delay(volatile int delay)
{
    while (delay--)
        ;
}

void updateScreen(int paddle_pos, int *blocks, struct breakoutBall ball) // Updates the screen with the positions     of all game elements
{
    int paddle = 0xFF << paddle_pos;
    int screen[SCREEN_H];
    for (int i = 0; i < SCREEN_H; i++)
    {
        screen[i] = blocks[i];
    }
    screen[PADDLE_Y] = screen[PADDLE_Y] | paddle;
    screen[ball.y] = screen[ball.y] | (0x1 << (SCREEN_W - 1 - ball.x));
    showpic(screen);
}

void printBinary(int input) // Function used for debugging. Prints a 32-bit integer as a binary     number
{
    for (int i = SCREEN_W - 1; i >= 0; i--)
    {
        printint(((0x1 << i) & input) != 0);
    }
}

int calcScore(int *blocks) // calculates the score by counting the remaining blocks
{
    int total = 0;
    for (int i = BLOCK_START; i < BLOCK_END + 1; i++)
    {
        for (int j = SCREEN_W - 1; j >= 0; j--)
        {
            total += ((0x1 << j) & blocks[i]) != 0;
        }
    }
    return NUM_BLOCKS - total;
}

#ifdef QEMU20180
int check_mmio(int paddle_pos)
{
    if (pollkbd() & RDR_READY_BIT)
    {
        volatile char paddle_dir = readchar();
        // Not readable code but left it in because it's cool. Expression is evaluated and sets the paddle position.
        paddle_pos = paddle_pos + ((paddle_dir == 'a') ? (paddle_pos < 24) : 0) + ((paddle_dir == 'd') ? -1 * (paddle_pos > 0) : 0);
        paddle_dir = '\0';
        for (int i = 0; i < SLOWDOWN; i++)
        {
            paddle_dir = readchar(); // clears MMIO input buffer by reading input many times. Also slows down qemu for smoother performance
        }
    }
    return paddle_pos; // return the new position of the paddle
}

#else // if running in logisim, just check the paddle
int check_mmio(int paddle_pos)
{
    return slider();
}
#endif

// this function checks if the ball is bouncing off the paddle and returns
// a ball struct with updated velocities
struct breakoutBall paddle_bounce(struct breakoutBall main_ball, int paddle_pos)
{
    int paddle = (0xFF << paddle_pos);

    // first check if the ball is above the paddle and moving toward it
    if ((0x1 << (31 - main_ball.x) & paddle && main_ball.y == 23) > 0)
    {
        main_ball.j = -main_ball.j;
    }

    // otherwise check if the ball is moving toward the corner of the paddle
    else if (((0x1 << (SCREEN_H - 1 - main_ball.x) >> main_ball.i) & paddle && main_ball.y == SCREEN_H) > 0)
    {
        main_ball.i = -main_ball.i;
        main_ball.j = -main_ball.j;
    }
    return main_ball; // return an updated breakoutBall
}

// This function checks if the ball is about to hit a wall
struct breakoutBall wall_bounce(struct breakoutBall main_ball)
{
    // check if the ball is going to hit the top
    if (main_ball.y + main_ball.j < 0)
    {
        main_ball.j = -main_ball.j;
    }

    // check if the ball is going to hit the sides
    if (main_ball.x + main_ball.i > 31 || main_ball.x + main_ball.i < 0)
    {
        main_ball.i = -main_ball.i;
    }
    main_ball.y += main_ball.j; // update the ball position
    main_ball.x += main_ball.i;

    return main_ball; // return an updated breakoutBall
}

// this function checks if a ball is going to hit a block and
// causes it to bounce and break the blocks
struct breakoutBall break_blocks(struct breakoutBall main_ball, int *blocks)
{
    int ball_pixels = 0x1 << (SCREEN_W - 1 - main_ball.x);
    if ((ball_pixels & blocks[main_ball.y]) > 0) // check if hitting the corner of the block
    {
        blocks[main_ball.y] &= ~ball_pixels; // break the block using a bitmask
        main_ball.i = -main_ball.i;          // invert both velocities
        main_ball.j = -main_ball.j;
    }
    else
    {
        // check if the ball is hitting the top or bottom of a block
        if ((ball_pixels & blocks[main_ball.y + main_ball.j]) > 0)
        {
            blocks[main_ball.y + main_ball.j] &= ~ball_pixels;
            main_ball.j = -main_ball.j;
        }

        // check if the ball is hitting either side of a block
        if (main_ball.i > 0)
            if ((ball_pixels & (blocks[main_ball.y] << main_ball.i)) > 0)
            {
                blocks[main_ball.y] &= ~(ball_pixels >> main_ball.i);
                main_ball.i = -main_ball.i;
            }
        if (main_ball.i < 0)
            if ((ball_pixels & (blocks[main_ball.y] >> main_ball.i * -1)) > 0)
            {
                blocks[main_ball.y] &= ~(ball_pixels << main_ball.i * -1);
                main_ball.i = -main_ball.i;
            }
    }
    return main_ball; // return an updated breakoutBall
}