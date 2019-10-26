JARNAME=m19-core

all:
	(cd src; javac -cp ../../../po-uuilib/po-uuilib.jar:../../m19-app/m19-app.jar -encoding UTF-8 `find . -name \*.java`)
	(cd src; jar cf ../$(JARNAME).jar `find . -name \*.class -o -name \*.java`)

clean:
	$(RM) $(JARNAME).jar `find . -name \*.class`
