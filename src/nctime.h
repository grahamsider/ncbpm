#ifndef NC_TIME_H
#define NC_TIME_H

#include "ncdefines.h"

void  store_time       (long unsigned *t_sec, long unsigned *t_usec,
                        const struct timeval *t_struct);
void  beat_init        (struct timestore *t_store, const struct timeval *t_now,
                        const struct termstore *term_xy, int *count);
void  beat_update      (struct timestore *t_store, const struct timeval *t_now,
                        const struct termstore *term_xy, int *count);
bool  timeout_rst      (const long unsigned t_prev_sec, const long unsigned t_prev_usec,
                        const struct timeval *t_now);

#endif
