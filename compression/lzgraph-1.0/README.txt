
Lempel-Ziv-based graph representation:
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

Compressed Web graph representation based on Lempel-Ziv compression algorithm.
More information can be found in 
http://www.cs.uwaterloo.ca/~fclaude/docs/tweb10.pdf

This version uses differences and bitmaps for representing the pointers.

The software is under GNU/GPL v3 and comes without any warranty.

Compiling:
-=-=-=-=-

For compiling execute 'make'. With 'make clean' you can erase temporary and
binary files, leaving only the code included in the tar file.

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

** For compressing a graph we use 'compressor'
$ ./compressor eu-2005 
* nodes=862664
* edges=19235140
Building structure...done
Compressing data...done
Phrases generated: 2928685
Saving file...done
$

this generates the file eu-2005.lzg
$ ls -l eu-2005.lzg 
-rw-r--r-- 1 fclaude fclaude 17747992 2010-03-02 15:27 eu-2005.lzg

** For rebuilding (testing correctness) use 'rebuild'
$ ./rebuild eu-2005.lzg 
Nodes: 862664
Edges: 19235140
Graph size: 17775908
Bits per link: 7.3931
Max outdegree: 6985
$ diff eu-2005.lzg.rebuilt eu-2005
$

** An interactive console is provided by 'use':

$ ./use eu-2005.lzg
Graph size: 17775908
Bits per link: 7.3931
Insert node ids to query adjacency lists, 0 ends the console
> 3
Outdegree: 30
Adjacency list: 
      288625      287491      287489      274025      273971
      273949        6195        6175        6160        6141
        6037        5521        5472        5453        4036
        3576        3112        2247        2078        1382
         613         518         492         471         390
         286          66          63          45          25
> 200
Outdegree: 63
Adjacency list: 
      417421      349791      349132      348262      347385
      306572      288623      288592      288581      288459
      288029      287714      287710      287501      287491
      287459      287346      286935      286525      286515
      286403      286329      286107      285860      274072
      274038      273984      273949        6209        6189
        6163        6155        6051        5535        5472
        5467        4050        3957        3590        3541
        3184        3126        2261        2092        2047
        1614        1612        1396         641         532
         506         485         404         300         240
         220         180         160         140         120
         100          80          59
> 0
$

