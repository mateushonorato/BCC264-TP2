all:
	gcc src/Processos.c -o bin/processos
	gcc src/Threads.c -o bin/threads

clean:
	rm bin/* -rf