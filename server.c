#include "common.h"
#include "inc_server.h"

extern int quantidadeAtual;

void fechaConexao(int socket){
    close(socket);
    exit(-1);
}


void inicializaMatriz(float(*matriz)[4][4]){
   for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            (*matriz)[i][j] = -1;
        }
    }
}

char* removeSubstringDeString(char *frase, char *palavra){
    int tamanhoPalavra = strlen(palavra);

    char * ponteiro = strstr(frase, palavra);
    if(ponteiro){
        int posicao = (ponteiro - frase);
        ponteiro = ponteiro + tamanhoPalavra;
        int i;
        for(i = 0; i < strlen(ponteiro); i++){
            frase[posicao + i] = ponteiro[i];
        }
        frase[posicao + i] = 0;
    }
    return frase;
}

char* comandoAddSensor(struct EstruturaDeControle *c, float(*matriz)[4][4], char *comando){

    char substring[] = {"add sensor "};
    sprintf(comando, "%s",removeSubstringDeString(comando, substring));

    iniciarEstrutura(c, comando);

    while(strcmp(c->palavra_atual,"in")){
        int sensor_id = atoi(c->palavra_atual);

        if(elementoInvalido(sensor_id)){
            return "invalid sensor";
        }
        if(c->aux + quantidadeAtual >= MAX_SENSORS){
            return "limit exceeded";
        }
        c->sensores_solicitados[c->aux-1] = sensor_id;
        c->palavra_atual = strtok(NULL, " ");
        c->aux++;
    }

    c->palavra_atual = strtok(NULL, " "); //depois do in
    int equipamento_id = atoi(c->palavra_atual);

    if(elementoInvalido(equipamento_id)){
        return "invalid equipament";
    }

    checarSensoresValidos(c, matriz, equipamento_id, 1);

    instalarSensoresValidos(c, matriz, equipamento_id);

    informarSensoresExistentes(c, equipamento_id);

    return c->resposta;
}

char* comandoRemoveSensor(struct EstruturaDeControle *c,float(*matriz)[4][4], char *comando){
    static char resposta_remove[100];
    memset(resposta_remove, 0, 100);

    char substring[] = {"remove sensor "};
    sprintf(comando, "%s",removeSubstringDeString(comando, substring));

    iniciarEstrutura(c, comando);

    while(strcmp(c->palavra_atual,"in")){
        int sensor_id = atoi(c->palavra_atual);

         if(elementoInvalido(sensor_id)){
            return "invalid sensor";
        }

        c->sensores_solicitados[c->aux-1] = sensor_id;
        c->palavra_atual = strtok(NULL, " ");
        c->aux++;
    }

    c->palavra_atual = strtok(NULL, " "); //depois do in
    int equipamento_id = atoi(c->palavra_atual);

    if(elementoInvalido(equipamento_id)){
        return "invalid equipament";
    }

    checarSensoresValidos(c, matriz, equipamento_id, 0);

    removerSensoresValidos(c, matriz, equipamento_id);

    informarSensoresNaoExistentes(c, equipamento_id);

    return c->resposta;

}

char* consultarEquipamento(float(**matriz)[4][4], int equipamentoID){
    int flag = 1;
    char *lista = malloc(sizeof(char));

    for(int sensores=0; sensores<4; sensores++){
        if((**matriz)[equipamentoID-1][sensores] != -1){
            char valor[2] = {'0', sensores+1 + '0'};
            if(flag){
                strcpy(lista, valor);
            }else{
                strcat(lista, " ");
                strcat(lista, valor);
            }
            flag = 0;
        }
    }
    if(flag){
        strcpy(lista, "none");
    }
    return lista;
}

char* comandoListSensors(float(*matriz)[4][4], char *comando){
    static char resposta_list[100];
    memset(resposta_list, 0, 100);

    char substring[] = {"list sensors in "};
    sprintf(comando, "%s",removeSubstringDeString(comando, substring));

    int equipamento_id = atoi(comando);

    if(equipamento_id < 1 || equipamento_id > 4){
        return "invalid equipament";
    }

    strcat(resposta_list, consultarEquipamento(&matriz, equipamento_id));

    return resposta_list;
}

char* comandoRead(struct EstruturaDeControle *c, float(*matriz)[4][4], char *comando){
    char substring[] = {"read "};
    sprintf(comando, "%s",removeSubstringDeString(comando, substring));

    iniciarEstrutura(c, comando);

    while(strcmp(c->palavra_atual,"in")){
        int sensor_id = atoi(c->palavra_atual);

        if(elementoInvalido(sensor_id)){
            return "invalid sensor";
        }

        c->sensores_solicitados[c->aux-1] = sensor_id;
        c->palavra_atual = strtok(NULL, " ");
        c->aux++;
    }

    c->palavra_atual = strtok(NULL, " "); //depois do in
    int equipamento_id = atoi(c->palavra_atual);

    if(elementoInvalido(equipamento_id)){
        return "invalid equipament";
    }

    checarSensoresValidos(c, matriz, equipamento_id, 0);

    lerSensoresValidos(c, matriz, equipamento_id);

    informarSensoresNaoExistentes(c, equipamento_id);

    return c->resposta;
}



int main(int argc, char **argv) {
    if (argc < 3) {
        exit(EXIT_FAILURE);
    }

    struct sockaddr_storage storage;
    if(defineProtocoloServidor(argv[1], argv[2], &storage) != 0){
        exit(EXIT_FAILURE);
    }

    int s_servidor;
    s_servidor = socket(storage.ss_family, SOCK_STREAM, 0);
    if(s_servidor == -1){
        exit(EXIT_FAILURE);
    }

    int enable = 1;
    if(setsockopt(s_servidor, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0){
        exit(EXIT_FAILURE);
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if(bind(s_servidor, addr, sizeof(storage)) != 0){
        exit(EXIT_FAILURE);
    }

    if(listen(s_servidor, 10) != 0) {
        exit(EXIT_FAILURE);
    }

    struct sockaddr_storage cstorage;
    struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
    socklen_t caddrlen = sizeof(cstorage);

    int csock = accept(s_servidor, caddr, &caddrlen);
    if(csock == -1){
        exit(EXIT_FAILURE);
    }

    float matriz[4][4];
    inicializaMatriz(&matriz);


    char mensagem_recebida[BUFSZ] = {0};
    char mensagem_enviada[BUFSZ] = "";

    while (1) {
        memset(mensagem_enviada, 0, BUFSZ);
        memset(mensagem_recebida, 0, BUFSZ);

        int count_read = read(csock, mensagem_recebida, BUFSZ);
        if (count_read < 1){
        		exit(-1);
    	}

        //  Imprime mensagem recebida
        printf("%s\n", mensagem_recebida);

        int acao = retornaAcao(mensagem_recebida);
        int flag_conexao = 1;

        struct EstruturaDeControle c;

        switch (acao){
            case ADD:
                sprintf(mensagem_enviada, "%s", comandoAddSensor(&c, &matriz, mensagem_recebida));
                break;
            case REMOVE:
                sprintf(mensagem_enviada, "%s", comandoRemoveSensor(&c, &matriz, mensagem_recebida));
                break;
            case LIST:
                sprintf(mensagem_enviada, "%s", comandoListSensors(&matriz, mensagem_recebida));
                break;
            case READ:
                sprintf(mensagem_enviada, "%s", comandoRead(&c, &matriz, mensagem_recebida));
                break;
            case KILL:
                fechaConexao(csock);
                break;
            case INVALIDO:
                fechaConexao(csock);
                break;
            default:
                close(csock);
                flag_conexao = 0;
                break;
        }

        if(flag_conexao){
            int count_send = send(csock, mensagem_enviada, strlen(mensagem_enviada), 0);
            if (count_send < 1){
                    exit(-1);
            }
        }else{
            csock = accept(s_servidor, caddr, &caddrlen);
            flag_conexao = 1;
        }
    }

    exit(EXIT_SUCCESS);
}