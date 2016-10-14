1. Introdução

O ADS1247 é um conversor sigma-delta análogo-digital (ADC) de 24bits, que possui maior precisão quando comparado ao ADC do arduino, de 8bits. Além da maoir precisão, o AD1247 possui uma taxa de amostragem que pode ser configurada para realizar de 5 a 2000 amostragens por segundo (SPS), além de possuir comunicação através da interface SPI. Caso não tenha o datasheet, segue o link do site da Texas Instruments (TI) para sua obtenção e, para a obtenção do programa de um Arduino versão 1.6 ou superior, segue outra sugestão de link para baixá-lo.

2. Conexões

Interface SPI entre Arduino e ADS1247:

    SCLK ( pino 13 ao 20 ) – Clock da comunicação gerado pelo arduino;

    MOSI ( pino 11 ao 19 ) – Linha de comunicação do Ardino ao ADS1247;

    MISO ( pino 12 ao 18 ) – Linha de comunicação do ADS1247 ao Arduino;

    CS/SS( pino 10 ao 16 ) – Seleciona dispositivo SPI a ser comunicado.

Interface própria do ADS1247, usada em conjunto com o SPI:

    START ( pino 15 ) Nível um (1) – Inicia a conversão (ADC). Para sincronizar (ou iniciar) o ADC via software, basta enviar o comando SYNC (04h, 05h). Ativa a parte analógica;

    DRDY ( pino 17 ) – Quando em nível zero, significa que o ADS1247 tem dados para enviar ao arduino.

Funcionamento do ADS1247:

    RESET ( pino 4 ) – Ativo em zero (0), conectado cir. RC (100 nF e 10 kOhm) para forçar o reset na conexão da alimentação;

    VRefOut ( pino 7 ) – Conectar o capacitor de 1 a 47 uF ao VRefCom;

    VRefCom ( pino 8 ) – Conectar ao negativo (ou terra) da fonte analógica (AVSS).

ads1247conec

3. Comunicação

A comunicação SPI não será explicada aqui, caso tenha dúvidas a seu respeito, o site do arduino poderá ajudá-lo. Para comunicar-se com o ADS1247, é necessário entender alguns de seus comandos básicos e sua estrutura de comunicação. Os treze comandos encontram-se na página 45 no datasheet, mas para obter um funcionamento básico, basta utilizar os três comandos seguintes:

    NOP (0xFF)  No Operation – É um comando de não operação. Ou seja, quando o arduino manda este comando, ele apenas gera um sinal de clock, não uma ordem;

    RDATA (0x12) Read Data Once – O comando carrega o resultado da última conversão para o registro de saída. O resultado da conversão então é trasmitido, enviando 24 SCLKs. Por exemplo, o arduino envia o comando de leitura e, em seguida, três NOPs para receber os dados durando a sua transmissão;

    WREG ( 0x4R ) Write Register – Comando informando o ADS1247 em qual registro haverá uma escrita. O arduino manda o 0x4R, onde o R é o número do registro. Este registro será explicado com mais detalhe no parágrafo abaixo, pois há alguns  detalhes a serem levados em consideração.

A esturutura de comunicação para configurar os registros funciona em pares. Ou seja, para configurar 2 registros, é necessário comunicar o índice dos dois registros e em seguida suas configurações. Caso queira-se configurar apenas um registro, envie o índice, um NOP e em seguida a configuração desejada.

Exemplo de configuração errada:

mux1 (0x42), sys0 (0x43), configuração do mux1(0x33), configuração do sys0(0x10).

Apesar dessa comunicação estar com a estrutura certa, o valor do sys0 encontra-se errado. Para configurar o segundo registro, deve-se usar a diferença dos dois registros, neste caso, o sys0 deve ser 0x01.

4. Registros

Os registros do ADS1247 encontram-se na pagina 55 do datasheet. Em seguida, há uma explicação de dois registros que são usados nesse exemplo.

    MUX1 (0x02) – Controlador do Multiplexer. Neste registro, configura-se o clock a ser utilizado ( interno ou externo ), o controle da referência interna, seleciona a referência e monitor do sistema;

    SYS0 (0x03) –  Controle do Sistema. Configua-se o ganho do PGA e o número de amostras por segundo.

5. Pseudo código para Arduino (Para facilitar a interpretação, referenciei as funções do código final no pseudo código).

//setup()
//SPIinitialize()
especifica os pinos do CS, DRDY, reset, start
colocar pinos em estado logico alto do CS, reset, start
configura SPI (velocidade da comunicação, MSBFIRST, SPI_MODE1)
espera pelomenos 2ms
Inicializa o modulo SPI //fim do SPIinitialize()
//ADreset()
coloca em estado logico baixo o pino do reset
espera no minimo 0.6 ms
coloca em estado logico alto o pino do reset
espera no minimo 0.6 ms //fim do Adreset()
//ADsetup()
coloca pino do CS em estado logico baixo
configura os registros por exemplo mux1, sys0 //fim do ADsetup()
inicializa comunicação serial // fim do setup()
//loop()
//Adread()
coloca em estado logico baixo o pino do CS
manda 0x12 via SPI
manda3 bytes de NOP
coloca em estado logico alto o pino do CS

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
