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
  int c, len;

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
  imsg(MSG_OUTPUT_PARSER, "Read message (%d bytes): %s", len, buf);

out:
  return 0;
}
