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
  int c, i, len, rlen, vlen, vint, num;
  sip_msg_t *msg;
  avp_t *avp, *tmp, *storage[10];
  const uint8_t *str;
  uint8_t *res, *val;
  uint8_t found;

  input = NULL;
  pname = argv[0];
  dtype = MSG_OUTPUT_PARSER;
  found = SC_FALSE;

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

  if (is_401_unauthorized_msg(msg))
  {
    imsg(MSG_OUTPUT_PARSER, "This SIP message is a 401 unauthorized message");
  }
  else
  {
    imsg(MSG_OUTPUT_PARSER, "This SIP message is not a 401 unauthorized message");
  }

  if (is_200_ok_msg(msg))
  {
    imsg(MSG_OUTPUT_PARSER, "This SIP message is a 200 OK message");
  }
  else
  {
    imsg(MSG_OUTPUT_PARSER, "This SIP message is NOT a 200 OK message");
  }

  imsg(MSG_OUTPUT_PARSER, "Before modification");
  print_sip_msg(msg);
  printf("\n");
  imsg(MSG_OUTPUT_PARSER, "Read message (%d bytes): %s", len, buf);
  printf("\n");

  avp = get_avp_from_sip_msg(msg, "f", 1, 0);
  tmp = init_avp("Hello", 5, "World!", 6);
  add_avp_to_sip_msg(msg, tmp, "i", 1, 0);

  num = get_num_of_avps_from_sip_msg(msg, "Via", 3);
  for (i=0; i<num; i++)
  {
    avp = get_avp_from_sip_msg(msg, "Via", 3, i);
    if (is_attribute_included(avp, "rport", 5))
    {
      found = SC_TRUE;
      break;
    }
  }

  if (found)
  {
    val = get_value_from_avp_by_name(avp, "rport", 5, &vlen);
    vint = atoi(val) - 30000;
    free(val);
    val = (uint8_t *)calloc(6, sizeof(uint8_t));
    vlen = snprintf(val, 6, "%d", vint);
    
    change_value_from_avp_by_name(avp, "rport", 5, val, vlen);
    free(val);
  }

  avp = get_avp_from_sip_msg(msg, "CSeq", 4, 0);
  num = get_num_of_values_from_avp(avp);
  imsg(MSG_OUTPUT_PARSER, "  # of values: %d", num);
  for (i=0; i<num; i++)
  {
    val = get_value_from_avp_by_idx(avp, i, &vlen);
    imsg(MSG_OUTPUT_PARSER, "  Value: %.*s", vlen, val);
  }
  str = "3 REGISTER";
  val = (uint8_t *)malloc(strlen(str));
  memcpy(val, str, strlen(str));
  vlen = strlen(val);

  change_value_from_avp_by_idx(avp, 0, val, vlen);
  free(val);

  imsg(MSG_OUTPUT_PARSER, "After modification");
  print_sip_msg(msg);
  printf("\n");

  res = serialize_sip_msg(msg, &rlen);
  free_sip_msg(msg);
  imsg(MSG_OUTPUT_PARSER, "Final message (%d bytes): %s", rlen, res);

out:
  return 0;
}
