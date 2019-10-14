all:
	(cd po-uuilib; make $(MFLAGS) all)
	(cd project/m19-core; make $(MFLAGS) all)
	(cd project/m19-app; make $(MFLAGS) all)

run:
	java -cp po-uuilib/po-uuilib.jar:project/m19-core/m19-core.jar:project/m19-app/m19-app.jar m19.app.App

clean:
	(cd po-uuilib; make $(MFLAGS) clean)
	(cd project/m19-core; make $(MFLAGS) clean)
	(cd project/m19-app; make $(MFLAGS) clean)

