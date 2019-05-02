
#include <Servo.h>


// change values in this block to your values)
int servo_pin = 0;
int left_motor_direct = 1;
int left_motor_reverse = 2;
int right_motor_direct = 3;
int right_motor_reverse = 4;
Servo steering;

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10); // "создать" модуль на пинах 9 и 10 Для nano
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб

void setup(){
  Serial.begin(9600); //открываем порт для связи с ПК
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0,15);     //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
  pinMode(LED_BUILTIN, OUTPUT);
  steering.attach(servo_pin);
}

byte control[4];
/* Control has 4 item,
0 is left motor power
1 is right motor power
2 is steering position
3 is reverse flag
*/


void loop() {
  // recive data to variable control coming soon....
  byte pipeNo;
  while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
    radio.read( &control, sizeof(control) );         // чиатем входящий сигнал

    Serial.print("Recieved: "); Serial.println(control);
 }
  // steering.move(control[2]); // MODIFY this correctly
  // if(control[3] == 0){
  //   analogWrite(right_motor_direct, control[0]);
  //   analogWrite(left_motor_direct, control[1]);
  //
  // }else{
  //   analogWrite(right_motor_reverse, control[0]);
  //   analogWrite(left_motor_reverse, control[1]);

  }
