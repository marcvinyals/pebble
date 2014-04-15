#!/usr/bin/env python
import networkx as nx
from sys import stdout

def reverse(x, n):
    b = '{:0{width}b}'.format(x, width=n)
    return int(b[::-1], 2)

def bit_reversal_graph(n):
    N=2**n
    G=nx.DiGraph()
    G.add_path(xrange(0,N))
    G.add_path(xrange(N,2*N))
    for i in xrange(N):
        G.add_edge(i,N+reverse(i,n))
    return G

def digraph_to_kth(G):
        print("{}".format(G.order()))
        for i in G.nodes_iter():
            print i+1,":"," ".join(str(j+1) for j in sorted(G.predecessors(i)))

digraph_to_kth(bit_reversal_graph(3))
