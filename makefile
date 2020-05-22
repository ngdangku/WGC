py=python

dep=$(CURDIR)/dependencies
datasets=$(CURDIR)/webgraph_datasets
utilities=$(CURDIR)/utilities

all: download convert

Transform.class: utilities
	javac -cp "$(dep)/*" $(utilities)/Transform.java

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
