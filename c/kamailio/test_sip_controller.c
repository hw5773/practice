#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>

#include "debug.h"
#include "sip_controller.h"

#define BUF_SIZE 2048

int dtype;

int usage(const char *pname)
{
  emsg(">> Usage: %s [options]", pname);
  emsg("Options");
  emsg("  -i, --input       Input message file");
  exit(1);
}

int main(int argc, char *argv[])
{
  char *pname, *input;
  char buf[BUF_SIZE] = {0, };
  int c, len, rlen;
  sip_msg_t *msg;
  avp_t *avp, *tmp;
  const uint8_t *key = "v";
  const uint8_t *value = "SIP/2.0/TCP [2607:fc20:ba53:1539:e495:7fff:fe80:2e48]:40091;rport=40598;branch=z9hG4bK1014822785";
  uint8_t *res;

  input = NULL;
  pname = argv[0];
  dtype = MSG_OUTPUT_PARSER;

  while (1)
  {
    int option_index = 0;
    static struct option long_options[] = {
      {"input", required_argument, 0, 'i'},
      {0, 0, 0, 0}
    };

    const char *opt = "i:0";

    c = getopt_long(argc, argv, opt, long_options, &option_index);

    if (c == -1)
      break;
    
    switch (c)
    {
      case 'i':
        input = optarg;
        if (access(input, F_OK) != 0)
        {
          emsg("The input file (%s) does not exist", input);
          input = NULL;
        }
        break;

      default:
        usage(pname);
    }
  }

  if (!input)
  {
    emsg("The input file should be inserted");
    goto out;
  }

  len = read_msg(input, buf, BUF_SIZE);
  msg = parse_sip_msg(buf, len);

  imsg(MSG_OUTPUT_PARSER, "Before modification");
  print_sip_msg(msg);
  printf("\n");
  imsg(MSG_OUTPUT_PARSER, "Read message (%d bytes): %s", len, buf);
  printf("\n");

  avp = get_avp_from_sip_msg(msg, "f", 1);
  change_value_from_avp(avp, "changed", 7);
  del_avp_from_sip_msg(msg, "Via", 3);
  tmp = init_avp("Hello", 5, "World!", 6);
  add_avp_to_sip_msg(msg, tmp, "v", 1);

  imsg(MSG_OUTPUT_PARSER, "After modification");
  print_sip_msg(msg);
  printf("\n");

  res = serialize_sip_msg(msg, &rlen);
  free_sip_msg(msg);
  imsg(MSG_OUTPUT_PARSER, "Final message (%d bytes): %s", rlen, res);

out:
  return 0;
}
