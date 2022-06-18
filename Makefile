all:
	rm *.o client server || echo "Nothing to remove..."
	gcc -Wall -c common.c
	gcc -Wall -c inc_server.c
	gcc -Wall client.c common.o -o client
	gcc -Wall server.c common.o inc_server.o -o server

testes:
	rm *.o || echo "No test to remove..."
	gcc -Wall -c common.c
	gcc -Wall -c inc_server.c
	gcc -Wall client.c common.o -o client
	gcc -Wall server.c common.o inc_server.o -o server
	gcc -Wall testes.c common.o inc_server.o -o testes
	./testes

clean:
	rm *.o client server