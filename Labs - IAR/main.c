void main()
{ 
   DIO_Init(PORTF,1,1);
   DIO_Init(PORTF,2,1);
   DIO_Init(PORTF,3,1);

   DIO_WritePort(PORTF,0x2);
   DIO_WritePort(PORTF,0x04);
   DIO_WritePort(PORTF,0x08);
   //DIO_WritePin(PORTF,3,1);
   
   while(1){};
}

