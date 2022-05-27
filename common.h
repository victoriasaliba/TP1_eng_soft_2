#pragma once
#include <stdlib.h>
#include <arpa/inet.h>

int defineProtocolo(const char *stringEndereco, const char *stringPorta, struct sockaddr_storage *storage);
int defineProtocoloServidor(const char *protocolo, const char *stringPorta, struct sockaddr_storage *storage);