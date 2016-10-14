<pre>#include<SPI.h>
const int SPIInitDelay = 200;    // SPI initialization delay
const int SPIDelay = 20;         // SPI delay for reset/start pin
const int ADResetPulse = 1;      // ADC reset pulse length in us defined by ADTreset()
const int ADResetDelay = 1;      // ADC delay afrter reset in ms min 0.6 ms
long dataIN[3];
 
void setup()
{
 SPIinitialize(); // Arduino setup SPI
// AD1247 wait 20ms, after power up, to start sending data. 2s delay in SPIinitialize
 ADreset(); // ADC reset before sending data.
 ADsetup();
 Serial.begin(115200);
}
 
void loop()
{
 if (digitalRead(9) == 0){ADread();}    // read data when ADC dataReady is low
}
 
void ADread()
{
 digitalWrite(10, LOW);
 SPI.transfer(0x12);
 for (int i = 0; i <= 2; i++){
 dataIN[i] = SPI.transfer(0xff);
 Serial.print(dataIN[i],HEX);
 }
 
 digitalWrite(10, HIGH);
 Serial.println(&amp;quot;&amp;quot;);
 }
 
void ADsetup()
{
 digitalWrite(10, LOW);
 SPI.transfer(0x42); // mux1 (write command)
 SPI.transfer(0x01); // sys0 (write command)
 SPI.transfer(0x30); // mux1 (data) - antes 33 agora 30
 SPI.transfer(0x00); // sys0 (data) - antes 10 agora 00
 SPI.transfer(0x40); // mux0 (write command)
 SPI.transfer(0x00); // não escreve nada
 SPI.transfer(0x01); // Ain1 Adc positive input; Ain0 negative inout
 SPI.transfer(0x16); // stop data conti (SDATAC command)
 delay(210); // delay to avoid read errors (em 24set16)
 digitalWrite(10, HIGH);
// delay(210); // delay to avoid read errors (comentado em 24set16)
 }
 
void ADreset()
{
 digitalWrite( 8, LOW);
 delay(ADResetDelay);
 digitalWrite( 8 , HIGH);
 delay(ADResetDelay);
 }
 
void SPIinitialize()
{
 // Arduino ADC interface (wire accordingly)
 // inverse means that when pin is low relavent function is active e.g.:
 // DRDY low means ADC has data to send
 pinMode(10, OUTPUT); // pin 10 as CS (inverse)
 pinMode(9, INPUT); // pin 09 as DRDY (inverse)
 pinMode(8, OUTPUT); // pin 08 as reset (inverse)
 pinMode(7, OUTPUT); // pin 07 as start
 //attachInterrupt(digitalPinToInterrupt(2) , ADread, LOW); // pin 09 as DRDY (inverse)
 digitalWrite(10, HIGH); // pins set to high
 digitalWrite(8 , HIGH);
 digitalWrite(7 , HIGH);
 
 // Arduino SPI settings (change first line according to IC specifications)
 // defines SPI comunication type and initializes module
 SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE1)); // SPI setup SPI communication
 delay(SPIInitDelay); // SPI delay for initialization
 SPI.begin(); // SPI Module 'wake up'
}

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
