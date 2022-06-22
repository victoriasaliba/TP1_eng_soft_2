#include "common.h"
#include "inc_server.h"
#include "sput-1.4.0/sput.h"

int setup(char *func, char *argv, char* argc) {
  struct sockaddr_storage storage;

  if(0 == strcmp("defineProtocolo", func)) {
    return defineProtocolo(argv, argc, &storage);
  }
  else if(0 == strcmp("defineProtocoloServidor", func)) {
    return defineProtocoloServidor(argv, argc, &storage);
  }
  else {
    return EXIT_FAILURE;
  }
}

/**
 * Client tests
 */
static void test_client_address_ipv4()
{
  int test = setup("defineProtocolo", "127.0.0.1", "5151");
  sput_fail_unless(test == 0, "Endereço IPv4 válido");
}

static void test_client_address_ipv4_invalido()
{
  int test = setup("defineProtocolo", "127.0.0.1.152", "5151");
  sput_fail_unless(test == -1, "Endereço IPv4 inválido : Mais de 3 pontos");
  test = setup("defineProtocolo", "127.256.0.1", "5151");
  sput_fail_unless(test == -1, "Endereço IPv4 inválido : Possui valor acima de 255");
}

static void test_client_address_ipv6()
{
  int test = setup("defineProtocolo", "59FB::1005:CC57:6571", "5151");
  sput_fail_unless(test == 0, "Endereço IPv6 válido");
}

static void test_client_address_ipv6_invalido()
{
  int test = setup("defineProtocolo", "56FE::2159:5BBC::6594", "5151");
  sput_fail_unless(test == -1, "Endereço IPv6 inválido");
}

static void test_client_porta_invalida()
{
  int test = setup("defineProtocolo", "127.0.0.1", "0");
  sput_fail_unless(test == -1, "Porta invalida com Endereço IPv4 válido");
  test = setup("defineProtocolo", "::1", "0");
  sput_fail_unless(test == -1, "Porta invalida com Endereço IPv6 válido");
}

/**
 * Server tests
 */
static void test_server_protocolo_ipv4()
{
  int test = setup("defineProtocoloServidor", "v4", "5151");
  sput_fail_unless(test == 0, "Protocolo IPv4");
}

static void test_server_protocolo_ipv6()
{
  int test = setup("defineProtocoloServidor", "v6", "5151");
  sput_fail_unless(test == 0, "Protocolo IPv6");
}

static void test_server_protocolo_invalido()
{
  int test = setup("defineProtocoloServidor", "v5", "5151");
  sput_fail_unless(test == -1, "Protocolo Invalido : Sistema só suporta IPv4 e IPv6");
}

static void test_server_porta_invalida()
{
  int test1 = setup("defineProtocoloServidor", "v4", "0") == -1;
  int test2 = setup("defineProtocoloServidor", "v6", "0") == -1;
  sput_fail_unless((test1 && test2), "Porta invalida com protocolo válido");
}

static void test_server_retornaAcao_invalido()
{
  int test1 = retornaAcao("add sensors 01 02 03 in 01") == INVALIDO;
  int test2 = retornaAcao("remove sensors 01 02 03") == INVALIDO;
  int test3 = retornaAcao("list sensor in 01") == INVALIDO;
  int test4 = retornaAcao("reads sensors in 02") == INVALIDO;
  int test5 = retornaAcao("comando aleatorio") == INVALIDO;
  
  sput_fail_unless((test1 && test2 && test3 && test4 && test5), "Comandos invalidos");
}

static void test_server_retornaAcao_add()
{
  int test1 = retornaAcao("add sensor 01 02 03 in 01") == ADD;
  sput_fail_unless((test1), "Comando ADD");
}


static void test_server_retornaAcao_remove()
{
  int test1 = retornaAcao("remove sensor 01 02 in 01") == REMOVE;
  sput_fail_unless((test1), "Comando REMOVE");
}


static void test_server_retornaAcao_list()
{
  int test1 = retornaAcao("list sensors in 01") == LIST;
  sput_fail_unless((test1), "Comando LIST");
}

static void test_server_retornaAcao_read()
{
  int test1 = retornaAcao("read 01 in 01") == READ;
  sput_fail_unless((test1), "Comando READ");
}

static void test_server_retornaAcao_kill()
{
  int test1 = retornaAcao("remove sensor 01 02 in 01 kill") == KILL;
  int test2 = retornaAcao("kill remove sensor 01 02 in 01") == KILL;
  int test3 = retornaAcao("remove killsensor 01 02 in 01") == KILL;
  int test4 = retornaAcao("remove senskillor 01 02 in 01") == KILL;
  int test5 = retornaAcao("remove sensor 01 02 kill in 01") == KILL;
  int test6 = retornaAcao("kill") == KILL;
  sput_fail_unless((test1 && test2 && test3 && test4 && test5 && test6), "Comando KILL");
}

int main(int argc, char *argv[])
{
  sput_start_testing();

  sput_enter_suite("Client -> defineProtocolo()");
  sput_run_test(test_client_address_ipv4);
  sput_run_test(test_client_address_ipv4_invalido);
  sput_run_test(test_client_address_ipv6);
  sput_run_test(test_client_address_ipv6_invalido);
  sput_run_test(test_client_porta_invalida);

  sput_enter_suite("Server -> defineProtocoloServidor() :");
  sput_run_test(test_server_protocolo_ipv4);
  sput_run_test(test_server_protocolo_ipv6);
  sput_run_test(test_server_protocolo_invalido);
  sput_run_test(test_server_porta_invalida);

  sput_enter_suite("Server -> retornaAcao() :");
  sput_run_test(test_server_retornaAcao_invalido);
  sput_run_test(test_server_retornaAcao_add);
  sput_run_test(test_server_retornaAcao_remove);
  sput_run_test(test_server_retornaAcao_list);
  sput_run_test(test_server_retornaAcao_read);
  sput_run_test(test_server_retornaAcao_kill);

  sput_finish_testing();

  return sput_get_return_value();
}