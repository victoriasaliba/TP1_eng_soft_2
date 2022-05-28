#include "common.h"
#include "inc_server.h"

extern int quantidadeAtual;

void iniciarEstrutura(struct EstruturaDeControle *c, char *comando) {
  c->palavra_atual = strtok(comando, " ");
  memset(c->resposta, 0, 100);

  c->aux = 1;
  c->flag_aomenos1valido = 0;
  c->flag_aomenos1jaexistente = 0;

  for (int i = 0; i < 4; i++) {
    c->sensores_solicitados[i] = -1;
    c->sensores_validos[i] = -1;
  }
}

void checarSensoresValidos(struct EstruturaDeControle *c, float(*matriz)[4][4], int equipamento_id) {
  for(int i=0; i<4; i++){
    if(c->sensores_solicitados[i] == -1){
      break;
    }

    c->sensores_validos[i] = !boolSensorJaInstalado(&matriz, c->sensores_solicitados[i], equipamento_id);

    if(c->sensores_validos[i]){
      c->flag_aomenos1valido = 1;
    }

    if(!c->sensores_validos[i]){
      c->flag_aomenos1jaexistente = c->flag_aomenos1jaexistente+1;
    }
  }

  if(c->flag_aomenos1valido){
    sprintf(c->resposta, "sensor ");
  }
}

int boolSensorJaInstalado(float(**matriz)[4][4], int sensor, int equipamento) {
  if((**matriz)[equipamento-1][sensor-1] != -1){
    return 1;
  }else{
    return 0;
  }
}

void instalarSensoresValidos(struct EstruturaDeControle *c, float(*matriz)[4][4], int equipamento_id) {
  for(int i=0; i<4; i++){
    if(c->sensores_validos[i] == -1) break;
    if(c->sensores_validos[i]){
      strcat(c->resposta, instalarSensor(&matriz, equipamento_id, c->sensores_solicitados[i]));
      strcat(c->resposta, " ");
    }
  }
  if(c->flag_aomenos1valido){
    strcat(c->resposta, "added");
  }
  if(c->flag_aomenos1jaexistente && c->flag_aomenos1valido){
    strcat(c->resposta, " ");
  }
}

char* instalarSensor(float(**matriz)[4][4], int equipamentoID, int sensorID) {
  static char resposta[50];

  float valorAleatorio = (rand() % 1000);
  valorAleatorio = valorAleatorio/100;
  (**matriz)[equipamentoID-1][sensorID-1] = valorAleatorio;
  quantidadeAtual++;
  sprintf(resposta, "0%d", sensorID);
  return resposta;
}

char* removerSensor(float(**matriz)[4][4], int equipamentoID, int sensorID) {
  static char resposta[50];
  if((**matriz)[equipamentoID-1][sensorID-1] != -1){
    (**matriz)[equipamentoID-1][sensorID-1] = -1;
    sprintf(resposta, "0%d", sensorID);
    quantidadeAtual--;
  }

  return resposta;
}

void informarSensoresExistentes(struct EstruturaDeControle *c, int equipamento_id) {
  c->aux = 0;
  for(int i=0; i<4; i++){
    if(c->sensores_validos[i] == -1) break;
    if(!c->sensores_validos[i]){
      static char frase[50];
      sprintf(frase, "sensor 0%d already exists in 0%d", c->sensores_solicitados[i], equipamento_id);
      strcat(c->resposta, frase);
      c->aux++;
      if(c->aux != c->flag_aomenos1jaexistente){
        strcat(c->resposta, " ");
      }
    }
  }
}

int elementoInvalido(int elemento_id) {
  return elemento_id < 1 || elemento_id > 4 ? 1 : 0;
}
