0. Caso não tenha lido a explicação do conversor análogo-digital ADS1247 e queira saber mais à respeito, acesse o linki abaixo:
https://github.com/KakiArduino/ADS1247/blob/master/Arduino/Tutorial%20temperatura%20interna.md

1. Introdução

Platinum resistance thermometers (PRT) oferece uma acurácia alta, para uma ampla faixa de temperatura (-200 a 850°C). A acurácia dos sensores variam entre os fabricantes, assim como seu encapsulamento, que varia de acordo com a aplicação. Ao contrário dos termopares do PRTs, o PRT não necessita a utilização de cabos especializados para se conectar ao sensor.

Para ultilizar um PRT, basta medir a resistência. O PT100 possui uma resistência de 100 ohms, 0°C e 138.4 ohms à 100°C. A relação de temperatura e resistência é aproximadamente linear, porém, para obter a medida com alta precisão é necessario linearizar a resistência. Com isso, a definição mais recente da relação entre resistência e temperatura é dada pelo International Temperature Standard 90 (ITS-90), cuja equação de linearização é: Rt = R0 * (1 + A* t + B*t2 + C*(t-100)* t3), onde Rt é a resistência na temperatura (t), R0 é a resistência à 0°C, A= 3.9083 E–3 , B = –5.775 E–7, C = –4.183 E–12 (abaixo de 0°C), ou C = 0 (acima de 0°C). Uma melhor explicação da conversão resistência conforme à temperatura será desenvolvida abaixo. Caso queira saber mais sobre o PT100, segue os links para o site da Omega, aqui e aqui.

2. Conexcões

Completação para o diagrama do turial da leitura de temperatua interna com arduino.
Diagrama do linki: https://drive.google.com/open?id=0B-KEVIklOjN3ZVBqdW9jODN3SU0
