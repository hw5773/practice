#include "debug.h"
#include "sip_controller_local.h"

int read_msg(char *input, char *buf, int max)
{
  assert(input != NULL);
  assert(buf != NULL);
  assert(max > 0);

  int len;
  FILE *fp;

  fp = fopen(input, "r");
  fseek(fp, 0L, SEEK_END);
  len = (int)ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  fread(buf, 1, len, fp);

  return len;
}

sip_msg_t *parse_sip_msg(char *buf, int len)
{
  assert(buf != NULL);
  assert(len > 0);

  sip_msg_t *ret;
  avp_t *avp;
  uint8_t key[KEY_LENGTH];
  uint8_t value[VALUE_LENGTH];
  uint8_t *c, *k, *v;
  uint8_t *p[2];
  uint8_t flag, kset;
  int klen, vlen;

  ret = (sip_msg_t *)calloc(1, sizeof(sip_msg_t));
  c = (uint8_t *)buf;
  p[KEY_IDX] = key;
  p[VALUE_IDX] = value;

  memcpy(key, "header", 6);
  p[KEY_IDX] += 6;
  flag = 1;
  kset = 0;

  while (c - (uint8_t *)buf < len)
  {
    if (*c == ':' && !kset)
    {
      flag = (flag + 1) % 2;
      kset = 1;
    }
    else if (*c == '\n')
    {
      flag = (flag + 1) % 2;
      klen = p[KEY_IDX] - key;
      vlen = p[VALUE_IDX] - value;

      k = key;
      while (*k == ' ')
      {
        k++;
        klen--;
      }

      while (*(k + klen - 1) == ' ')
      {
        klen--;
      }

      v = value;
      while (*v == ' ')
      {
        v++;
        vlen--;
      }

      while (*(v + vlen - 1) == ' ')
      {
        vlen--;
      }

      dmsg(MSG_OUTPUT_PARSER, "key (%d bytes): %.*s, value (%d bytes): %.*s", klen, klen, k, vlen, vlen, v);
      if (klen > 0 && vlen > 0)
      {
        avp = init_avp(k, klen, v, vlen);
        add_avp_to_sip_msg(ret, avp, NULL, 0);
      }

      p[KEY_IDX] = key;
      p[VALUE_IDX] = value;
      kset = 0;
    }
    else
    {
      *(p[flag]++) = *c;
    }
    c++;
  }

  return ret;
}

uint8_t *serialize_sip_msg(sip_msg_t *msg, int *len)
{
  assert(msg != NULL);
  assert(len != NULL);

  uint8_t *ret;
  return ret;
}

int replace_value_by_key(sip_msg_t *msg, char *key, int klen, char *value, int vlen)
{
  int ret;
  avp_t *avp;

  avp = get_avp_from_sip_msg(msg, key, klen);
  if (avp)
  {
    if (avp->value)
      free(avp->value);
    avp->vlen = vlen;
    avp->value = (uint8_t *)calloc(1, vlen);
    memcpy(avp->value, value, vlen);
  }
  else
    ret = FAILURE;

  return ret;
}

void print_sip_msg(sip_msg_t *msg)
{
  assert(msg != NULL);

  avp_t *curr;
  curr = msg->head;

  imsg(MSG_OUTPUT_PARSER, "# of avps: %d", msg->num);
  while (curr)
  {
    imsg(MSG_OUTPUT_PARSER, "key (%d bytes): %.*s, value (%d bytes): %.*s", curr->klen, curr->klen, curr->key, 
        curr->vlen, curr->vlen, curr->value);
    curr = curr->next;
  }
}

sip_msg_t *init_sip_msg(void)
{
  sip_msg_t *ret;
  ret = (sip_msg_t *)calloc(1, sizeof(sip_msg_t));

  return ret;
}

void free_sip_msg(sip_msg_t *msg)
{
  avp_t *curr, *next;

  if (msg)
  {
    curr = msg->head;
    while (curr)
    {
      next = curr->next;
      free_avp(curr);
      curr = next;
    }
  }
}

avp_t *init_avp(uint8_t *key, int klen, uint8_t *value, int vlen)
{
  assert(key != NULL);
  assert(klen > 0);
  assert(value != NULL);
  assert(vlen > 0);

  avp_t *ret;
  ret = (avp_t *)calloc(1, sizeof(avp_t));

  ret->key = (uint8_t *)calloc(1, klen);
  memcpy(ret->key, key, klen);
  ret->klen = klen;

  ret->value = (uint8_t *)calloc(1, vlen);
  memcpy(ret->value, value, vlen);
  ret->vlen = vlen;

  return ret;
}

void free_avp(avp_t *avp)
{
  if (avp)
  {
    if (avp->key)
      free(avp->key);
    avp->klen = 0;
    
    if (avp->value)
      free(avp->value);
    avp->value = 0;
  }
}

avp_t *get_avp_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen)
{
  assert(msg != NULL);
  assert(key != NULL);
  assert(klen > 0);

  avp_t *ret, *curr;

  ret = NULL;
  curr = msg->head;

  while (curr)
  {
    if (curr->klen == klen
        && !strncmp((const char *)(curr->key), (const char *)key, curr->klen))
      ret = curr;
  }

  return ret;
}

avp_t *del_avp_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen)
{
  assert(msg != NULL);
  assert(key != NULL);
  assert(klen > 0);

  avp_t *ret, *prev, *curr;

  ret = NULL;
  prev = NULL;
  curr = msg->head;

  while (curr)
  {
    if (curr->klen == klen
        && !strncmp((const char *)(curr->key), (const char *)key, curr->klen))
    {
      ret = curr;
      if (!prev)
        msg->head = curr->next;
      else
        prev->next = curr->next;

      if (msg->tail == curr)
        msg->tail = prev;
    }
  }

  if (!ret)
    msg->num--;

  return ret;
}

int add_avp_to_sip_msg(sip_msg_t *msg, avp_t *avp, uint8_t *key, int klen)
{
  assert(msg != NULL);
  assert(avp != NULL);

  avp_t *prev;
  int ret;

  ret = 0;
  if (!key)
    prev = msg->tail;
  else
  {
    prev = get_avp_from_sip_msg(msg, key, klen);

    if (!prev)
      prev = msg->tail;
  }

  if (prev)
  {
    avp->next = prev->next;
    prev->next = avp;
  }
  else
  {
    if (!msg->head)
      msg->head = avp;
  }

  if (!prev || (prev == msg->tail))
    msg->tail = avp;

  ret = SUCCESS;
  msg->num++;
  return ret;
}
