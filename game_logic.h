#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

// Breakout constants
#define SCREEN_H 32
#define SCREEN_W 32
#define PADDLE_Y 24

#define BLOCK_START 6
#define BLOCK_END 8
#define NUM_BLOCKS ((BLOCK_END - BLOCK_START + 1) * 32)

#ifdef DEBUG // Debug mode stretches the paddle across the screen to garauntee bounces
#define PADDLE_W 0xFFFFFFFF
#define PAD_START 0
#else
#define PADDLE_W 0xFF
#define PAD_START 12
#endif

struct breakoutBall
{
    int x;
    int y;
    int i;
    int j;
};

// Breakout Functions
void startup(int *blocks);
int start_button(void);
int slider(void);
void delay(int);
void updateScreen(int paddle, int *blocks, struct breakoutBall ball);
void printBinary(int input);
int calcScore(int *blocks);
int check_mmio(int paddle_pos);
struct breakoutBall paddle_bounce(struct breakoutBall main_ball, int paddle_pos);
struct breakoutBall wall_bounce(struct breakoutBall);
struct breakoutBall break_blocks(struct breakoutBall main_ball, int *blocks);

#endif