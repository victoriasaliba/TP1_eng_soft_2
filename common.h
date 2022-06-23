#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <inttypes.h>

#define BUFSZ 500

int defineProtocolo(const char *stringEndereco, const char *stringPorta, struct sockaddr_storage *storage);
int defineProtocoloServidor(const char *protocolo, const char *stringPorta, struct sockaddr_storage *storage);