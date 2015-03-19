#include "CAN.h"
#pragma ioreg
#pragma interrupt INTTAA4CC0 TimerAA4_int
#pragma interrupt INTTAA4OV TimerAA4_over_int
#pragma interrupt INTC0REC CAN0_reception_int

unsigned short over_count; 
unsigned short cap_count;
unsigned short transfer_count;
unsigned char transfer_enable;
unsigned char Gears_data[8];
unsigned char data_array[1000][3];
unsigned short tm_cap_val;
unsigned short tm_cap_ov;



void main()
{
  __DI();
 
  Clock_Init();
  Port_Init();
  init_CAN();
  
  TAA2CTL0 = 0x04; /* count clock=8M/16=500K, Cycle=2us */
  TAA2CTL1 = 0x04; /* set PWM mode */
  TAA2CCR0 = 0x012C; /* Cycle=2us*300=600us */
  TAA2CCR1 = 0x0096; /* Duty=150/300=50% */
  TAA2IOC0 = 0x05;   /* Timmer0 out put enable */
  
  TAA4CTL0 = 0x04; /* count clock=8M/16=500K, Cycle=2us */
  TAA4CTL1 = 0x06; /* set Pulse width measurement mode */
  TAA4CCR0 = 0x0000; 
  TAA4IOC1 = 0x03; /* set TIAA40 detection of both edges */
  
  TAA4CCIC0 = 0x07; /* enable INTTAA4CC0, set interrupt priority level7 */
  TAA4OVIC = 0x07;  /* enable INTTAA4OV, set interrupt priority level7 */
  
  C0RECIC = 0x06;  
  
  over_count = 0;
  cap_count = 0;
  
  TAA2CTL0.7 = 1;
  
  __EI();  
  
  while(1)
  {
    if (transfer_enable==1)
     {     
       for (transfer_count = 0; transfer_count < 1000; transfer_count++)
        {
	 Gears_data[0] =data_array[transfer_count][0] ;
	 Gears_data[1] =data_array[transfer_count][1] ;
	 Gears_data[2] =data_array[transfer_count][2] ;
	 transfer_count ++;
	 Gears_data[3] =data_array[transfer_count][0] ;
	 Gears_data[4] =data_array[transfer_count][1] ;
	 Gears_data[5] =data_array[transfer_count][2] ;
	  
	 while ((*((unsigned short *)C0MCTRL0) & 0x0002) == 0x0002)
          {	  
            __asm("nop");  
	    __asm("nop"); 
	    __asm("nop"); 
          } 
	  
	  transfer_CAN0_Buffer(MBADD_TX00, Gears_data, 8);
	}
	
       transfer_enable = 0;
     }
     
  }
   
}


void TimerAA4_int(void)
{
  tm_cap_val = TAA4CCR0;
  tm_cap_ov  = over_count;
  data_array[cap_count][2] = tm_cap_ov;
  data_array[cap_count][0] = (unsigned char) tm_cap_val;
  data_array[cap_count][1] = (unsigned char) tm_cap_val>>8;
  cap_count ++;
  over_count = 0;
  
  if (cap_count==1000)
   {
     TAA4CTL0.7 = 0; 
     transfer_enable = 1;
   }
   
}

void TimerAA4_over_int(void)
{
  over_count ++;
}

void CAN0_reception_int(void)
{
  if(Gears_data[0]==0xAA && Gears_data[1]==0x55 && Gears_data[2]==0xAA && Gears_data[3]==0x55)
   {
     TAA4CTL0.7 = 1;
   }
	
}









