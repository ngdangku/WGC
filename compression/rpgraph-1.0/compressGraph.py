#!/usr/bin/python

import sys,os

if len(sys.argv)!=4:
  print("usage: "+sys.argv[0]+" <graph> <memory (ints)> <k>")
  sys.exit(0)

res=os.system("./buildIndex "+sys.argv[1]+" "+sys.argv[2]+" "+sys.argv[3])
print("buildIndex done: "+str(res))

if res!=0:
  print("buildIndex failed, aborting")
  sys.exit(1)

res=os.system("./compressDictionary "+sys.argv[1])
print("compressDictionary done: "+str(res))

if res!=0:
  print("compressDictionary failed, aborting")
  sys.exit(1)

print("Removing temporary files")
for ext in ["rp","dict","ptr"]:
  res=os.system("rm -f "+sys.argv[1]+"."+ext)
