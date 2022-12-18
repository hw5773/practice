#include "debug.h"
#include "sip_controller_local.h"

int read_msg(char *input, char *buf, int max)
{
  int len;
  FILE *fp;

  fp = fopen(input, "r");
  fseek(fp, 0L, SEEK_END);
  len = (int)ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  fread(buf, 1, len, fp);

  return len;
}



int get_value_by_key(sip_msg_t *msg, char *key, int klen, char *val, int max)
{
  int vlen;



  return vlen;
}

int replace_value_by_key(sip_msg_t *msg, char *key, int klen, char *value, int vlen)
{
  int len;

  return len;
}
