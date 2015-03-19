#define	MBADDBASE	(0x03FEC100)

#define	MBADD_TX00	(0x03FEC100)
#define	MBADD_TX01	(0x03FEC120)
#define	MBADD_TX02	(0x03FEC140)
#define	MBADD_TX03	(0x03FEC160)

#define	MBADD_RX00	(0x03FEC180)
#define	MBADD_RX01	(0x03FEC1A0)
#define	MBADD_RX02	(0x03FEC1C0)
#define	MBADD_RX03	(0x03FEC1E0)
#define C0MCTRL0        (0x03FEC10E)


void init_CAN();
void init_CAN0_Buffer();
void init_Transmit_Buffer(unsigned long,unsigned char,unsigned short,unsigned short);
void init_Receive_Buffer(unsigned long,unsigned short,unsigned short);
void CanMsgGetData(unsigned long sttp_MsgRegTypeBuf, unsigned char* u1t_RxDatabuf, unsigned char num);
void FastRecoverAFCANChannel (unsigned char channel);

