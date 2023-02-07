BIN = server
GSL_PATH ?=/usr/local
CC = gcc
#CFLAGS = -Wall -Wextra -std=c99 -g
CFLAGS = -Wall -Wextra -std=c99 -g3 -I ${GSL_PATH}/include 

.PHONY: all test build clean

build: ${BIN} test libraries

all: build 

test: tst/* src/*
	${CC} ${CFLAGS} -L${GSL_PATH}/lib -ftest-coverage -fprofile-arcs tst/test_graph.c tst/test_mechanism.c  tst/alltests.c src/mechanism.c src/graph.c src/color.c -lgsl -lgslcblas -lm -o alltests

player1.o: src/player.h src/color.h src/graph.h src/player1.c 
	${CC} ${CFLAGS} -c -fPIC src/player1.c

player2.o: src/player.h src/color.h src/graph.h src/player2.c 
	${CC} ${CFLAGS} -c -fPIC src/player2.c

player3.o: src/player.h src/color.h src/graph.h src/player3.c 
	${CC} ${CFLAGS} -c -fPIC src/player3.c

color.o: src/color.c src/color.h
	${CC} ${CFLAGS} -c src/color.c 

server.o: src/server.c src/player.h src/color.h src/graph.h
	${CC} ${CFLAGS} -c src/server.c -ldl

mechanism.o: src/mechanism.c src/mechanism.h src/graph.h src/color.h
	${CC} ${CFLAGS} -c src/mechanism.c

strategy.o: src/strategy.c src/strategy.h src/mechanism.h src/graph.h src/color.h
	${CC} ${CFLAGS} -c src/strategy.c

graph.o: src/graph.c src/graph.h src/color.h
	${CC} ${CFLAGS} -c src/graph.c

libraries: player1.o player2.o player3.o strategy.o mechanism.o
	${CC} strategy.o mechanism.o -o libplayer1.so -shared player1.o
	${CC} strategy.o mechanism.o -o libplayer2.so -shared player2.o
	${CC} strategy.o mechanism.o -o libplayer3.so -shared player3.o

server: server.o color.o graph.o mechanism.o strategy.o
	${CC} -L${GSL_PATH}/lib server.o color.o mechanism.o strategy.o graph.o -lgsl -lgslcblas -lm -o $@ -ldl

clean:
	rm -f *.o ${BIN} ${BIN2} alltests *.so *.gcda

install: server
	make test
	make libraries
	cp *.so alltests server install
	make clean
