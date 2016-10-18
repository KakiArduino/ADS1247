#include <SPI.h>

const int ADResetPulse = 1;      // ADC reset (min. 4x244 = 976 ns, low), 244 ns = 1/4,096 MHz;
const int ADResetDelay = 1;      // ADC delay afrter reset in ms min 0.6 ms
int mux1settings;
int a =1;


void setup() 
{
 Serial.begin(115200);
 delay (10);
 SPIinitialize(); // Arduino setup SPI.
 ADreset(); // ADC " hardware" reset before sending data.
 ADsetup();
 //Calibra();

}


void loop() 
{ 
   if (digitalRead(9) == 0)
       {
          ADread(); // read data when ADC dataReady is low
       }

}
 
void ADread()
{
 digitalWrite(10, LOW);
 delayMicroseconds(1);        // atraso mínimo de 10 ns (tcssc).
 SPI.transfer(0x12);          // read data (lê o bufer de saída).

 int32_t regData;
    regData |= SPI.transfer(0xff);
    regData <<= 8;
    regData |= SPI.transfer(0xff);
    regData <<= 8;
    regData |= SPI.transfer(0xff);
    Serial.print(regData,HEX);
    Serial.print(" <--> ");
    Serial.print(regData,DEC);
    Serial.print(" <--> ");

// Converção de HEX para float
    double dado;  
    dado= regData;
    dado=((dado*1125)/(8388607*4))+0.79686; // esse 0.79586 é uma correcao
    Serial.print(dado);
    
//Conversão para temperatura em celcius
     long double a,b,c,delta, A=(3.9083/1000), B=(-5.775/10000000), C=(-4.183/1000000000000);
     double T;
     int R0=100;
     
     a = B*R0;
     b = (R0*A);
     c = (R0-dado);
     delta=(b*b)-(4*c*a);
     
     T= (-b + sqrt(delta))/(2*a);

     Serial.print(" <--> ");
     Serial.print(T);
     Serial.println("");
         
  delayMicroseconds(10);        // atraso mínimo de 7x244 = 1708 ns (tsccs).
  digitalWrite(10, HIGH);
  delay(50); 
  
 }

void Calibra ()
{
  digitalWrite(10, LOW);
  delayMicroseconds(1); 
 
  SPI.transfer(0x60); // System offset
  delay(3500);

  SPI.transfer(0x24);
  SPI.transfer(0x01);
     int32_t byte1 = SPI.transfer(0xff),byte2 = SPI.transfer(0xff);
     Serial.println( byte1,HEX);
     Serial.println( byte2,HEX);
  SPI.transfer(0x26);
  SPI.transfer(0x00); 
     int32_t byte3 = SPI.transfer(0xff); 
     Serial.println( byte3,HEX);

   delayMicroseconds(10);        // atraso mínimo de 7x244 = 1708 ns (tsccs).
   digitalWrite(10, HIGH);
   delay(50);
    
   Serial.println("fim da calibracao");
}
 
void ADsetup()
{
  digitalWrite(10, LOW);       // habilita a comunicação SPI.
  delayMicroseconds(1);        // atraso mínimo de 10 ns.
  SPI.transfer(0x06);          // Reset de "software"
  delay(ADResetDelay);         // atraso mínimo de 0,6 ms.
  SPI.transfer(0x16);          // stop data continuous (SDATAC command); Comando tem efeito após concluído conversão do ADC.
  delay(210);                  // Atraso mínimo de 1/SPS (SPS-taxa de amostragem);
  
  SPI.transfer(0x42);           // mux1      (write command)
  SPI.transfer(0x01);           // sys0      (write command) -------------
  SPI.transfer(0x20);           // mux1      Ativa a ref. interna (e IDAC) e seleciona VrefExt.
  SPI.transfer(0x20);           // sys0      ganho 4 e taxa de amostragem 5/s ---------
  
  SPI.transfer(0x40);           // mux0       (write command)
  SPI.transfer(0x00);           // não seleciona registrador
  SPI.transfer(0x0A);           // Ain1 Adc positive input; Ain2 negative inout (lê o PT100).
  
  SPI.transfer(0x4A);           // IDAC0  (escreve no registro IDAC0) 
  SPI.transfer(0x00);           // não seleciona registrador
  SPI.transfer(0x06);           // configura IDAC1 e IDAC2 (fonte de corrente 1 e 2) para 1mA.
  
  SPI.transfer(0x4B);           // IDAC1 (escreve no registro IDAC1)  
  SPI.transfer(0x00);           // não seleciona registrador
  SPI.transfer(0x03);           // Seleciona a fonte 1 na Ain0 e fonte 2 na Ain3

  delayMicroseconds(10);        // atraso mínimo de 7x244 = 1708 ns (tsccs).
  digitalWrite(10, HIGH);       // libera o barramento de comunicação SPI.
  }

  
void ADreset()  // Reset de "hardware"
{
 digitalWrite( 8, LOW);
 delay(ADResetPulse);
 digitalWrite( 8 , HIGH);
 delay(ADResetDelay); 
 }

 
void SPIinitialize()
{
 // Arduino ADC interface (wire accordingly)
 // inverse means that when pin is low relavent function is active e.g.:
 // DRDY low means ADC has data to send
 pinMode(10, OUTPUT); // pin 10 as CS (inverse) - CS = Chip Select, habilita comunicação (em low) SPI com ADS1247.
 pinMode(9, INPUT); // pin 09 as DRDY (inverse) - LOW indica conversão (ADC) completa.
 pinMode(8, OUTPUT); // pin 08 as reset (inverse) - LOW (min. 4x244 ns =976 ns; 244 ns = 1/4,096 MHz) p/ reset.
 pinMode(7, OUTPUT); // pin 07 as start - High (min. 3x244 = 732 ns) inicia conversão (ADC). POde ficar high sempre.
 //attachInterrupt(digitalPinToInterrupt(2) , ADread, LOW); // pin 09 as DRDY (inverse)
 digitalWrite(10, HIGH); // pins set to high, desablita a comunicação SPI. 
 digitalWrite(8 , HIGH); // operação normal, reset em LOW.
 digitalWrite(7 , HIGH); // operação normal, ADC convertendo continuamente.
 
 // Arduino SPI settings (change first line according to IC specifications)
 // defines SPI comunication type and initializes module
 SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE1)); // SPI setup SPI communication
 delay(200); // SPI delay for initialization
 SPI.begin(); // SPI Module 'wake up'
 
}
