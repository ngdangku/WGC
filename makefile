py=python

dep=$(CURDIR)/dependencies
datasets=$(CURDIR)/webgraph_datasets

all: Transform.class

Transform.class: Transform.java
	javac -cp "$(dep)/*" Transform.java

download: Transform.class
	$(py) download_dataset.py

#convert:
#	java -cp "$(dep)/*:." Transform $(datasets)/eu-2005 > eu-2005

clean:
	rm Transform.class
