#ifndef __SIP_CONTROLLER_H__
#define __SIP_CONTROLLER_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SC_SUCCESS 1
#define SC_FAILURE -1

#define SC_TRUE 1
#define SC_FALSE 0

#define SC_KEY_LENGTH 1024
#define SC_ATTR_LENGTH 1024
#define SC_VALUE_LENGTH 16384
#define SC_BUF_LENGTH 16384

#define SC_KEY_IDX 0
#define SC_VALUE_IDX 1

typedef struct sip_msg_st sip_msg_t;
typedef struct avp_st avp_t;
typedef struct vlst_st vlst_t;
typedef struct val_st val_t;

struct val_st
{
  uint8_t *attr;
  int alen;
  uint8_t *val;
  int vlen;
  struct val_st *next;
};

struct vlst_st
{
  int num;
  struct val_st *head;
};

struct avp_st
{
  uint8_t *key;
  int klen;
  struct vlst_st *vlst;
  struct avp_st *next;
};

struct sip_msg_st
{
  int num;
  struct avp_st *head;
  struct avp_st *tail;
};

int read_msg(char *input, char *buf, int max);
sip_msg_t *parse_sip_msg(char *buf, int len);
void free_sip_msg(sip_msg_t *msg);
uint8_t *serialize_sip_msg(sip_msg_t *msg, int *len);
void print_sip_msg(sip_msg_t *msg);

avp_t *init_avp(uint8_t *key, int klen, uint8_t *value, int vlen);
void free_avp(avp_t *avp);

int is_401_unauthorized_msg(sip_msg_t *msg);
int is_200_ok_msg(sip_msg_t *msg);
int get_num_of_avps_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen);
avp_t *get_avp_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen, int idx);
int add_avp_to_sip_msg(sip_msg_t *msg, avp_t *avp, uint8_t *key, int klen, int idx);
void del_avp_from_sip_msg(sip_msg_t *msg, uint8_t *key, int klen, int idx);

int is_attribute_included(avp_t *avp, uint8_t *attr, int alen);
int get_num_of_values_from_avp(avp_t *avp);
uint8_t *get_value_from_avp_by_idx(avp_t *avp, int idx, int *vlen);
uint8_t *get_value_from_avp_by_name(avp_t *avp, uint8_t *attr, int alen, int *vlen);
void change_value_from_avp_by_idx(avp_t *avp, int idx, uint8_t *value, int vlen);
void change_value_from_avp_by_name(avp_t *avp, uint8_t *attr, int alen, uint8_t *value, int vlen);

#endif /* __SIP_CONTROLLER_H__ */
