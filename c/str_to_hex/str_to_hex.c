#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define BUF_SIZE 1024

int usage(const char *pname)
{
  printf(">> Usage: %s [options]\n", pname);
  printf("Options\n");
  printf("  -i, --input       Input string\n");
  exit(1);
}

int char_to_hex(uint8_t c)
{
  int ret;

  switch (c)
  {
    case '0':
      ret = 0x0;
      break;

    case '1':
      ret = 0x1;
      break;

    case '2':
      ret = 0x2;
      break;

    case '3':
      ret = 0x3;
      break;

    case '4':
      ret = 0x4;
      break;

    case '5':
      ret = 0x5;
      break;

    case '6':
      ret = 0x6;
      break;

    case '7':
      ret = 0x7;
      break;

    case '8':
      ret = 0x8;
      break;

    case '9':
      ret = 0x9;
      break;

    case 'a':
      ret = 0xa;
      break;

    case 'b':
      ret = 0xb;
      break;

    case 'c':
      ret = 0xc;
      break;

    case 'd':
      ret = 0xd;
      break;

    case 'e':
      ret = 0xe;
      break;

    case 'f':
      ret = 0xf;
      break;

    case 'A':
      ret = 0xa;
      break;

    case 'B':
      ret = 0xb;
      break;

    case 'C':
      ret = 0xc;
      break;

    case 'D':
      ret = 0xd;
      break;

    case 'E':
      ret = 0xe;
      break;

    case 'F':
      ret = 0xf;
      break;

    default:
      ret = 0x0;
      break;
  }

  return ret;
}

int str_to_hex(uint8_t *str)
{
  int ret;
  int i, len;
  len = strlen(str);
  assert(len >= 2);

  ret = 0;
  for (i=0; i<2; i++)
  {
    ret *= 16;
    ret += char_to_hex(str[i]);
  }

  return ret;
}

uint8_t *make_array(uint8_t *str, int *len)
{
  int i, slen;
  uint8_t *ret, *p;
  *len = strlen(str)/2;
  ret = (uint8_t *)malloc(*len);

  p = str;
  for (i=0; i<*len; i++)
  {
    ret[i] = str_to_hex(p);
    printf("i: %d, p: %.*s, hex: %02x\n", i, 2, p, ret[i]);
    p += 2;
  }

  return ret;
}

int main(int argc, char *argv[])
{
  uint8_t *pname, *input, *arr;
  int c, i, ilen;

  input = NULL;
  pname = argv[0];

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
        input = make_array(optarg, &ilen);
        break;

      default:
        usage(pname);
    }
  }

  if (!input)
  {
    printf("The input string should be inserted\n");
    goto out;
  }

  printf("input: \n");
  for (i=0; i<ilen; i++)
  {
    printf("%02x ", input[i]);
  }
  printf("\n");

out:
  return 0;
}
