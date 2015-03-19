#pragma ioreg 

void Port_Init(void)
{
  PMC0.2 = 1;
  PFCE0.2 = 1;
  PFC0.2 = 0;   /* set P02 as TIAA40 */
  
  PMC9L.6 = 1;
  PMC9L.7 = 1;
  PFCE9L.6 = 1;
  PFCE9L.7 = 1;
  PFC9L.6 = 1;
  PFC9L.7 = 1;  /* set P96 as TOAA21, P97 as TOAA20 */
  
  PMCCM.1 = 1;  /* set PCM1 as CLKOUT */  
  
}



