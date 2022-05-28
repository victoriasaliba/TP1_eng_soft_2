#include "common.h"

//  Verifica se o protocolo é IPv4 ou IPv6. Caso seja nenhum dos dois, retorna -1
int defineProtocolo(const char *stringEndereco, const char *stringPorta, struct sockaddr_storage *storage){
    if(stringEndereco == NULL || stringPorta == NULL) {
        return -1;
    }

    uint16_t porta = (uint16_t)atoi(stringPorta); // Converte a porta de string para int (16 bits sem sinal)
    if(porta == 0) {
        return -1;
    }
    porta = htons(porta); //Converte o inteiro da representação de dispositivo para a representação da rede

    //  Testa o IPv4
    struct in_addr enderecoIPv4;
    if (inet_pton(AF_INET, stringEndereco, &enderecoIPv4)) {
        struct sockaddr_in *endereco4 = (struct sockaddr_in *)storage;
        endereco4->sin_family = AF_INET;
        endereco4->sin_port = porta;
        endereco4->sin_addr = enderecoIPv4;
        return 0;
    }

    //Se der erro, tenta o IPv6
    struct in6_addr enderecoIPv6;
    if (inet_pton(AF_INET6, stringEndereco, &enderecoIPv6)) {
        struct sockaddr_in6 *endereco6 = (struct sockaddr_in6 *)storage;
        endereco6->sin6_family = AF_INET6;
        endereco6->sin6_port = porta;
        memcpy(&(endereco6->sin6_addr), &enderecoIPv6, sizeof(enderecoIPv6));
        return 0;
    }

    // Se não der nenhum dos dois, retorna -1
    return -1;
}

//  Define o protocolo do servidor
int defineProtocoloServidor(const char *protocolo, const char *stringPorta, struct sockaddr_storage *storage){
        uint16_t porta = (uint16_t)atoi(stringPorta);
        if(porta == 0){
            return -1;
    }
    porta = htons(porta);

    memset(storage, 0, sizeof(*storage));
    if(strcmp(protocolo, "v4") == 0){
        struct sockaddr_in *endereco4 = (struct sockaddr_in *)storage;
        endereco4->sin_family = AF_INET;
        endereco4->sin_addr.s_addr = INADDR_ANY;
        endereco4->sin_port = porta;
        return 0;
    }else if(strcmp(protocolo, "v6") == 0){
        struct sockaddr_in6 *endereco6 = (struct sockaddr_in6 *)storage;
        endereco6->sin6_family = AF_INET6;
        endereco6->sin6_addr = in6addr_any;
        endereco6->sin6_port = porta;
        return 0;
    }else{
        return -1;
    }
}
