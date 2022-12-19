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
void print_sip_msg(sip_msg_t *msg);

avp_t *init_avp(uint8_t *key, int klen, uint8_t *value, int vlen);
void free_avp(avp_t *avp);

avp_t *get_avp_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen);
int add_avp_to_sip_msg(sip_msg_t *msg, avp_t *avp, uint8_t *key, int klen);
void del_avp_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen);
void change_value_from_avp(avp_t *avp, uint8_t *value, int vlen);

#endif /* __SIP_CONTROLLER_H__ */
