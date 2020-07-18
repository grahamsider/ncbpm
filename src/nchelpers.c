#include "curses.h"
#include "nchelpers.h"

#define X_OFST 8

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

void print_help (const int x, const int y)
{
    mvprintw(y - 1, 0, "r = reset");
    mvprintw(y, 0, "q = quit");
    mvprintw(y - 1, x - 17, "<-- press key -->");
    mvprintw(y, x - 14, "auto-reset: 3s");
}

void mvclrln (const int y)
{
    move(y, 0);
    clrtoeol();
}
