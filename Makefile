all:
	rm *.o client server || echo "Nothing to remove..."
	gcc -Wall -c common.c
	gcc -Wall -c inc_server.c
	gcc -Wall client.c common.o -o client
	gcc -Wall server.c common.o inc_server.o -o server

testes-unitarios:
	rm *.o client server || echo "No test to remove..."
	gcc -Wall -c common.c
	gcc -Wall -c inc_server.c
	gcc -Wall client.c common.o -o client
	gcc -Wall server.c common.o inc_server.o -o server
	gcc -Wall testes.c common.o inc_server.o -o testes
	./testes
	rm testes

testes-sistema:
	@bash run_testes_e2e.sh

clean:
	rm *.o client server

SHELL=/bin/bash