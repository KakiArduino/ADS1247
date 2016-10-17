#line 1 "C:/Users/Daniel/Documents/Unipampa/Kakuno/Kaki/Firmware/0.1/USB_Comm.c"
 unsigned char HidReadBuff[64] absolute 0x500;
 unsigned char HidWriteBuff[64] absolute 0x540;

 char count;
 void interrupt()
 {
 Usb_Interrupt_Proc();
 }
 void main()
 {
 HID_Enable(&HidReadBuff, &HidWriteBuff);
 while(1)
 {
 while (!HID_Read());
 for(count=0; count<64; count++)
 HidWriteBuff[count]=HidReadBuff[count];
 while(!Hid_Write(HidWriteBuff, 64));
 }
 }
