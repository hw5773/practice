#ifndef __SIP_CONTROLLER_LOCAL_H__
#define __SIP_CONTROLLER_LOCAL_H__

#include <string.h>
#include <stdlib.h>
#include "sip_controller.h"

#define SUCCESS 1
#define FAILURE -1

#define KEY_LENGTH 1024
#define VALUE_LENGTH 16384
#define BUF_LENGTH 16384

#define KEY_IDX 0
#define VALUE_IDX 1

struct avp_st
{
  uint8_t *key;
  int klen;
  uint8_t *value;
  int vlen;
  struct avp_st *next;
};

struct sip_msg_st
{
  int num;
  struct avp_st *head;
  struct avp_st *tail;
};

#endif /* __SIP_CONTROLLER_LOCAL_H__ */
