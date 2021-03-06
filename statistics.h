/*
   Copyright (C) 2010, 2011, 2012, 2013 by Massimo Lauria <lauria.massimo@gmail.com>

   Created   : "2011-01-12, mercoledì 17:38 (CET) Massimo Lauria"
   Time-stamp: "2013-09-18, 10:17 (CEST) Massimo Lauria"

   Description::

   Utilities for statistics about the running time.


*/


/* Preamble */
#ifndef  STATISTICS_H
#define  STATISTICS_H

#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "timedflags.h"
/* Code */


typedef struct {

  Counter clock;

  /* Counters for interval amounts */
  Counter processed;              /* Element processed */
  Counter queued;                 /* Element inserted in the queue */
  Counter queued_and_discarded;   /* Element inserted in the queue */
  Counter first_queuing;          /* Queued elements encountered for the first time */
  Counter delayed;                /* Elements queued for the next round */

  Counter requeuing;              /* Old elements pushed back in the queue */
  Counter above_upper_bound;      /* Element above the upper bounds */
  Counter suboptimal;             /* Element discarded because better path exists */
  Counter offspring;              /* Element generated by a processed element */
  Counter final;                  /* Number of final configuration met */

  /* Counters for totals */
  Counter processed_T;              /* Element processed */
  Counter queued_T;                 /* Element inserted in the queue */
  Counter queued_and_discarded_T;   /* Element inserted in the queue */
  Counter first_queuing_T;          /* Queued elements encountered for the first time */
  Counter delayed_T;                /* Elements queued for the next round */

  Counter requeuing_T;              /* Old elements pushed back in the queue */
  Counter above_upper_bound_T;      /* Element above the upper bounds */
  Counter suboptimal_T;             /* Element discarded because better path exists */
  Counter offspring_T;              /* Element generated by a processed element */
  Counter final_T;                  /* Number of final configuration met */

  /* Dictionary queries */
  Counter dict_hops;                     /* Hops per queries */
  Counter dict_hops_T;
  Counter dict_queries;                  /* Number of queries */
  Counter dict_queries_T;

  Counter dict_writes;                   /* Number of writes */
  Counter dict_writes_T;
  Counter dict_misses;                   /* Number of misses */
  Counter dict_misses_T;

  Counter dict_size;

} Statistic;

extern void statistics_make_report(FILE *stream,Statistic *const s);

#if PRINT_STATS_INTERVAL > 0

#define STATS_REPORT(S,fmt,...) {fprintf(stderr,fmt,__VA_ARGS__); statistics_make_report(stderr,&(S));}
#define STATS_CREATE(s) Statistic s={0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0, 0,0,0,0, 0}
#define STATS_INC(s,n) { (s).n++; }
#define STATS_SET(s,n,v) { (s).n=(v); }
#define STATS_ADD(s,n,v) { (s).n+=(v); }
#define STATS_GET(s,n)   ((s).n)

#define STATS_TIMER_OFF  print_running_stats_flag
#define STATS_TIMER_RESET()  { print_running_stats_flag=0; }
#define STATS_CLOCK_UPDATE(s) { (s).clock+=timedflags_clock_freq; }
  
#else

#define STATS_REPORT(S,fmt,...) {}
#define STATS_CREATE(s) { }
#define STATS_INC(s,n) { }
#define STATS_SET(s,n,v) { }
#define STATS_ADD(s,n,v) { }
#define STATS_GET(s,n) { }

#define STATS_TIMER_OFF  0
#define STATS_TIMER_RESET()  { }
#define STATS_CLOCK_UPDATE(s) { }
  
#endif

#endif /* STATISTICS_H */




