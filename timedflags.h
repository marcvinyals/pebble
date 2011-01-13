/*
   Copyright (C) 2010, 2011 by Massimo Lauria <lauria.massimo@gmail.com>

   Created   : "2011-01-11, martedì 23:53 (CET) Massimo Lauria"
   Time-stamp: "2011-01-13, giovedì 16:33 (CET) Massimo Lauria"

   Description::




*/


/* Preamble */
#ifndef  TIMEDFLAGS_H
#define  TIMEDFLAGS_H


#include <signal.h>

/* Flags */
extern volatile sig_atomic_t print_running_stats_flag;
extern unsigned int timedflags_clock_freq;

/* Flags installer */
extern void install_timed_flags(unsigned int freq);

#endif /* TIMEDFLAGS_H */