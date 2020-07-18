#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <curses.h>

#include "ncbpm.h"
#include "nchelpers.h"
#include "nctime.h"

static bool sw_dtct;

void loop (void)
{
    struct timeval t_now;
    struct timestore t_store = {0, 0, 0, 0};
    struct termstore term_xy = { COLS - 1, LINES - 1, (COLS - 1) / 2, (LINES - 1) / 2 };

    int i = 0;
    int ch;

    // TUI Init
    print_bpm_unkwn(i, term_xy.x_cntr, term_xy.y_cntr);
    print_help(term_xy.x_max, term_xy.y_max);
    refresh();

    while (1) 
    {
        ch = getch();
        gettimeofday(&t_now, NULL);

        // Check sigwinch
        if (sw_dtct)
            reload_sigwinch (&term_xy);

        // Check quit and keyboard reset
        if (ch == 'q' || ch == 'Q') break;
        if (ch == 'r' || ch == 'R')
        {
            i = 0;
            print_bpm_unkwn(i, term_xy.x_cntr, term_xy.y_cntr);
            clear_help(term_xy.y_max);
            print_help(term_xy.x_max, term_xy.y_max);
        }

        // First beat
        else if (!i)
            beat_init(&t_store, &t_now, &term_xy, &i);

        // Check timeout reset
        else if (timeout_rst(t_store.t_prev_sec, t_store.t_prev_usec, &t_now))
        {
            i = 0;
            beat_init(&t_store, &t_now, &term_xy, &i);
        }

        // Update
        else
            beat_update(&t_store, &t_now, &term_xy, &i);

        refresh();
    }
}

void reload_sigwinch (struct termstore *term_xy)
{
    endwin();
    refresh();
    clear_bpm(term_xy->y_cntr);
    clear_help(term_xy->y_max);
    getmaxyx(stdscr, term_xy->y_max, term_xy->x_max);
    --term_xy->y_max;
    --term_xy->x_max;
    print_help(term_xy->x_max, term_xy->y_max);
    term_xy->x_cntr = term_xy->x_max / 2;
    term_xy->y_cntr = term_xy->y_max / 2;
    sw_dtct = false;
}

void handle_sigwinch (int signo)
{
    sw_dtct = true;
}

int setup (void)
{
    initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    signal(SIGWINCH, handle_sigwinch);

    clear();
    refresh();

    return 0;
}

void quit (void)
{
    endwin();
}

int main (int argv, char **argc)
{
    setup();
    loop();
    quit();

    return 0;
}
