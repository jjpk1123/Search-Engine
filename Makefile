CC = g++
CFLAGS = -Wall -c
LFLAGS = -Wall
OBJS = main.o Corpus.o Document.o Stemmer.o
INCLUDE = -I.

PA9 : $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(INCLUDE) -g -o PA9

main.o : main.cpp Corpus.h Document.h Stemmer.h
	$(CC) main.cpp $(INCLUDE) $(CFLAGS) -g

Corpus.o : Corpus.cpp Corpus.h
	$(CC) Corpus.cpp $(INCLUDE) $(CFLAGS) -g
	
Document.o : Document.cpp Document.h
	$(CC) Document.cpp $(INCLUDE) $(CFLAGS) -g
	
Stemmer.o : Stemmer.cpp Stemmer.h
	$(CC) Stemmer.cpp $(INCLUDE) $(CFLAGS) -g

clean :
	-rm *.o PA9
