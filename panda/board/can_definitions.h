#include "dlc_to_len.h"

#define CAN_PACKET_VERSION 2
typedef struct {
  unsigned char reserved : 1;
  unsigned char bus : 3;
  unsigned char data_len_code : 4;
  unsigned char rejected : 1;
  unsigned char returned : 1;
  unsigned char extended : 1;
  unsigned int addr : 29;
  unsigned char data[CANPACKET_DATA_SIZE_MAX];
} __attribute__((packed, aligned(4))) CANPacket_t;

#define GET_BUS(msg) ((msg)->bus)
#define GET_LEN(msg) (dlc_to_len[(msg)->data_len_code])
#define GET_ADDR(msg) ((msg)->addr)

// Flasher and pedal use raw mailbox access
#define GET_MAILBOX_BYTE(msg, b) (((int)(b) > 3) ? (((msg)->RDHR >> (8U * ((unsigned int)(b) % 4U))) & 0xFFU) : (((msg)->RDLR >> (8U * (unsigned int)(b))) & 0xFFU))
#define GET_MAILBOX_BYTES_04(msg) ((msg)->RDLR)
#define GET_MAILBOX_BYTES_48(msg) ((msg)->RDHR)

#define CAN_INIT_TIMEOUT_MS 500U

#define CANPACKET_HEAD_SIZE 5U

#define WORD_TO_BYTE_ARRAY(dst8, src32) 0[dst8] = ((src32) & 0xFF); 1[dst8] = (((src32) >> 8) & 0xFF); 2[dst8] = (((src32) >> 16) & 0xFF); 3[dst8] = (((src32) >> 24) & 0xFF)
#define BYTE_ARRAY_TO_WORD(dst32, src8) ((dst32) = 0[src8] | (1[src8] << 8) | (2[src8] << 16) | (3[src8] << 24))
