#ifndef NC_HELPERS_H
#define NC_HELPERS_H

void  print_bpm        (const double bpmf, const int bpmi, const int count, const int x, const int y);
void  print_bpm_unkwn  (const int count, const int x, const int y);
void  print_help       (const int x, const int y);
void  clear_bpm        (const int y);
void  clear_help       (const int y);
void  mvclrln          (const int y);

#endif
