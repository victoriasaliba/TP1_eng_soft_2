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

  sput_finish_testing();

  return sput_get_return_value();
}