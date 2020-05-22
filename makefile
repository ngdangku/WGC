py=python

dep=$(CURDIR)/dependencies
datasets=$(CURDIR)/webgraph_datasets
utilities=$(CURDIR)/utilities
compression=$(CURDIR)/compression

lz=$(compression)/lzgraph-1.0
rp=$(compression)/rpgraph-1.0

all: download convert

Transform.class: utilities
	javac -cp "$(dep)/*" $(utilities)/Transform.java

.ONESEHLL:
test_lz:
	$(lz)/compressor $(datasets)/eu-2005
	$(lz)/use $(datasets)/eu-2005.lzg sequence
	$(lz)/use $(datasets)/eu-2005.lzg random

#	$(lz)/compressor $(datasets)/cnr-2000
#	$(lz)/use $(datasets)/cnr-2000.lzg sequence
#	$(lz)/use $(datasets)/cnr-2000.lzg random
#
#	$(lz)/compressor $(datasets)/in-2004
#	$(lz)/use $(datasets)/in-2004.lzg sequence
#	$(lz)/use $(datasets)/in-2004.lzg random
#
#	$(lz)/compressor $(datasets)/uk-2007-05@1000000
#	$(lz)/use $(datasets)/uk-2007-05@1000000.lzg sequence
#	$(lz)/use $(datasets)/uk-2007-05@1000000.lzg random

.ONESHELL:
test_rp:
	cd $(rp)
	$(py) compressGraph.py $(datasets)/eu-2005 20298783 1000000
	$(rp)/use $(datasets)/eu-2005 sequence
	$(rp)/use $(datasets)/eu-2005 random

#	$(py) compressGraph.py $(datasets)/cnr-2000 20298783 1000000
#	$(rp)/use $(datasets)/cnr-2000 sequence
#	$(rp)/use $(datasets)/cnr-2000 random

#	$(py) compressGraph.py $(datasets)/in-2004 20298783 1000000
#	$(rp)/use $(datasets)/in-2004 sequence
#	$(rp)/use $(datasets)/in-2004 random
#
#	$(py) compressGraph.py $(datasets)/uk-2007-05@1000000 20298783 1000000
#	$(rp)/use $(datasets)/uk-2007-05@1000000 sequence
#	$(rp)/use $(datasets)/uk-2007-05@1000000 random

download:
	$(py) -m utilities.download_dataset

visualise:
	$(py) -m visualisation.AM

.ONESHELL:
convert: Transform.class
	#Warning: remove comment and execute following commands
	cd $(utilities)
	java -cp "$(dep)/*:." Transform $(datasets)/eu-2005  > $(datasets)/eu-2005
	java -cp "$(dep)/*:." Transform $(datasets)/cnr-2000 > $(datasets)/cnr-2000
	java -cp "$(dep)/*:." Transform $(datasets)/in-2004  > $(datasets)/in-2004
	java -cp "$(dep)/*:." Transform $(datasets)/uk-2007-05@1000000 > $(datasets)/uk-2007-05@1000000

clean:
	rm Transform.class
