#include "common.h"
#include "inc_server.h"

int quantidadeAtual = 0;

void fechaConexao(int socket){
    close(socket);
    exit(-1);
}

int retornaAcao(char *comandoCompleto){
    char comando[BUFSZ];
    sprintf(comando, "%s", comandoCompleto);
    char escape[] = {"kill"};
    char *ponteiro;
    ponteiro = strstr(comando, escape); // procura pela string kill dentro da mensagem recebida
    if(ponteiro){
        return KILL;
    }else{
        char *palavra = strtok(comando, " ");
        if (strcmp(palavra, "add") == 0){
            palavra = strtok(NULL, " ");
            if(strcmp(palavra,"sensor") == 0){
                return ADD;
            }else{
                return INVALIDO;
            }
        }
        else if (strcmp(palavra, "remove") == 0){
            palavra = strtok(NULL, " ");
            if(strcmp(palavra,"sensor") == 0){
                return REMOVE;
            }else{
                return INVALIDO;
            }
        }
        else if (strcmp(palavra, "list") == 0){
            palavra = strtok(NULL, " ");
            if(strcmp(palavra,"sensors") == 0){
                palavra = strtok(NULL, " ");
                if(strcmp(palavra,"in") == 0){
                    return LIST;
                }else{
                    return INVALIDO;
                }
            }else{
                return INVALIDO;
            }
        }
        else if (strcmp(palavra, "read") == 0){
            return READ;
        }

        return INVALIDO;
    }
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

    checarSensoresValidos(c, matriz, equipamento_id);

    instalarSensoresValidos(c, matriz, equipamento_id);

    informarSensoresExistentes(c, equipamento_id);

    return c->resposta;
}

char* comandoRemoveSensor(float(*matriz)[4][4], char *comando){
    static char resposta_remove[100];
    memset(resposta_remove, 0, 100);

    char substring[] = {"remove sensor "};
    sprintf(comando, "%s",removeSubstringDeString(comando, substring));

    char *palavra_atual = strtok(comando, " ");
    int sensores_solicitados[] = {-1, -1, -1, -1};
    int sensores_validos[] = {-1, -1, -1, -1};
    int aux = 1;

    while(strcmp(palavra_atual,"in")){
        int sensor_id = atoi(palavra_atual);

        if(sensor_id < 1 || sensor_id > 4){
            return "invalid sensor";
        }

        sensores_solicitados[aux-1] = sensor_id;
        palavra_atual = strtok(NULL, " ");
        aux++;
    }

    palavra_atual = strtok(NULL, " "); // depois do in
    int equipamento_id = atoi(palavra_atual);

    if(equipamento_id < 1 || equipamento_id > 4){
        return "invalid equipament";
    }

    int flag_aomenos1valido = 0;
    int flag_aomenos1naoexistente = 0;
    // Vê quais sensores são válidos para serem removidos
    for(int i=0; i<4; i++){
        if(sensores_solicitados[i] == -1){
            break;
        }
        sensores_validos[i] = boolSensorJaInstalado(&matriz, sensores_solicitados[i], equipamento_id);
        if(sensores_validos[i]){
            flag_aomenos1valido = 1;
        }
        if(!sensores_validos[i]){
            flag_aomenos1naoexistente = flag_aomenos1naoexistente+1;
        }
    }

    if(flag_aomenos1valido){
        sprintf(resposta_remove, "sensor ");
    }
    //Remove sensores validos
    for(int i=0; i<4; i++){
        if(sensores_validos[i] == -1) break;
        if(sensores_validos[i]){
            strcat(resposta_remove, removerSensor(&matriz, equipamento_id, sensores_solicitados[i]));
            strcat(resposta_remove, " ");
        }
    }
    if(flag_aomenos1valido){
        strcat(resposta_remove, "removed");
    }
    if(flag_aomenos1naoexistente){
        strcat(resposta_remove, " ");
    }
    int qnt = 0;
    // Informa sensores já existentes
    for(int i=0; i<4; i++){
        if(sensores_validos[i] == -1) break;
        if(!sensores_validos[i]){
            static char frase[50];
            sprintf(frase, "sensor 0%d does not exists in 0%d", sensores_solicitados[i], equipamento_id);
            strcat(resposta_remove, frase);
            qnt++;
            if(qnt != flag_aomenos1naoexistente){
                strcat(resposta_remove, " ");
            }
        }
    }
    return resposta_remove;

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

char* comandoListSensors(float(*matriz)[4][4],char *comando){
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

 char* consultarVariavel(float(**matriz)[4][4], int equipamentoID, int sensorID){
    static char resposta[50];
    if((**matriz)[equipamentoID-1][sensorID-1] != -1){
        sprintf(resposta, "%0.2f", (**matriz)[equipamentoID-1][sensorID-1]);
    }else{
        sprintf(resposta, "0%d not installed", sensorID);
    }
    return resposta;
}

char* comandoRead(float(*matriz)[4][4], char *comando){
    static char resposta_read[100];
    memset(resposta_read, 0, 100);

    char substring[] = {"read "};
    sprintf(comando, "%s",removeSubstringDeString(comando, substring));

    int aux = 1;
    char *palavra_atual = strtok(comando, " ");
    int sensores_solicitados[] = {-1, -1, -1, -1};
    int sensores_validos[] = {-1, -1, -1, -1};

    while(strcmp(palavra_atual,"in")){
        int sensor_id = atoi(palavra_atual);

        if(sensor_id < 1 || sensor_id > 4){
            return "invalid sensor";
        }

        sensores_solicitados[aux-1] = sensor_id;
        palavra_atual = strtok(NULL, " ");
        aux++;
    }

    palavra_atual = strtok(NULL, " "); //depois do in
    int equipamento_id = atoi(palavra_atual);
    if(equipamento_id < 1 || equipamento_id > 4){
        return "invalid equipament";
    }

    int flag_aomenos1valido = 0;
    int flag_aomenos1naoexistente = 0;

    // Vê quais sensores não estão instalados ainda
    for(int i=0; i<4; i++){
        if(sensores_solicitados[i] == -1){
            break;
        }
        sensores_validos[i] = boolSensorJaInstalado(&matriz, sensores_solicitados[i], equipamento_id);

        if(!sensores_validos[i]){
            flag_aomenos1naoexistente = flag_aomenos1naoexistente+1;
        }else{
            flag_aomenos1valido = 1;
        }
    }

    int flag_espaco = 0;
    //Lê sensores válidos
    for(int i=0; i<4; i++){
        if(sensores_validos[i] == -1) break;
        if(sensores_validos[i]){
            if(flag_espaco){
                strcat(resposta_read, " ");
            }
            strcat(resposta_read, consultarVariavel(&matriz, equipamento_id, sensores_solicitados[i]));
            flag_espaco = 1;
        }
    }

    if(flag_aomenos1valido && flag_aomenos1naoexistente){
        strcat(resposta_read, " and ");
    }

    //int qnt = 0;
    // Informa sensores não instalados
    for(int i=0; i<4; i++){
        if(sensores_validos[i] == -1) break;
        if(!sensores_validos[i]){
            static char frase[50];
            sprintf(frase, "0%d", sensores_solicitados[i]);
            strcat(resposta_read, frase);
            strcat(resposta_read, " ");
        }
    }
    if(flag_aomenos1naoexistente){
        strcat(resposta_read, "not installed");
    }

    return resposta_read;
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
                sprintf(mensagem_enviada, "%s", comandoRemoveSensor(&matriz, mensagem_recebida));
                break;
            case LIST:
                sprintf(mensagem_enviada, "%s", comandoListSensors(&matriz, mensagem_recebida));
                break;
            case READ:
                sprintf(mensagem_enviada, "%s", comandoRead(&matriz, mensagem_recebida));
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