#ifndef BPM_H
#define BPM_H

#include "ncdefines.h"

void  loop             (void);
int   setup            (void);
void  quit             (void);
int   main             (int argv, char **argc);
void  handle_sigwinch  (int signo);
void  reload_sigwinch  (struct termstore *term_xy);

#endif
