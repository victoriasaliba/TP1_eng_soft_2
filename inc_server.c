#include "common.h"
#include "inc_server.h"

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
}

int boolSensorJaInstalado(float(**matriz)[4][4], int sensor, int equipamento) {
  if((**matriz)[equipamento-1][sensor-1] != -1){
    return 1;
  }else{
    return 0;
  }
}

int elementoInvalido(int elemento_id) {
  return elemento_id < 1 || elemento_id > 4 ? 1 : 0;
}
