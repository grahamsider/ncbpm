#ifndef BPM_H
#define BPM_H

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

void  loop             (void);
int   setup            (void);
void  quit             (void);
int   main             (int argv, char **argc);
void  handle_sigwinch  (int signo);
void  reload_sigwinch  (struct termstore *term_xy);

void  print_bpm        (const double bpmf, const int bpmi, const int count, const int x, const int y);
void  print_bpm_unkwn  (const int count, const int x, const int y);
void  print_help       (const int y);
void  clear_bpm        (const int y);
void  clear_help       (const int y);
void  mvclrln          (const int y);

void  store_time       (long unsigned *t_sec, long unsigned *t_usec,
                        const struct timeval *t_struct);
void  beat_init        (struct timestore *t_store, const struct timeval *t_now,
                        const struct termstore *term_xy, int *count);
void  beat_update      (struct timestore *t_store, const struct timeval *t_now,
                        const struct termstore *term_xy, int *count);

bool  timeout_rst      (const long unsigned t_prev_sec, const long unsigned t_prev_usec,
                        const struct timeval *t_now);

#endif
