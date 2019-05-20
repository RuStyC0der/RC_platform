

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги


#define SECURITY_KEY 121 // Security key value must be same on transmitter and receiver

byte dataArray[7];

/* Control has 6 item,
0 is forward
1 is back
2 is left flag
3 is right flag
4 is motor power
5 is rotation sensivity
6 is a security key
*/

// change values in this block to your values)
int motor_pwm = 6;
int rotation_pwm = 5;

//motor pins
int motor_f = 8;
int motor_s = 7;

//rotation motor pins
int rotation_f = 3;
int rotation_s = 2;


byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб

void setup(){

  pinMode(motor_pwm, OUTPUT);
  pinMode(rotation_pwm, OUTPUT);

  pinMode(motor_f, OUTPUT);
  pinMode(motor_s, OUTPUT);

  pinMode(rotation_f, OUTPUT);
  pinMode(rotation_s, OUTPUT);

  Serial.begin(9600); // INFO: enable this when you wont to debug

  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0,15);     //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x65);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_HIGH); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
}


void Left(){
  digitalWrite(rotation_f, LOW);
  digitalWrite(rotation_s, HIGH);
  analogWrite(rotation_pwm, dataArray[5]);
}

void Right(){

  digitalWrite(rotation_f, HIGH);
  digitalWrite(rotation_s, LOW);
  analogWrite(rotation_pwm, dataArray[5]);
}

void Back(){
  digitalWrite(motor_f, HIGH);
  digitalWrite(motor_s, LOW);
  analogWrite(motor_pwm, dataArray[4]);
}

void Forward(){
  digitalWrite(motor_s, HIGH);
  digitalWrite(motor_f, LOW);
  analogWrite(motor_pwm, dataArray[4]);
}

void Stop_move(){
  analogWrite(motor_pwm, 0);
}

void Stop_rotation(){
  analogWrite(rotation_pwm, 0);
}





void loop() {
    byte pipeNo;
    while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
      radio.read( &dataArray, sizeof(dataArray) );         // чиатем входящий сигнал
      Serial.print(dataArray[0]);
      Serial.println(dataArray[6]);
      if (dataArray[6] == SECURITY_KEY){
        if (dataArray[0] == dataArray[1]){
          Stop_move();


        }else if (dataArray[0]){
          Forward();
        }else if (dataArray[1]){
          Back();
        }

        if (dataArray[2] == dataArray[3]){
          Stop_rotation();
        }else if (dataArray[2]){
          Left();

        }else if (dataArray[3]){
          Right();

        }
      }else{
      }
    }
}
