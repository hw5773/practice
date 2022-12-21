#include "sip_controller.h"

uint8_t *serialize_value(avp_t *avp, int *vlen);

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
  uint8_t key[SC_KEY_LENGTH];
  uint8_t value[SC_VALUE_LENGTH];
  uint8_t *c, *k, *v;
  uint8_t *p[2];
  uint8_t flag, kset;
  int klen, vlen;

  ret = (sip_msg_t *)calloc(1, sizeof(sip_msg_t));
  c = (uint8_t *)buf;
  p[SC_KEY_IDX] = key;
  p[SC_VALUE_IDX] = value;

  memcpy(key, "header", 6);
  p[SC_KEY_IDX] += 6;
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
      klen = p[SC_KEY_IDX] - key;
      vlen = p[SC_VALUE_IDX] - value;

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

      if (klen > 0 && vlen > 0)
      {
        avp = init_avp(k, klen, v, vlen);
        add_avp_to_sip_msg(ret, avp, NULL, 0, 0);
      }

      p[SC_KEY_IDX] = key;
      p[SC_VALUE_IDX] = value;
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

  uint8_t *ret, *p, *vtmp;
  uint8_t tmp[SC_BUF_LENGTH];
  int vlen;
  avp_t *curr;

  p = tmp;
  curr = msg->head;

  while (curr)
  {
    if (strncmp(curr->key, "header", curr->klen))
    {
      memcpy(p, curr->key, curr->klen);
      p += curr->klen;

      memcpy(p, ": ", 2);
      p += 2;
    }
    vtmp = serialize_value(curr, &vlen);
    memcpy(p, vtmp, vlen);
    p += vlen;
    *(p++) = '\n';
    curr = curr->next;
  }
  
  *(p++) = '\n';
  *len = p - tmp;
  ret = (uint8_t *)malloc(*len);
  memcpy(ret, tmp, *len);

  return ret;
}

void print_sip_msg(sip_msg_t *msg)
{
  assert(msg != NULL);

  avp_t *curr;
  uint8_t *value;
  int vlen;
  curr = msg->head;

  printf(">>> # of avps: %d\n", msg->num);
  while (curr)
  {
    value = serialize_value(curr, &vlen);
    printf("key (%d bytes): %.*s, value (%d bytes): %.*s\n", curr->klen, curr->klen, curr->key, vlen, vlen, value);
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

val_t *init_val(uint8_t *val, int len)
{
  val_t *ret;
  int tlen;
  uint8_t *p, *t;
  uint8_t tmp[SC_BUF_LENGTH] = {0, };
  ret = (val_t *)calloc(1, sizeof(val_t));

  p = val;
  t = tmp;
  while (p - val <= len)
  {
    if (*p == '=')
    {
      tlen = t-tmp;
      ret->attr = (uint8_t *)calloc(1, tlen);
      memcpy(ret->attr, tmp, tlen);
      ret->alen = tlen;
      t = tmp;
    }
    else if (p - val == len)
    {
      tlen = t-tmp;
      ret->val = (uint8_t *)calloc(1, tlen);
      memcpy(ret->val, tmp, tlen);
      ret->vlen = tlen;
    }
    else
    {
      *(t++) = *p;
    }

    p++;
  }

  return ret;
}

void free_val(val_t *val)
{
  if (val)
  {
    if (val->attr)
      free(val->attr);
    val->alen = 0;

    if (val->val)
      free(val->val);
    val->vlen = 0;
  }
}

uint8_t *serialize_value(avp_t *avp, int *vlen)
{
  assert(avp != NULL);
  assert(vlen != NULL);

  uint8_t *ret, *p;
  uint8_t tmp[SC_BUF_LENGTH] = {0, };
  vlst_t *vlst;
  val_t *val;

  vlst = avp->vlst;
  p = tmp;

  val = vlst->head;
  while (val)
  {
    if (val != vlst->head)
      *(p++) = ';';

    if (val->attr)
    {
      memcpy(p, val->attr, val->alen);
      p += val->alen;
      *(p++) = '=';
    }
    memcpy(p, val->val, val->vlen);
    p += val->vlen;

    val = val->next;
  }
  *vlen = p-tmp;
  ret = (uint8_t *)malloc(*vlen);
  memcpy(ret, tmp, *vlen);

  return ret;
}

void add_val_to_vlst(vlst_t *vlst, val_t *val)
{
  assert(vlst != NULL);
  assert(val != NULL);

  val_t *curr;
  curr = vlst->head;

  if (!curr)
  {
    vlst->head = val;
  }
  else
  {
    while (curr->next)
    {
      curr = curr->next;
    }
    curr->next = val;
  }
}

vlst_t *init_vlst(uint8_t *value, int vlen)
{
  vlst_t *ret;
  val_t *val;
  int tlen;
  uint8_t *p, *q;
  uint8_t tmp[SC_BUF_LENGTH] = {0, };
  
  ret = (vlst_t *)calloc(1, sizeof(vlst_t));

  p = value;
  q = tmp;
  while (p - value <= vlen)
  {
    if (*p == ';' || *p == '\n' || p - value == vlen)
    {
      tlen = q - tmp;
      val = init_val(tmp, tlen);
      add_val_to_vlst(ret, val);
      q = tmp;
      memset(tmp, 0, SC_BUF_LENGTH);
    }
    else
    {
      *(q++) = *p;
    }
    p++;
  }

  return ret;
}

void free_vlst(vlst_t *vlst)
{
  val_t *curr, *next;

  if (vlst)
  {
    curr = vlst->head;

    while (curr)
    {
      next = curr->next;
      free_val(curr);
      curr = next;
    }
    free(vlst);
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

  ret->vlst = init_vlst(value, vlen);

  return ret;
}

void free_avp(avp_t *avp)
{
  if (avp)
  {
    if (avp->key)
      free(avp->key);
    avp->klen = 0;
    
    if (avp->vlst)
      free_vlst(avp->vlst);
  }
}

int get_num_of_avps_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen)
{
  assert(msg != NULL);
  assert(key != NULL);
  assert(klen > 0);

  int ret;
  avp_t *curr;
  ret = 0;
  curr = msg->head;

  while (curr)
  {
    if (curr->klen == klen
        && !strncmp((const char *)(curr->key), (const char *)key, curr->klen))
      ret++;
    curr = curr->next;
  }

  return ret;
}

avp_t *get_avp_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen, int idx)
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
    {
      idx--;
      if (idx < 0)
      {
        ret = curr;
        break;
      }
    }
    curr = curr->next;
  }

  return ret;
}

void del_avp_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen, int idx)
{
  assert(msg != NULL);
  assert(key != NULL);
  assert(klen > 0);

  avp_t *res, *prev, *curr;

  res = NULL;
  prev = NULL;
  curr = msg->head;

  while (curr)
  {
    if (curr->klen == klen
        && !strncmp((const char *)(curr->key), (const char *)key, curr->klen))
    {
      idx--;

      if (idx < 0)
      {
        res = curr;
        if (!prev)
          msg->head = curr->next;
        else
          prev->next = curr->next;

        if (msg->tail == curr)
          msg->tail = prev;
        break;
      }
    }
    prev = curr;
    curr = curr->next;
  }

  if (res)
  {
    free_avp(res);
    msg->num--;
  }
}

int add_avp_to_sip_msg(sip_msg_t *msg, avp_t *avp, uint8_t *key, int klen, int idx)
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
    prev = get_avp_from_sip_msg(msg, key, klen, idx);

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

  ret = SC_SUCCESS;
  msg->num++;
  return ret;
}

int is_attribute_included(avp_t *avp, uint8_t *attr, int alen)
{
  assert(avp != NULL);
  assert(attr != NULL);
  assert(alen > 0);

  int ret;
  vlst_t *vlst;
  val_t *val;

  ret = SC_FALSE;
  vlst = avp->vlst;
  val = vlst->head;

  while (val)
  {
    if (val->alen == alen 
        && !strncmp(val->attr, attr, alen))
    {
      ret = SC_TRUE;
      break;
    }
    val = val->next;
  }

  return ret;
}

uint8_t *get_value_from_avp(avp_t *avp, uint8_t *attr, int alen, int *vlen)
{
  assert(avp != NULL);
  assert(attr != NULL);
  assert(alen > 0);
  assert(vlen != NULL);

  uint8_t *ret;
  vlst_t *vlst;
  val_t *val;

  ret = NULL;
  vlst = avp->vlst;
  val = vlst->head;

  while (val)
  {
    if (val->alen == alen 
        && !strncmp(val->attr, attr, alen))
    {
      ret = (uint8_t *)malloc(val->vlen);
      memcpy(ret, val->val, val->vlen);
      *vlen = val->vlen;
      break;
    }
    val = val->next;
  }

  return ret;
}

void change_value_from_avp(avp_t *avp, uint8_t *attr, int alen, uint8_t *value, int vlen)
{
  assert(avp != NULL);
  assert(attr != NULL);
  assert(alen > 0);
  assert(value != NULL);
  assert(vlen > 0);

  vlst_t *vlst;
  val_t *val;

  vlst = avp->vlst;
  val = vlst->head;

  while (val)
  {
    if (val->alen == alen 
        && !strncmp(val->attr, attr, alen))
    {
      if (val->val)
        free(val->val);
      val->val = (uint8_t *)malloc(vlen);
      memcpy(val->val, value, vlen);
      val->vlen = vlen;
    }
    val = val->next;
  }
}
