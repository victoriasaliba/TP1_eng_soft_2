#include "common.h"

void leEntrada(char *entrada){
    fflush(stdin);
    scanf("%[^\n]%*c", entrada);
}

int main(int argc, char **argv) {
	if(argc < 3) {
		exit(EXIT_FAILURE);
	}

	struct sockaddr_storage storage;
	if(defineProtocolo(argv[1], argv[2], &storage) != 0) {
		exit(EXIT_FAILURE);
	}

	int s_cliente;
	s_cliente = socket(storage.ss_family, SOCK_STREAM, 0);
	if(s_cliente == -1) {
		exit(EXIT_FAILURE);
	}

	struct sockaddr *addr = (struct sockaddr *)(&storage);
	if(connect(s_cliente, addr, sizeof(storage)) != 0) {
		exit(EXIT_FAILURE);
	}

	char mensagem_enviada[BUFSZ] = {0};
	char mensagem_recebida[BUFSZ];
	memset(mensagem_enviada, 0, BUFSZ);
	memset(mensagem_recebida, 0, BUFSZ);

	while(1){
		leEntrada(mensagem_enviada);

		int count_send = send(s_cliente, mensagem_enviada, strlen(mensagem_enviada), 0);
		if (count_send < 1){
			exit(-1);
		}

		memset(mensagem_recebida, 0, sizeof(mensagem_recebida));
		int count_read = read(s_cliente, mensagem_recebida, BUFSZ);
		if (count_read < 1){
			exit(-1);
		}
		printf("%s\n", mensagem_recebida);
	}
}