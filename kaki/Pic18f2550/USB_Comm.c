 unsigned char HidReadBuff[64]  absolute 0x500;     // Buffers should be in USB RAM
 unsigned char HidWriteBuff[64] absolute 0x540;

 char count;
    void interrupt()
    {
     Usb_Interrupt_Proc();
    }
    void main()
    {
      HID_Enable(&HidReadBuff, &HidWriteBuff); //Habilita a comunicação USB
      while(1)
      {
        while (!HID_Read());
        for(count=0; count<64; count++)
        HidWriteBuff[count]=HidReadBuff[count];
        while(!Hid_Write(HidWriteBuff, 64));
      }
    }