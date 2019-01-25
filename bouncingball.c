#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define RIGHT 1
#define LEFT 0
#define UP 1
#define DOWN 0
#define U_DELAY 1500 /* Refresh rate */

/**
 * Bouncing ball main function.
 * 
 * Difficulti can be changed by adjusting the terminal's height.
 */
int main(void) 
{    
    WINDOW *mainwin;
    struct winsize w;
    
    struct position {
        int x;
        int y;
    };
    
    struct position ball;
    struct position pad;
    
    short x_direction, y_direction;
    unsigned int input, random_x, loop;
    
    /* Initialise window */
    if( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    /* Don't echo any keypresses */
    noecho(); 
    
    /*Disable cursor */
    curs_set(FALSE); 
    
    /* Disable keyboard/line buffering in tty */
    cbreak(); 
    
    /* Sets getch() non-blocking call */
    nodelay(mainwin, 1); 
    
    /* Enables keypad in terminal */
    keypad(mainwin, 1); 
    
    srand( (unsigned int)time(NULL) );
    
    ball.x = rand() % COLS; /* Random X starting position */
    ball.y = 0; 
    x_direction = RIGHT;
    y_direction = DOWN;
    pad.x = COLS / 2;
    pad.y = LINES - 1;
    
    while(1) {
        /* Dynamically adjusting the grid boundaries */        
        ioctl(0, TIOCGWINSZ, &w);
        
        /* Adjusting the bouncing pad's Y position in case the terminal size is changed */
        if(pad.y != w.ws_row-1)
            pad.y = w.ws_row-1;
        
        /* Add the bouncing pad */
        move(pad.y, pad.x);
        addstr("######");
                
        /* Loop for the ball moves the ball at a slower rate as U_DELAY */
        if(loop % 60 == 0) {
            clear();
            
            /* Add the ball */
            move(ball.y, ball.x);
            addch('O');
            
            /* Right edge of the window */
            if(ball.x >= w.ws_col) {
                ball.x = w.ws_col;
                x_direction = LEFT;
            }
        
            /* Left edge of the window */
            if(ball.x <= 0)
                x_direction = RIGHT;
            
            /* X direction */
            x_direction == RIGHT ? ball.x++ : ball.x--;
        
            /* Bottom edge of the window, check if pad is under the ball */
            if( ball.y == (w.ws_row-2) && (ball.x >= pad.x && ball.x <= (pad.x+6)) ) {
                y_direction = UP;
            }
            /* Ball is under the pad, reset the ball */
            else if(ball.y > w.ws_row-2) {
                ball.x = rand() % w.ws_col;
                ball.y = 0;
            }
            
            /* Top edge of the window */
            if(ball.y <= 0)
                y_direction = DOWN;
        
            /* Y direction */
            y_direction == DOWN ? ball.y++ : ball.y--;            
        }
        
        /* Refresh the screen */
        refresh();

        /* Delay in microseconds*/
        usleep(U_DELAY);
        
        loop++;
        
        /* Get keyboard input */
        input = getch();
        
        /* Right key is pressed */
        if( input == KEY_RIGHT && pad.x < (w.ws_col-6) ) { 
            /* Clear the pad's first character to avoid the pad to grow */
            move(pad.y, pad.x);
            addch(' ');
            
            pad.x++;
        }
        
        /* Left key is pressed */
        if( input == KEY_LEFT && pad.x > 0 ) {
            /* Clear the pad's last character to avoid the pad to grow */
            move(pad.y, pad.x + 5);
            addch(' ');
            
            pad.x--;
        }
    }
    
    delwin(mainwin);
    endwin();
    
    return(EXIT_SUCCESS);
}