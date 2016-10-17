
_interrupt:

;USB_Comm.c,5 :: 		void interrupt()
;USB_Comm.c,7 :: 		Usb_Interrupt_Proc();
	CALL        _USB_Interrupt_Proc+0, 0
;USB_Comm.c,8 :: 		}
L_end_interrupt:
L__interrupt10:
	RETFIE      1
; end of _interrupt

_main:

;USB_Comm.c,9 :: 		void main()
;USB_Comm.c,11 :: 		HID_Enable(&HidReadBuff, &HidWriteBuff); //Habilita a comunicação USB
	MOVLW       _HidReadBuff+0
	MOVWF       FARG_HID_Enable_readbuff+0 
	MOVLW       hi_addr(_HidReadBuff+0)
	MOVWF       FARG_HID_Enable_readbuff+1 
	MOVLW       _HidWriteBuff+0
	MOVWF       FARG_HID_Enable_writebuff+0 
	MOVLW       hi_addr(_HidWriteBuff+0)
	MOVWF       FARG_HID_Enable_writebuff+1 
	CALL        _HID_Enable+0, 0
;USB_Comm.c,12 :: 		while(1)
L_main0:
;USB_Comm.c,14 :: 		while (!HID_Read());
L_main2:
	CALL        _HID_Read+0, 0
	MOVF        R0, 1 
	BTFSS       STATUS+0, 2 
	GOTO        L_main3
	GOTO        L_main2
L_main3:
;USB_Comm.c,15 :: 		for(count=0; count<64; count++)
	CLRF        _count+0 
L_main4:
	MOVLW       64
	SUBWF       _count+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_main5
;USB_Comm.c,16 :: 		HidWriteBuff[count]=HidReadBuff[count];
	MOVLW       _HidWriteBuff+0
	MOVWF       FSR1 
	MOVLW       hi_addr(_HidWriteBuff+0)
	MOVWF       FSR1H 
	MOVF        _count+0, 0 
	ADDWF       FSR1, 1 
	BTFSC       STATUS+0, 0 
	INCF        FSR1H, 1 
	MOVLW       _HidReadBuff+0
	MOVWF       FSR0 
	MOVLW       hi_addr(_HidReadBuff+0)
	MOVWF       FSR0H 
	MOVF        _count+0, 0 
	ADDWF       FSR0, 1 
	BTFSC       STATUS+0, 0 
	INCF        FSR0H, 1 
	MOVF        POSTINC0+0, 0 
	MOVWF       POSTINC1+0 
;USB_Comm.c,15 :: 		for(count=0; count<64; count++)
	INCF        _count+0, 1 
;USB_Comm.c,16 :: 		HidWriteBuff[count]=HidReadBuff[count];
	GOTO        L_main4
L_main5:
;USB_Comm.c,17 :: 		while(!Hid_Write(HidWriteBuff, 64));
L_main7:
	MOVLW       _HidWriteBuff+0
	MOVWF       FARG_HID_Write_writebuff+0 
	MOVLW       hi_addr(_HidWriteBuff+0)
	MOVWF       FARG_HID_Write_writebuff+1 
	MOVLW       64
	MOVWF       FARG_HID_Write_len+0 
	CALL        _HID_Write+0, 0
	MOVF        R0, 1 
	BTFSS       STATUS+0, 2 
	GOTO        L_main8
	GOTO        L_main7
L_main8:
;USB_Comm.c,18 :: 		}
	GOTO        L_main0
;USB_Comm.c,19 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
