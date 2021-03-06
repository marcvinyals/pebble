/*
   Copyright (C) 2010, 2011, 2012 by Massimo Lauria <lauria.massimo@gmail.com>

   Created   : "2010-12-18, sabato 01:24 (CET) Massimo Lauria"
   Time-stamp: "2012-06-04, 02:55 (CEST) Massimo Lauria"

   Description::

   Header file for hashtable.c, a very simple implementation of a dictionary with hashtable.

*/


/* Preamble */
#ifndef  HASHTABLE_H
#define  HASHTABLE_H

#include <stdlib.h>
#include "common.h"
#include "dsbasic.h"

/* Code */

/* The dictionary has a very simple implementation.  It is an array of
   lists, each list is indexed by an hash which is reduced modulo the
   size of the array.  Notice that for performance we statically
   allocate the array, and the actual size is between 15/16 and 16/16
   of the allocated size.

   The dictionary also keeps a list of elements in the buckets. It is
   sorted with respect to the time of insertion in the dictionary.
 */
typedef struct {

  size_t allocation;
  size_t size;

  LinkedList **buckets;

  size_t   (*key_function)(void *data);
  Boolean  ( *eq_function)(void *A,void *B);
  void     (*dispose_function)(void *data);

} Dict;

/*
 * When the  dictionary is queried the  result is simply  the index of
 * the bucket in which the record  hashes, and a pointer to a matching
 * occurrence  in the  dictionary, if  there  is any.  A null  pointer
 * represent no matching record.
 */
typedef struct {

  size_t key;
  void   *value;
  size_t bucket;
  size_t hops;

} DictQueryResult;

extern Dict *newDict(size_t allocation);
extern void disposeDict(Dict *d);

extern Boolean isconsistentDict(Dict *d);

extern void queryDict(Dict* d,DictQueryResult *const result,void *data);
extern void writeDict(Dict *d,DictQueryResult *const result,void *data);
extern void unsafe_noquery_writeDict(Dict *d,DictQueryResult *const result,void *data);

extern void histogramDict(FILE *stream,Dict *d);


#endif /* HASHTABLE_H */
