#ifndef __SIP_CONTROLLER_H__
#define __SIP_CONTROLLER_H__

#include <stdint.h>

typedef struct sip_msg_st sip_msg_t;
typedef struct avp_st avp_t;

int read_msg(char *input, char *buf, int max);
sip_msg_t *parse_sip_msg(char *buf, int len);
int get_value_by_key(sip_msg_t *msg, char *key, int klen, char *val, int max);
int replace_value_by_key(sip_msg_t *msg, char *key, int klen, char *value, int vlen);

#endif /* __SIP_CONTROLLER_H__ */
