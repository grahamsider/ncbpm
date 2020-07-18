#ifndef DEF_H
#define DEF_H

struct timestore
{
    long unsigned t0_sec;
    long unsigned t0_usec;
    long unsigned t_prev_sec;
    long unsigned t_prev_usec;
};

struct termstore
{
    int x_max;
    int y_max;
    int x_cntr;
    int y_cntr;
};

#endif
