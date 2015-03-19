#pragma ioreg

//#pragma	interrupt	INTC0REC	receive_CAN0_Buffer
#pragma  interrupt INTC0ERR  receive_CAN0_Error
#pragma  interrupt INTC0WUP  receive_CAN0_Wakeup

#include "CAN.h"

//extern void receive_display(unsigned char);
extern unsigned char Gears_data[8];

unsigned char u1t_RxDatabuf[8];
unsigned char u1t_RxData[8];
void receive_CAN0_Buffer();

void init_CAN0_Buffer(){
	/*
	 * CAN message buffer initialization
	 */
		unsigned char		u1t_bufno;
		unsigned long		sttp_MsgRegTypeBuf;

		/* Init all message buffer */
		for (u1t_bufno = 0 ; u1t_bufno < 16 ; u1t_bufno++) {
			/*	Set CAN message buffer[n] register address */
			sttp_MsgRegTypeBuf = (MBADDBASE + (0x20 * u1t_bufno));

			/* Clear RDY bit */
			*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0001;

			/* Clear MA0 bit */
			*((unsigned char *)(sttp_MsgRegTypeBuf + 0x09)) &= 0xf8;

			/* Clear TRQ, DN bit */
			*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0006;
	}
}


void init_CAN(){

  PMC3L.3 = 1;		/* init P33/CTXD0 */
  PFC3L.3 = 0;
  PFCE3L.3 = 1;
  
  PMC3L.4 = 1;		/* init P34/CRXD0 */
  PFC3L.4 = 0;
  PFCE3L.4 = 1;
  
  BPC = 0x8ffb;
	C0GMCS = 0;
	C0GMCTRL = 0x100; /* Set GOM*/
	C0CTRL = 0x807f;	/*set init mode*/
	
	C0BRP = 1; /*5; /* fXP1=8MHz 500K bps */
	
	C0BTR = 0x0104; /* 0x030a; /* 75% sample */
	init_CAN0_Buffer();
	
	
	init_Transmit_Buffer(MBADD_TX00,8,0x0,0x1ddc);	/* Standard ID  0x777 for Velocity signal */
	
	init_Receive_Buffer(MBADD_RX00,0x0,0x0224); /*st id 0x089*/
	
  C0IE = 0x03e01;   //0x023d;  										/* Set C0IE register:CIE1(Reception Interrupt) bit is set to 1 */
	C0RECIF = 0;
	//C0RECMK = 0;			/*enable receive complete interrupt */
	C0ERRIF = 0;      /*enable receive error interrupt */
	C0ERRMK = 0;
	C0WUPIF = 0;      /*enable wakeup interrupt */
	C0WUPMK = 0;
	
	C0CTRL = 0x817e;/*set normal mode*/
	//C0CTRL =0x0502;		/* Set self-test mode */
	/*C0CTRL = 0x0100;*/
}

void init_Transmit_Buffer(unsigned long sttp_MsgRegTypeBuf,unsigned char num,unsigned short MIDL,unsigned short MIDH)
{
		/*	Set CAN message buffer[n] register address */

														/* Set MCONF register */
		*((unsigned char *)(sttp_MsgRegTypeBuf + 0x09)) = 0x01;

														/* Set MIDH,MIDL register */
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0a)) = MIDL;
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0c)) = MIDH;

														/* Set MDLC register */
		*((unsigned char *)(sttp_MsgRegTypeBuf + 0x08)) = 0x08;

														/* Clear MDATA register */
		{
			unsigned char	u1t_datactr;
			for(u1t_datactr = 0 ; u1t_datactr < num ; u1t_datactr++) {
				*((unsigned char *)(sttp_MsgRegTypeBuf + 0x00 + u1t_datactr)) = 0x00;
			}
		}

														/* Set MCTRL register */
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x001e;

														/* Set RDY bit */
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0100;

}

void init_Receive_Buffer(unsigned long MBADDRX,unsigned short MIDL,unsigned short MIDH){

	  unsigned long	sttp_MsgRegTypeBuf;

		sttp_MsgRegTypeBuf = (MBADDRX);				/*	Set CAN message buffer[n] register address */

														/* Set MCONF register */
		*((unsigned char *)(sttp_MsgRegTypeBuf + 0x09)) = 0x09;

														/* Set MIDH,MIDL register */
		 *((unsigned short *)(sttp_MsgRegTypeBuf + 0x0a)) = MIDL;
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0c)) = MIDH;

														/* Set MCTRL register:Set IE bit,Clear MOW,DN,TRQ bit */
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0816;

														/* Set RDY bit */
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0100;
}


void transfer_CAN0_Buffer(unsigned long sttp_MsgRegTypeBuf, unsigned char* u1t_TxDatabuf, unsigned char num){
		/* Check TRQ bit */
		unsigned short	u2t_MCTRL;
		u2t_MCTRL = *((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e));
		//if((u2t_MCTRL & 0x0002) != 0) {
			//return;
		//}

/*	{													/* Set Tx data */
/*		signed int	s4t_ctr;
		for(s4t_ctr = 1 ; s4t_ctr < num ; s4t_ctr++) {
			/*u1t_TxDatabuf[s4t_ctr] = u1t_txdata++;*/
	/*		u1t_TxDatabuf[s4t_ctr] = 0x55;
		}		
		
	}*/

														/* Clear RDY bit */
	*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0001;

														/* Check RDY bit */
	if((*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) & 0x0001) == 0){

		{
			unsigned char	u1t_MDLCctr;
														/* Set MDATA register */
			for(u1t_MDLCctr = 0 ; u1t_MDLCctr < 8 ; u1t_MDLCctr++) {
				*((unsigned char *)(sttp_MsgRegTypeBuf + 0x00 + u1t_MDLCctr)) = \
											u1t_TxDatabuf[u1t_MDLCctr];
			}
		}
														/* Set RDY bit */
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0100;

														/* Set TRQ bit */
		*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0200;
	}
	//while((u2t_MCTRL & 0x02) == 0x02);
	return;
}

/*void transfer_CAN0_Buffer(unsigned short* start_addr, unsigned char* pdata, unsigned char num){
	unsigned char i;
	unsigned short* C0MDLCm = start_addr + 8;
	unsigned short* C0MCONFm = start_addr + 9;
	
	unsigned short* C0MCTRLm = start_addr + 0xe;
	C0MDLCm[0] = num;
	
	for(i = 0; i < num; i ++){
		start_addr[i] = pdata[i];
	}
	
	C0MCONFm[0] = 0x1;
	C0MCTRLm[0] = 0x908; // RDY = 1
	
	while((C0MCTRLm[0] & 0x2) == 0x2);
	
	C0MCTRLm[0] = 0x0200;     /*SEND MESSAGE*/
	
/*	while((C0MCTRLm[0] & 0x02) == 0x02); /*WAIT FOR ACK */
/*	return;
}*/

//-----------------------------------------------------------------------------
// Module name: FastRecoverAFCANChannel
// Description: Fast recover AFCAN channel after error detection
//-----------------------------------------------------------------------------
void FastRecoverAFCANChannel (unsigned char channel)
{
        unsigned long		sttp_MsgRegTypeBuf;
        sttp_MsgRegTypeBuf = ( MBADDBASE + (channel * 0x600) );

        *((unsigned short *)(sttp_MsgRegTypeBuf + 0x12e)) = 0x001F; // Clear TRQ flag of CnMCTRL0

        // Make sure that the CAN Macro is in Init mode
        *((unsigned short *)(sttp_MsgRegTypeBuf + 0x50)) = 0x001F; // Set OPMODE and PSMODE to init mode in CnCTRL

        *((unsigned short *)(sttp_MsgRegTypeBuf + 0x50)) = 0x8000; // Clear error counters in CnCTRL
        *((unsigned short *)(sttp_MsgRegTypeBuf + 0x58)) = 0x003f; // Clear interrupts status register in CnINTS


      	*((unsigned short *)(sttp_MsgRegTypeBuf + 0x50)) = 0x0100; // switch to operation mode Opmode0 = 1 again in CnCTRL
}


__interrupt void receive_CAN0_Error(){
	C0INTS = 0x1c;
	if((C0INFO & 0x10)>>4)  //BOFF=1;
	{
	  FastRecoverAFCANChannel(0);
	}
	//CINTS = 0x0c;
}

__interrupt void receive_CAN0_Wakeup(){
	C0INTS = 0x20;
	//TMMK000 = 0x00;
}

void CanMsgGetData(unsigned long sttp_MsgRegTypeBuf, unsigned char* u1t_RxDatabuf, unsigned char num)
{
  unsigned char	u1t_MDLC;
  //unsigned char u1t_RxDatabuf;
  /* Clear DN bit */
  *((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0004;
  
  for(u1t_MDLC=0;u1t_MDLC<num;u1t_MDLC++){
	  u1t_RxDatabuf[u1t_MDLC] = (*((unsigned char *)(sttp_MsgRegTypeBuf + u1t_MDLC)));
  }
}

#if 0
__interrupt void receive_CAN0_Buffer(){
	unsigned char	u1t_bufno_cnt;
	unsigned long	sttp_MsgRegTypeBuf;
  
  C0INTS = 0x02;
switch(C0LIPT) {
    case 4: // buffer 2 ID:
      CanMsgGetData(MBADD_RX00,u1t_RxData,8); 
      break;
    case 5: // buffer 3 ID:
      CanMsgGetData(MBADD_RX01,u1t_RxData,8); 
      break;
    default:
      break;
  }
	//receive_display(u1t_RxData[0]);
}
#endif

void receive_CAN0_Buffer(){
	unsigned char		u1t_bufno_cnt;
	unsigned long	  sttp_MsgRegTypeBuf;
	unsigned char	u1t_MDLC;
	unsigned char	u1t_MDLCctr;
	
	if((C0INTS & 0x0002) == 2)
	{
	  
		C0INTS |=0x0002;
	  if((C0RGPT & 0x0001) == 1)  //ROVF=1?
	  {
			C0RGPT |=0x0001;
		}
	  sttp_MsgRegTypeBuf = (MBADDBASE + (0x20 * 4));
	  if(((*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e))) & 0x0004) != 0) 
	  {
	  	CanMsgGetData(MBADD_RX00,u1t_RxData,8); 
	  	transfer_CAN0_Buffer(MBADD_TX00, Gears_data, 8);
	  }
	  else
	  {
	    //receive_display(0x0e);
	  }
		//if((CRGPT&0x0002)==0)	/* RHPM=0  if there is buffer has  messages */
#if 0		
		{
			for(u1t_bufno_cnt = 0 ; u1t_bufno_cnt < 16 ; u1t_bufno_cnt++)
			{
				/* Set CAN message buffer[n] register address */
				sttp_MsgRegTypeBuf = (MBADDBASE + (0x10 * u1t_bufno_cnt));

				if(((*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e))) & 0x0004) != 0) 
				{
																/* Clear DN bit */
					*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e)) = 0x0004;
					/* Get Rx message */
					
					/* Get MDLC register */
					u1t_MDLC = *((unsigned char *)(sttp_MsgRegTypeBuf + 0x08));
					/* Get MDATA register */
					for(u1t_MDLCctr = 0 ;((u1t_MDLCctr < u1t_MDLC) && (u1t_MDLCctr < 8)) ; u1t_MDLCctr++)
					{
							u1t_RxData[u1t_MDLCctr] = *((unsigned char *)(sttp_MsgRegTypeBuf + 0x00 + u1t_MDLCctr));
					}
					/* Check DN & MUC bit */
					if (((*((unsigned short *)(sttp_MsgRegTypeBuf + 0x0e))) & 0x2004) != 0)
				  {
						break;
					}
					//LED_Disp();  //7Seg LED display
				}
			}
    }
#endif    
	}
}
