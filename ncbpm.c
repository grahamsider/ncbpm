#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>
#include <curses.h>
#include <ncbpm.h>

#define USEC_CONV 1000000
#define MIN_CONV  60
#define USEC_RST  3000000
#define X_OFST    8

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
    print_help(term_xy.y_max);

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
        
    }
}

void beat_init (struct timestore *t_store, const struct timeval *t_now,
                const struct termstore *term_xy, int *count)
{
    store_time(&(t_store->t0_sec), &(t_store->t0_usec), t_now);
    store_time(&(t_store->t_prev_sec), &(t_store->t_prev_usec), t_now);
    print_bpm_unkwn(++(*count), term_xy->x_cntr, term_xy->y_cntr);
}

void beat_update (struct timestore *t_store, const struct timeval *t_now,
                  const struct termstore *term_xy, int *count)
{
    unsigned t_elapsed_usec_total = (((t_now->tv_sec - t_store->t0_sec) * USEC_CONV) \
                                    + (t_now->tv_usec - t_store->t0_usec));
    double t_elapsed_min_total = ((double) t_elapsed_usec_total / USEC_CONV) / MIN_CONV;
    double bpmf = (double) (*count) / t_elapsed_min_total;
    unsigned bpmi = (int) (bpmf + 0.5);

    print_bpm(bpmf, bpmi, *count, term_xy->x_cntr, term_xy->y_cntr);
    refresh();

    store_time(&(t_store->t_prev_sec), &(t_store->t_prev_usec), t_now);
    ++(*count);
}

bool timeout_rst (const long unsigned t_prev_sec, const long unsigned t_prev_usec,
                  const struct timeval *t_now)
{
    return (((t_now->tv_sec - t_prev_sec) * USEC_CONV) + (t_now->tv_usec - t_prev_usec)) >= USEC_RST;
}

void store_time (long unsigned *t_sec, long unsigned *t_usec, const struct timeval *t_struct)
{
    *t_sec = t_struct->tv_sec;
    *t_usec = t_struct->tv_usec;
}

void clear_bpm (const int y)
{
    mvclrln(y - 1);
    mvclrln(y);
    mvclrln(y + 1);
}

void print_bpm (const double bpmf, const int bpmi, const int count, const int x, const int y)
{
    clear_bpm(y);

    // TODO: Bold pre-colon text
    mvprintw(y - 1, x - X_OFST, "Average BPM:   %9.6f", bpmf);
    mvprintw(y,     x - X_OFST, "Nearest Whole: %d", bpmi);
    mvprintw(y + 1, x - X_OFST, "Beat Count:    %d", count + 1);
}

void print_bpm_unkwn (const int count, const int x, const int y)
{
    clear_bpm(y);

    // TODO: Bold pre-colon text
    mvprintw(y - 1, x - X_OFST, "Average BPM:   ?");
    mvprintw(y,     x - X_OFST, "Nearest Whole: ?");
    mvprintw(y + 1, x - X_OFST, "Beat Count:    %d", count);
}

void clear_help (const int y)
{
    mvclrln(y - 1);
    mvclrln(y);
}

void print_help (const int y)
{
    mvprintw(y - 1, 0, "r = reset");
    mvprintw(y, 0, "q = quit");
}

void mvclrln (const int y)
{
    move(y, 0);
    clrtoeol();
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
    print_help(term_xy->y_max);
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
