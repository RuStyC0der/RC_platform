
#define PARSE_AMOUNT 6       // число значений в массиве, который хотим получить
#define INPUT_AMOUNT 20      // максимальное количество символов в пакете, который идёт в сериал
char inputData[INPUT_AMOUNT];  // массив входных значений (СИМВОЛЫ)
byte intData[PARSE_AMOUNT];     // массив численных значений после парсинга
boolean recievedFlag;
boolean getStarted;
byte index;
String string_convert;


#define SECURITY_KEY 121 // Security key value must be same on transmitter and receiver



/* Control has 6 item,
0 is forward
1 is back
2 is left flag
3 is right flag
4 is motor power
5 is rotation sensivity
6 is constant for noise cut and security
*/


#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб


void parsing() {
while (Serial.available() > 0) {
  char incomingByte = Serial.read();      // обязательно ЧИТАЕМ входящий символ
  if (incomingByte == '$') {              // если это $
    getStarted = true;                    // поднимаем флаг, что можно парсить
  } else if (incomingByte != ';' && getStarted) { // пока это не ;
    // в общем происходит всякая магия, парсинг осуществляется функцией strtok_r
    inputData[index] = incomingByte;
    index++;
    inputData[index] = '\0';
  } else {
    if (getStarted) {
      char *p = inputData;
      char *str;
      index = 0;
      String value = "";
      while ((str = strtok_r(p, " ", & p)) != NULL) {
        string_convert = str;
        intData[index] = string_convert.toInt();
        index++;
      }
      index = 0;
    }
  }
  if (incomingByte == ';') {        // если таки приняли ; - конец парсинга
  getStarted = false;
  recievedFlag = true;
  }
}
}

void setup() {
  Serial.begin(9600); //открываем порт для связи с ПК

  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x65);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_HIGH); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}

void loop() {

    parsing();
    if (recievedFlag) {                           // если получены данные
    recievedFlag = false;
    radio.write(&intData, sizeof(intData));
    Serial.print(intData[0]);
    Serial.print(" ");
    Serial.print(intData[1]);
    Serial.print(" ");
    Serial.print(intData[2]);
    Serial.print(" ");
    Serial.print(intData[3]);
    Serial.print(" ");
    Serial.println(intData[4]);
}

  }
