py=python

dep=$(CURDIR)/dependencies
datasets=$(CURDIR)/webgraph_datasets

all: Transform.class

Transform.class: Transform.java
	javac -cp "$(dep)/*" Transform.java

download: Transform.class
	$(py) download_dataset.py

convert: Transform.class
	#Warning: remove comment and execute following commands
	java -cp "$(dep)/*:." Transform $(datasets)/eu-2005 > eu-2005
	java -cp "$(dep)/*:." Transform $(datasets)/cnr-2000 > cnr-2000

clean:
	rm Transform.class
