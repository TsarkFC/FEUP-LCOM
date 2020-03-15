#define MOUSE_IRQ 12
#define AUX BIT(5)

#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)
#define NEG_TO_POS BIT(8)

#define WRT_BYTE_MOUSE 0xD4

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define EN_DATA_REPOR 0xF4
#define DISABLE_DATA_REPOR 0xF5

#define MOU_DIS BIT(5)
#define FIRST_BYTE BIT(3)
#define EN_MOU_KBC_INT BIT(0)|BIT(1)
#define EN_MOU_KBC 0xCF

#define REMOTE_MODE 0xF0
#define STREAM_MODE 0xEA
#define READ_DATA 0xEB
