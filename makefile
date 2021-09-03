major2: major2.o path.o exit.o history.o pipe.o batch.o alias.o cd.o shell.o
	gcc -o major2 major2.o path.o shell.o exit.o history.o alias.o pipe.o batch.o cd.o -W;

major2.o: major2.c cd.c exit.c history.c pipe.c batch.c path.c alias.c shell.c major2.h
	gcc -c major2.c;

cd.o: cd.c major2.h
	gcc -c cd.c;

exit.o: exit.c major2.h
	gcc -c exit.c;

history.o: history.c major2.h
	gcc -c history.c;

path.o: path.c major2.h
	gcc -c path.c;

shell.o: shell.c major2.h
	gcc -c shell.c;

alias.o: alias.c major2.h
	gcc -c alias.c;

batch.o: batch.c major2.h
	gcc -c batch.c;

pipe.o: pipe.c major2.h
	gcc -c pipe.c;

clean:
	rm major2 *.o;

debug:
	gcc -g -o major2dbg major2.o path.o shell.o exit.o history.o cd.o;
