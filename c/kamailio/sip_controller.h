#ifndef __SIP_CONTROLLER_H__
#define __SIP_CONTROLLER_H__

#include <stdio.h>
#include <stdint.h>

typedef struct sip_msg_st sip_msg_t;
typedef struct avp_st avp_t;

int read_msg(char *input, char *buf, int max);
sip_msg_t *parse_sip_msg(char *buf, int len);
void free_sip_msg(sip_msg_t *msg);
uint8_t *serialize_sip_msg(sip_msg_t *msg, int *len);
int replace_value_by_key(sip_msg_t *msg, char *key, int klen, char *value, int vlen);
void print_sip_msg(sip_msg_t *msg);

#endif /* __SIP_CONTROLLER_H__ */
