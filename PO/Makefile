CLASSPATH=po-uuilib/po-uuilib.jar:project/m19-core/m19-core.jar:project/m19-app/m19-app.jar

all:
	#(cd po-uuilib; make all)
	(cd project/m19-core; make all)
	(cd project/m19-app; make all)

run:
	java -cp $(CLASSPATH) m19.app.App

clean:
	#(cd po-uuilib; make clean)
	(cd project/m19-core; make clean)
	(cd project/m19-app; make clean)
