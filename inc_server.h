#pragma once

#define MAX_SENSORS 15
#define INVALIDO 0
#define ADD 1
#define REMOVE 2
#define LIST 3
#define READ 4
#define KILL 5

struct EstruturaDeControle {
  char resposta[100];
  char *palavra_atual;

  int aux;
  int sensores_solicitados[4];
  int sensores_validos[4];
  int flag_aomenos1valido;
  int flag_aomenos1jaexistente;
  int flag_aomenos1naoexistente;
};

void iniciarEstrutura(struct EstruturaDeControle *c, char *comando);
void checarSensoresValidos(struct EstruturaDeControle *c, float(*matriz)[4][4], int equipamento_id, int boolInstalarOuRemover);

void instalarSensoresValidos(struct EstruturaDeControle *c, float(*matriz)[4][4], int equipamento_id);
void removerSensoresValidos(struct EstruturaDeControle *c, float(*matriz)[4][4], int equipamento_id);
void lerSensoresValidos(struct EstruturaDeControle *c, float(*matriz)[4][4], int equipamento_id);
void informarSensoresExistentes(struct EstruturaDeControle *c, int equipamento_id);
void informarSensoresNaoExistentes(struct EstruturaDeControle *c, int equipamento_id);

char* instalarSensor(float(**matriz)[4][4], int equipamentoID, int sensorID);
char* removerSensor(float(**matriz)[4][4], int equipamentoID, int sensorID);
char* consultarVariavel(float(**matriz)[4][4], int equipamentoID, int sensorID);

int boolSensorJaInstalado(float(**matriz)[4][4], int sensor, int equipamento);
int elementoInvalido(int element_id);
