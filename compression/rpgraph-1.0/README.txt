
RePair-based graph representation:
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

Compressed Web graph representation based on Re-Pair compression algorithm.
More information can be found in
http://www.cs.uwaterloo.ca/~fclaude/docs/tweb10.pdf

This version uses differences and bitmaps for representing the pointers, both
the C and R_S are represented using a compact array representation.

The software is under GNU/GPL v3 and comes without any warranty.


Compiling:
-=-=-=-=-

For compiling execute 'make'. With 'make clean' you can erase temporary and
binary files, leaving only the code included in the tar file (plus
compressGraph.py and this README).

Binaries Included:
-=-=-=-=-=-=-=-=-

There are 7 binary files generated:

For compressing a graph we have to use the following two programs:

-buildIndex: given an input graph, generates a temporary representation based
    on Re-Pair. This representation can't be used directly, its output should 
    be processed by compressDictionary. The input format is discussed in the
    next section.
-compressDictionary: takes the output from buildIndex and generates the 
    .rpgraph file, which is the compressed representation that can be used by
    the binaries use and rebuild.

A script encapsulating this functionality, and that also removes the
temporary files, is available (compressGraph.py).

For accessing the compressed representation we use the two following binaries:

-use: and interactive shell for consulting adjacency lists in the compressed
    representation.
-rebuid: takes the compressed graph and generates a fle with the same prefix
    adding .rebuilt to the name. This is the uncompressed representation of
    the graph.

Our implementation is limited to 3GB of RAM usage per graph. In the case when
we want to compress larger graphs we have to cut it in pieces. The binaries
used in this case are:

-cutGraph: takes an uncompressed graph and generated cuts limiting the number
    of edges in each cut (the cut si simply taking contiguous range of nodes
    that do not exceed the limit of edges given their adjacency lists). This
    utility also generates an offset file, required to access the graph in
    compressed form. Each cut file generated has to be processed with 
    buildIndex and compressDictionary (or compressGraph.py).

For accessing a set of
-useMulti: same use as use but reads many pieces.
-rebuildMulti: same as rebuild, also considering many pieces.


Input Format:
-=-=-=-=-=-=

The input format for the graphs is a binary file containing a 4 bytes field
that represents the number of nodes in the graph, an 8 bytes field representing
the number of edges. After those two values we concatenate the representation
of all lists. The representation for the list of node i is:

- 4 bytes field with value (-i).
- 4 bytes fields per node pointed from node i.

An utility for transforming a graph from the WebGraph project is provided
in http://www.cs.uwaterloo.ca/~fclaude/software.html. The crawls from the
WebGraph project (and their software) can be downloaded from
http://webgraph.dsi.unimi.it/


Usage example:
-=-=-=-=-=-=-

** For compressing a graph we use 'compressGraph.py'
$ ./compressGraph.py eu-2005 20298783 1000000
Nodes: 862664
Edges: 19235140
Nodes read: 862664
Graph read, starting compression.
Iteration 1:
    * pairs counted
    * pairs replaced
    * sequence rearranged
    * |C|/|T|: 0.732586
...
Iteration 14:
    * pairs counted
    * pairs replaced
    * sequence rearranged
Compression finished.
Saving files.
Files saved.
buildIndex done: 0
compressDictionary done: 0
Removing temporary files
$

This generates the file eu-2005.rpgraph

$ ls -l eu-2005.rpgraph
rw-r--r-- 1 fclaude fclaude 11888028 2010-05-06 17:44 eu-2005.rpgraph

** For rebuilding (testing correctness) use 'rebuild'
$ ./rebuild eu-2005
Nodes: 862664
Edges: 19235140
Graph size: 11915987
Bits per link: 4.95592
Max outdegree: 6985
$ diff eu-2005.rebuilt eu-2005
$

** An interactive console is provided by 'use':

$ ./use eu-2005
Graph size: 11915987
Bits per link: 4.95592
Insert node ids to query adjacency lists, 0 ends the console
> 3
Outdegree: 30
Adjacency list: 
          25          45          63          66         286
         390         471         492         518         613
        1382        2078        2247        3112        3576
        4036        5453        5472        5521        6037
        6141        6160        6175        6195      273949
      273971      274025      287489      287491      288625
> 200
Outdegree: 63
Adjacency list: 
          59          80         100         120         140
         160         180         220         240         300
         404         485         506         532         641
        1396        1612        1614        2047        2092
        2261        3126        3184        3541        3590
        3957        4050        5467        5472        5535
        6051        6155        6163        6189        6209
      273949      273984      274038      274072      285860
      286107      286329      286403      286515      286525
      286935      287346      287459      287491      287501
      287710      287714      288029      288459      288581
      288592      288623      306572      347385      348262
      349132      349791      417421
> 0
$

** In case we want to cut the graph in pieces we do the following:

$ ./cutGraph eu-2005 5000000 eu-2005_cut
 Nodes: 862664
 Edges: 19235140
 MaxL : 5000000
 Closing file  : eu-2005_cut1
  nodes written: 237003
  edges written: 4999987
  real node    : 237004
 Closing file  : eu-2005_cut2
  nodes written: 220590
  edges written: 4999999
  real node    : 457594
 Closing file  : eu-2005_cut3
  nodes written: 165474
  edges written: 4999995
  real node    : 623068
 Closing file  : eu-2005_cut4
  nodes written: 239597
  edges written: 4235159
  read_node    : 862664
$

this generates 4 files, each of them with at most 5000000 edges
in it. The files are eu-2005_cut[1-4]. Now we compress each cut:

$ for i in 1 2 3 4; do ./compressGraph.py eu-2005_cut$i 20000000 1000; done
[output omitted]

We can list the files that represent this graph:
$ ls -lh *_cut*.rpgraph *.offset
-rw-r--r-- 1 fclaude fclaude 3.7M 2010-05-06 17:52 eu-2005_cut1.rpgraph
-rw-r--r-- 1 fclaude fclaude 2.7M 2010-05-06 17:52 eu-2005_cut2.rpgraph
-rw-r--r-- 1 fclaude fclaude 2.1M 2010-05-06 17:52 eu-2005_cut3.rpgraph
-rw-r--r-- 1 fclaude fclaude 3.0M 2010-05-06 17:52 eu-2005_cut4.rpgraph
-rw-r--r-- 1 fclaude fclaude   20 2010-05-06 17:49 eu-2005_cut.offset

For using this compressed representation we use the binaries 'useMulti' and
'rebuildMulti'. 

$ ./rebuildMulti eu-2005_cut 4
Nodes: 862664
Edges: 19235140
Graph size: 11823233
Bits per link: 4.91735
Max outdegree: 6985
Not written: 0

The useMulti binary file works just as 'use':

$ ./useMulti eu-2005_cut 4
Graph size: 11823233
Bits per link: 4.91735
Insert node ids to query adjacency lists, 0 ends the console
> 3
Outdegree: 30
Adjacency list: 
          25          45          63          66         286
         390         471         492         518         613
        1382        2078        2247        3112        3576
        4036        5453        5472        5521        6037
        6141        6160        6175        6195      273949
      273971      274025      287489      287491      288625
> 200
Outdegree: 63
Adjacency list: 
          59          80         100         120         140
         160         180         220         240         300
         404         485         506         532         641
        1396        1612        1614        2047        2092
        2261        3126        3184        3541        3590
        3957        4050        5467        5472        5535
        6051        6155        6163        6189        6209
      273949      273984      274038      274072      285860
      286107      286329      286403      286515      286525
      286935      287346      287459      287491      287501
      287710      287714      288029      288459      288581
      288592      288623      306572      347385      348262
      349132      349791      417421
> 0


