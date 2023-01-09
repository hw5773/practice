#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "base64.h"
#include "debug.h"

int dtype;

int usage(const char *pname)
{
  emsg(">> Usage: %s [options]", pname);
  emsg("Options");
  emsg("  -d, --decode    Number to be decoded");
  exit(1);
}

int main(int argc, char *argv[])
{
  uint8_t *in;
  uint8_t out[16384] = {0, };
  int c, ilen, olen;
  const char *pname;
  
  dtype = DPI_DEBUG_CIRCUIT;
  in = NULL;

  while (1)
  {
    int option_index = 0;
    static struct option long_options[] = {
      {"decode", required_argument, 0, 'a'},
      {0, 0, 0, 0}
    };

    const char *opt = "d:0";

    c = getopt_long(argc, argv, opt, long_options, &option_index);

    if (c == -1)
      break;
    
    switch (c)
    {
      case 'd':
        ilen = strlen(optarg);
        in = (uint8_t *)malloc(ilen);
        memcpy(in, optarg, ilen);
        break;

      default:
        usage(pname);
    }
  }

  if (!in)
  {
    emsg("Please insert the input base64 encoded string");
    exit(1);
  }

  imsg(DPI_DEBUG_CIRCUIT, "input (%d bytes): %.*s", ilen, ilen, in);
  iprint(DPI_DEBUG_CIRCUIT, "input", in, 0, (int)(strlen(in)), 16);
  base64_url_decode(in, ilen, out, &olen);
  imsg(DPI_DEBUG_CIRCUIT, "decoded (%d bytes): %.*s", olen, olen, out);
  iprint(DPI_DEBUG_CIRCUIT, "decoded", out, 0, olen, 16);

  return 0;
}
