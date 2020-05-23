py=python

dep=$(CURDIR)/dependencies
datasets=$(CURDIR)/webgraph_datasets
utilities=$(CURDIR)/utilities
compression=$(CURDIR)/compression
experiments=$(CURDIR)/experiments

lz=$(compression)/lzgraph-1.0
rp=$(compression)/rpgraph-1.0

all: convert

Transform.class: utilities
	javac -cp "$(dep)/*" $(utilities)/Transform.java

.ONESEHLL:
test_lz:
	time $(lz)/compressor $(datasets)/eu-2005
	time $(lz)/use $(datasets)/eu-2005.lzg sequence
	time $(lz)/use $(datasets)/eu-2005.lzg random

#	time $(lz)/compressor $(datasets)/cnr-2000
#	time $(lz)/use $(datasets)/cnr-2000.lzg sequence
#	time $(lz)/use $(datasets)/cnr-2000.lzg random
#
#	time $(lz)/compressor $(datasets)/in-2004
#	time $(lz)/use $(datasets)/in-2004.lzg sequence
#	time $(lz)/use $(datasets)/in-2004.lzg random
#
#	time $(lz)/compressor $(datasets)/uk-2007-05@1000000
#	time $(lz)/use $(datasets)/uk-2007-05@1000000.lzg sequence
#	time $(lz)/use $(datasets)/uk-2007-05@1000000.lzg random

.ONESHELL:
test_rp:
	cd $(rp)
	time $(py) compressGraph.py $(datasets)/eu-2005 214748364 10000000
	time $(rp)/use $(datasets)/eu-2005 sequence
	time $(rp)/use $(datasets)/eu-2005 random

#	time $(py) compressGraph.py $(datasets)/cnr-2000 2147483647 100000000
#	time $(rp)/use $(datasets)/cnr-2000 sequence
#	time $(rp)/use $(datasets)/cnr-2000 random

#	time $(py) compressGraph.py $(datasets)/in-2004 2147483647 100000000
#	time $(rp)/use $(datasets)/in-2004 sequence
#	time $(rp)/use $(datasets)/in-2004 random

#	time $(py) compressGraph.py $(datasets)/uk-2007-05@1000000 2147483647 100000000
#	time $(rp)/use $(datasets)/uk-2007-05@1000000 sequence
#	time $(rp)/use $(datasets)/uk-2007-05@1000000 random

test_webgraph:

	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.BVGraph -m 0 -w 0 -i 0 $(datasets)/eu-2005 $(experiments)/eu-2005-gaps 2>&1 | tee $(experiments)/eu-2005-gaps.stats && \
	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.test.SpeedTest -g BVGraph $(experiments)/eu-2005-gaps 2>&1 | tee $(experiments)/eu-2005-gaps.seqspeedtest && \
	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.test.SpeedTest -g BVGraph -r 100000 $(experiments)/eu-2005-gaps 2>&1 | tee $(experiments)/eu-2005-gaps.randspeedtest

#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.BVGraph -m 0 -w 0 -i 0 $(datasets)/cnr-2000 $(experiments)/cnr-2000-gaps 2>&1 | tee $(experiments)/cnr-2000-gaps.stats && \
#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.test.SpeedTest -g BVGraph $(experiments)/cnr-2000-gaps 2>&1 | tee $(experiments)/cnr-2000-gaps.seqspeedtest && \
#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.test.SpeedTest -g BVGraph -r 100000 $(experiments)/cnr-2000-gaps 2>&1 | tee $(experiments)/cnr-2000-gaps.randspeedtest

#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.BVGraph -m 0 -w 0 -i 0 $(datasets)/in-2004 $(experiments)/in-2004-gaps 2>&1 | tee $(experiments)/in-2004-gaps.stats && \
#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.test.SpeedTest -g BVGraph $(experiments)/in-2004-gaps 2>&1 | tee $(experiments)/in-2004-gaps.seqspeedtest && \
#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.test.SpeedTest -g BVGraph -r 100000 $(experiments)/in-2004-gaps 2>&1 | tee $(experiments)/in-2004-gaps.randspeedtest

#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.BVGraph -m 0 -w 0 -i 0 $(datasets)/uk-2007-05@1000000 $(experiments)/uk-2007-05@1000000-gaps 2>&1 | tee $(experiments)/uk-2007-05@1000000-gaps.stats && \
#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.test.SpeedTest -g BVGraph $(experiments)/uk-2007-05@1000000-gaps 2>&1 | tee $(experiments)/uk-2007-05@1000000-gaps.seqspeedtest && \
#	java -Xss64M -cp "$(dep)/*:." it.unimi.dsi.webgraph.test.SpeedTest -g BVGraph -r 100000 $(experiments)/uk-2007-05@1000000-gaps 2>&1 | tee $(experiments)/uk-2007-05@1000000-gaps.randspeedtest

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

.ONESHELL:
download:
	cd $(CURDIR)
	$(py) -m utilities.download_dataset
