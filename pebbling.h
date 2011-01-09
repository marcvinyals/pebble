/*
   Copyright (C) 2010 by Massimo Lauria <lauria.massimo@gmail.com>

   Created   : "2010-12-17, venerdì 12:03 (CET) Massimo Lauria"
   Time-stamp: "2010-12-20, lunedì 16:32 (CET) Massimo Lauria"

   Description::

   Pebbling data structure.


*/


/* Preamble */
#ifndef  PEBBLING_H
#define  PEBBLING_H

#include <stdlib.h>
#include <limits.h>
#include "common.h"
#include "dag.h"

#define MAX_VERTICES sizeof(BitTuple)*CHAR_BIT

/* Code */

/*  A pebble configuration is represented by a bitmask of at most
 *  MAX_VERTICES elements.  This allows to represent the pebbling in
 *  small space, which is the most important thing since the
 *  configuration may be a large number.
 */
typedef struct _PebbleConfiguration {

  /* Since the configuration space can be large, we use integers of
   specific size for as bit tuples of fixed length.  The specific
   integer type is defined here. */
  BitTuple white_pebbled;    /* White pebbled vertices */
  BitTuple black_pebbled;    /* Black pebbled vertices */

  int pebbles;  /* Number of pebbles in the pebbling */

  /* Information regarding the  configuration transition:  what is the
     pebble cost  needed to reach  the present configuration?  What is
     the actual  previous configuration? What was the  vertex we acted
     on to reach the present configuration? */
  int pebble_cost;
  struct _PebbleConfiguration *previous_configuration;
  Vertex last_changed_vertex;

} PebbleConfiguration;

extern PebbleConfiguration*  new_PebbleConfiguration();
extern PebbleConfiguration* copy_PebbleConfiguration(const PebbleConfiguration *src);
extern void              dispose_PebbleConfiguration(PebbleConfiguration *ptr);
extern Boolean      isconsistent_PebbleConfiguration(const DAG *graph,const PebbleConfiguration *ptr);


/* Discover vertex status */
extern Boolean isblack  (const Vertex v, const DAG *g, const PebbleConfiguration *c);
extern Boolean iswhite  (const Vertex v, const DAG *g, const PebbleConfiguration *c);
extern Boolean ispebbled(const Vertex v, const DAG *g, const PebbleConfiguration *c);
extern Boolean isactive (const Vertex v, const DAG *g, const PebbleConfiguration *c);

extern void print_dot_Pebbling(const DAG *g, PebbleConfiguration *peb,
                        char *name,char* options);


#endif /* PEBBLING_H */