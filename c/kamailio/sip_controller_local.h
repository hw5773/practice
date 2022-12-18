#ifndef __SIP_CONTROLLER_LOCAL_H__
#define __SIP_CONTROLLER_LOCAL_H__

#include "sip_controller.h"

struct avp_st
{
  uint8_t *key;
  uint8_t *value;
  struct avp_st *next;
};

struct sip_msg_st
{
  int num;
  struct avp_st *head;
};

#endif /* __SIP_CONTROLLER_LOCAL_H__ */
