filename = project1writeup

all: pdf concurrency1.o

pdf: ps
	ps2pdf ${filename}.ps

ps: dvi
	dvips -t letter ${filename}.dvi

dvi:
	latex ${filename}
	bibtex ${filename}||true
	latex ${filename}
	latex ${filename}

concurrency1.o: concurrency1.c mt19937ar.c
	gcc -pthread -o concurrency1.o concurrency1.c mt19937ar.c

clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg *.o *.out *.exe
