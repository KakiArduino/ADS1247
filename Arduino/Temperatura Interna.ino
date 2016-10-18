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
 SPI.transfer(0x12);          // Transfere a leitura do ADC para o buffer de saida
 /* A estrutura abixo salva byte a byte do bauffer de saida do ADC, na mesma vairiavel (regData), e com o bit-shifting,
 quando estamos salvando os bytes em um vertor, perdiamos o zero à esquerda o que complicava, na hora da converção para DEC */ 
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
 }

void ADsetup()
{
  digitalWrite(10, LOW);       // habilita a comunicação SPI.
  delayMicroseconds(1);        // atraso mínimo de 10 ns.
  SPI.transfer(0x06);          // Reset de "software"
  delay(ADResetDelay);         // atraso mínimo de 0,6 ms.
  SPI.transfer(0x16);          // stop data continuous (SDATAC command); Comando tem efeito após concluído conversão do ADC.
  delay(210);                  // Atraso mínimo de 1000(miliseconds)/taxa de amostragem (SPS), no nosso caso é 5.
  
  SPI.transfer(0x42);           // mux1      Seleciona o registro para escrita (write command)
  SPI.transfer(0x01);           // sys0      (write command) - o registro sys0  é 0x43 (0x42 + 0x01)
  SPI.transfer(0x33);           // mux1      Ativa a ref. interna (e IDAC) e seleciona VrefExt.
  SPI.transfer(0x00);           // sys0      Ganho 1 e taxa de amostragem 5/s,
// Na leitura da temperatura interna assume-se sempre ganho unitario, independente do ganho selecionado.
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
