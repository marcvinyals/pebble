/*
   Copyright (C) 2010 by Massimo Lauria <lauria.massimo@gmail.com>

   Created   : "2010-12-16, giovedì 17:03 (CET) Massimo Lauria"
   Time-stamp: "2010-12-16, giovedì 19:02 (CET) Massimo Lauria"

   Description::

   Implementation of Direct acyclic graph functionality.


*/

/* Preamble */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "dag.h"


/* Code */

/*
   Graph structure is mostly used statically. Once the graph has been
   defined, it is not modified anymore.  Thus we can do precomputing
   of some information.  */
void dag_precompute_data(DAG *digraph) {

  ASSERT_NULL(digraph->sinks);
  ASSERT_NULL(digraph->sources);

  /* Count the number of sources */
  digraph->sink_number=0;
  digraph->source_number=0;

  for(size_t i=0;i<digraph->size;i++) {
    if (digraph->indegree[i]  == 0) digraph->source_number++;
    if (digraph->outdegree[i] == 0) digraph->sink_number++;
  }

  ASSERT_TRUE(digraph->source_number>0);
  ASSERT_TRUE(digraph->sink_number  >0);

  /* Allocate the space for source and sink arrays */
  digraph->sinks=(Vertex*)malloc(sizeof(Vertex)*(digraph->sink_number));
  digraph->sources=(Vertex*)malloc(sizeof(Vertex)*(digraph->source_number));

  /* Now save the sources and sinks */
  size_t source_extend=0;
  size_t sink_extend=0;
  for(size_t i=0;i<digraph->size;i++) {
    if (digraph->indegree[i] == 0) digraph->sources[source_extend++]=i;
    if (digraph->outdegree[i] == 0) digraph->sinks[sink_extend++]=i;
  }

}


/* A classic case for pebbling is the piramid graph */
/* {{{ */ DAG* create_piramid_graph(int h) {
/*

           14
          / \
         12   13
        / \ / \
       9   10  11      this graph has HEIGHT 4
      / \ / \ / \
     5   6   7   8
    / \ / \ / \ / \
   0   1   2   3   4

*/

  int i,j,v;
  DAG *d=(DAG*)malloc(sizeof(DAG));
  ASSERT_NOTNULL(d);

  /* Allocation of degree information */
  d->size = (h+2)*(h+1) / 2;

  d->in  = (Vertex**)malloc( d->size*sizeof(Vertex*) );
  d->out = (Vertex**)malloc( d->size*sizeof(Vertex*) );

  ASSERT_NOTNULL(d->in );
  ASSERT_NOTNULL(d->out);

  d->indegree  = (size_t*)calloc( d->size,sizeof(size_t) );
  d->outdegree = (size_t*)calloc( d->size,sizeof(size_t) );

  ASSERT_NOTNULL(d->indegree );
  ASSERT_NOTNULL(d->outdegree);

  /* Computing of degree information */
  v=0;
  for (i=0; i < h; ++i)
  {
    d->outdegree[v]=1;      /* First of each level has only an arc */
    v++;
    for (j = 1; j < h-i; ++j) {
      d->outdegree[v]=2;
      v++;
    }
    d->outdegree[v]=1;     /* Last of each level has only an arc */
    v++;
  }
  for(v=h+1;v<d->size;v++) d->indegree[v]=2; /* Indegree is 2 for any non source */

  /* Allocation of neighbour informations */
  for(v=0;v<d->size;v++) {
    d->in[v]  = (Vertex*)malloc( (d->indegree[v]) *sizeof(Vertex) );
    d->out[v] = (Vertex*)malloc( (d->outdegree[v])*sizeof(Vertex) );
    ASSERT_NOTNULL(d->in [v]);
    ASSERT_NOTNULL(d->out[v]);
  }

  /* Fix the incoming vertices */
  v=0;
  for (i=0; i < h; ++i)
  {
    /* First of each level has only an arc */
    d->out      [v][0]=v+h+1-i;
    d->in [v+h+1-i][0]=v      ;
    v++;
    for (j = 1; j < h-i; ++j) {

      d->out      [v][0]=v+h-i;   /* Left arc */
      d->in [v+h  -i][1]=v;

      d->out      [v][1]=v+h+1-i; /* Right arc */
      d->in [v+h+1-i][0]=v;
      v++;
    }
    d->out      [v][0]=v+h-i;  /* Last of each level has only an arc */
    d->in [v+h  -i][1]=v;
    v++;
  }

  dag_precompute_data(d);
  return d;
}
/* }}} */


/* This builds a copy of a DAG.

   N.B. The clone of an inconsistent graph maybe inconsistent, but the
   inconsistency may be different. */

/* {{{ */ DAG* clone_graph(DAG *src) {

  DAG *d;

  if (!src) return NULL;

  d=(DAG*)malloc(sizeof(DAG));
  ASSERT_NOTNULL(d);

  /* Allocation for arcs specification */
  d->size = src->size;

  d->in  = (Vertex**)malloc( d->size*sizeof(Vertex*) );
  d->out = (Vertex**)malloc( d->size*sizeof(Vertex*) );

  ASSERT_NOTNULL(d->in );
  ASSERT_NOTNULL(d->out);

  /* Copy the degree specifications */
  d->indegree  = (size_t*)malloc( d->size*sizeof(size_t) );
  d->outdegree = (size_t*)malloc( d->size*sizeof(size_t) );

  ASSERT_NOTNULL(d->indegree);
  ASSERT_NOTNULL(d->outdegree);

  memcpy(d->indegree,src->indegree,d->size);
  memcpy(d->outdegree,src->outdegree,d->size);

  /*
     If the original graph is not well formed (e.g. it has outgoing
     edges but not the respective incoming edges) we copy the same
     defects.
  */
  /* Allocation of neighbour informations; INDEGREE */
  Vertex v;
  for(v=0;v<d->size;v++) {

    if (d->indegree[v]==0) { d->in[v]=NULL; continue; }

    d->in[v]  = (Vertex*)malloc( (d->indegree[v]) *sizeof(Vertex) );
    ASSERT_NOTNULL(d->in [v]);
    ASSERT_NOTNULL(src->in [v]);
    memcpy(d->in[v],src->in[v],d->size);
  }

  /* Allocation of neighbour informations; OUTDEGREE */
  for(v=0;v<d->size;v++) {

    if (d->outdegree[v]==0) { d->out[v]=NULL; continue; }

    d->out[v] = (Vertex*)malloc( (d->outdegree[v])*sizeof(Vertex) );
    ASSERT_NOTNULL(d->out[v]);
    ASSERT_NOTNULL(src->out[v]);
    memcpy(d->out[v],src->out[v],d->size);
  }

  dag_precompute_data(d);
  return d;
}
/* }}} */


/* Destroys a DAG structure, freeing memory */
/* {{{ */ void dispose_graph(DAG *p) {


  /* Ignore null graphs */
  if (p==NULL) return;

  Vertex v; /* index variable */

  /* Remove incoming arcs arrays */
  if (p->in!=NULL) {
    for(v=0;v<p->size;v++)
      if (p->in[v]!=NULL) free(p->in[v]);
    free(p->in);
  }
  /* Remove outgoing arcs arrays */
  if (p->out!=NULL) {
    for(v=0;v< p->size ;v++)
      if (p->out[v]!=NULL) free(p->out[v]);
    free(p->out);
  }

  /* Dispose other arays */
  if (p->indegree  !=NULL) free(p->indegree );
  if (p->outdegree !=NULL) free(p->outdegree);
  if (p->sources   !=NULL) free(p->sources);
  if (p->sinks     !=NULL) free(p->sinks  );

  /* Dispose the main data structure */
  free(p);
}
/* }}} */

/* This is the default way a label is assigned to a vertex idx Given a
   buffer and a size limit, it writes on the buffer the label for the
   vertex v.  The default impletemtation is to map numbers in their
   string representation.
 */
extern int snprintf(char* buf,size_t size, const char *format, ... );
static void default_vertex_label_hash(char* buf,size_t l,Vertex v) {
  snprintf(buf,l,"%lud",v);
}


/* Prints a string representation of the DAG */
/* {{{ */ void print_graph(DAG *p, void (*vertex_label_hash)(char*,size_t,Vertex)) {
  Vertex v,w;
  char label_buffer[20];

  /* Ignore null graphs */
  if (!p) return;

  ASSERT_NOTNULL(p->indegree);
  ASSERT_NOTNULL(p->in);
  ASSERT_NOTNULL(p->outdegree);
  ASSERT_NOTNULL(p->out);

  /* If no fucntion for computing labels is specified, then we use the
     default one, which turn a number in its string respresentation */
  if (!vertex_label_hash) vertex_label_hash=default_vertex_label_hash;

  for (v = 0; v < p->size; ++v) {

    vertex_label_hash(label_buffer,20,v);
    printf("%s ",label_buffer);
    /* Incoming edges */
    printf("I:");
    for(w=0;w < p->indegree[v]; w++) {
      vertex_label_hash(label_buffer,20,p->in[v][w]);
      printf(" %s",label_buffer);
    }

    /* Outgoing edges */
    printf("O:");
    for(w=0;w < p->outdegree[v]; w++) {
      vertex_label_hash(label_buffer,20,p->out[v][w]);
      printf(" %s",label_buffer);
    }

    printf("\n");
  }
}
/* }}} */



/* Prints a representation of the DAG, which can be used by DOT and
 * graphviz.
 */
void print_dot_graph(DAG *p,
                     char *name,
                     char* options,
                     char **vertex_options,
                     void (*vertex_label_hash)(char*,size_t,Vertex) ) {
  Vertex i,j;
  char label_buffer[20];

  /* Ignore null graphs */
  if (!p) return;

  ASSERT_NOTNULL(p->indegree);
  ASSERT_NOTNULL(p->in);
  ASSERT_NOTNULL(p->outdegree);
  ASSERT_NOTNULL(p->out);

  /* If no function for computing labels is specified, then we use the
   * default one, which turn a number in its string
   * representation. (setq c-block-comment-prefix "*")
   */
  if (!vertex_label_hash) vertex_label_hash=default_vertex_label_hash;

  printf("digraph %s {\n",name);
  printf("\t rankdir=BT;\n");
  if (options) printf("%s\n",options);



  /* Print the vertex infos */
  for (i = 0; i < p->size; ++i) {

    /* Vertex identifier */
    printf("\t %d [",i);

    /* Start with vertex info */
    vertex_label_hash(label_buffer,20,i);
    printf("label=%s,penwidth=2,shape=circle,style=filled,fixedsize=true",label_buffer);

    /* if there are no vertex dependent options, move to the next
       vertex */
    if (vertex_options && vertex_options[i])
      printf(",%s",vertex_options[i]);
    /*
    if (p->info[i] & BLACK_PEBBLE) {
      printf(",color=gray,fontcolor=white,fillcolor=black");
    } else if (p->info[i] & WHITE_PEBBLE) {
      printf(",color=gray,fontcolor=black,fillcolor=white");
    } else {
      printf(",color=gray,fontcolor=black,fillcolor=lightgray");
      }*/
    printf("]\n");
  }


  for (i = 0; i < p->size; ++i) {

    /* Outgoing edges */
    vertex_label_hash(label_buffer,20,i);
    printf("\t /* Arcs outgoing from %s (key %d)*/ \n",label_buffer,i);

    for(j=0;j<p->outdegree[i];j++)
      printf("\t %d -> %d;\n",i,p->out[i][j]);

    printf("\n");
  }

  printf("}\n");

}
/* }}} */


/* Utility for product_graph */
/* {{{ */ void offset_graph(DAG *p,int offset) {
  int i;
  VertexList *h;

  /* Ignore null graphs */
  if (!p) return;

  /* Incoming edges */
  if (p->in) {
    for (i = 0; i < p->size; ++i) {
      h=p->in[i];
      while(h) { h->idx += offset; h=h->next;}
    }
  }
 /* Outgoing edges */
  if (p->out) {
    for (i = 0; i < p->size; ++i) {
      h=p->out[i];
      while(h) { h->idx += offset; h=h->next;}
    }
  }
}
/* }}} */


/* Build the OR product of two DAGs */
/* {{{ */ DAG *product_graph(DAG *inner,DAG *outer) {
  DAG *p=NULL;
  DAG *h=NULL;
  VertexList *t;
  Vertex x,y;
  int A,B;
  int i;
  if (!inner || !outer) return NULL;
  A=inner->size;
  B=outer->size;

  /* Initial structure */
  p=(DAG*)malloc(sizeof(DAG));
  ASSERT_NOTNULL(p);
  p->size=A*B;
  p->in  = (VertexList**)calloc( A*B, sizeof(VertexList*) );
  p->out = (VertexList**)calloc( A*B, sizeof(VertexList*) );
  p->info= (VertexInfo* )calloc( A*B, sizeof(VertexInfo)  );

  ASSERT_NOTNULL(p->in );
  ASSERT_NOTNULL(p->out);
  ASSERT_NOTNULL(p->info);

  /* Several copies of inner graphs are produced and glued together */
  for(i=0;i<B;i++) {
    h=clone_graph(inner);
    offset_graph(h,i*A);
    memcpy(p->in + i*A, h->in , A*sizeof(VertexList*) );
    memcpy(p->out+ i*A, h->out, A*sizeof(VertexList*) );
    /* Dispose the cloned copy. Notice that the lists in the cloned copy
       are not freed, since we keep them attached to the product graph
       adjacency list.  */
    free(h->in);
    free(h->out);
    free(h->info);
    free(h);
  }
  /* Add intermediate edges */
  for(i=0;i<B;i++) {
    t=outer->out[i];
    while(t) {
      x=i*A+A-1;
      for( y=A*(t->idx); y < A*(t->idx+1) ; y++ ) add_arc(p,x,y);
      t=t->next;
    }
  }
  return p;
}
/* }}} */