#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

#include "nctime.h"
#include "nchelpers.h"

#define USEC_CONV 1000000
#define MIN_CONV  60
#define USEC_RST  3000000

void beat_init (struct timestore *t_store, const struct timeval *t_now,
                const struct termstore *term_xy, int *count)
{
    store_time(&(t_store->t0_sec), &(t_store->t0_usec), t_now);
    store_time(&(t_store->t_prev_sec), &(t_store->t_prev_usec), t_now);
    print_bpm_unkwn(++(*count), term_xy->x_cntr, term_xy->y_cntr);
    clear_help(term_xy->y_max);
    print_help(term_xy->x_max, term_xy->y_max);
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
