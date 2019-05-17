#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
#include "I2Cdev.h"
#include "MPU6050.h"




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

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги
MPU6050 accel;

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб


void setup() {
  Serial.begin(9600); //открываем порт для связи с ПК

  accel.initialize(); // первичная настройка датчика
  Serial.println(accel.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_HIGH); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик


  accel.setXAccelOffset(1662);
  accel.setYAccelOffset(-247);
  accel.setZAccelOffset(1292);
  accel.setXGyroOffset(27);
  accel.setYGyroOffset(-18);
  accel.setZGyroOffset(8);

}



// переменные для Калмана
float varVolt_a = 78.9;   // среднее отклонение (ищем в excel)
float varProcess_a = 0.5; // скорость реакции на изменение (подбирается вручную)
float Pc_a = 0.0, G_a = 0.0, P_a = 1.0, Xp_a = 0.0, Zp_a = 0.0, Xe_a = 0.0;
// переменные для Калмана

// Функция фильтрации
float filter_a(float val) {
  Pc_a = P_a + varProcess_a;
  G_a = Pc_a / (Pc_a + varVolt_a);
  P_a = (1 - G_a) * Pc_a;
  Xp_a = Xe_a;
  Zp_a = Xp_a;
  Xe_a = G_a * (val - Zp_a) + Xp_a; // "фильтрованное" значение
  return (Xe_a);
}

// переменные для Калмана
float varVolt_b = 78.9;   // среднее отклонение (ищем в excel)
float varProcess_b = 5; // скорость реакции на изменение (подбирается вручную)
float Pc_b = 0.0, G_b = 0.0, P_b = 1.0, Xp_b= 0.0, Zp_b = 0.0, Xe_b = 0.0;
// переменные для Калмана

// Функция фильтрации
float filter_b(float val) {
  Pc_b = P_b + varProcess_b;
  G_b = Pc_b / (Pc_b + varVolt_b);
  P_b = (1 - G_b) * Pc_b;
  Xp_b = Xe_b;
  Zp_b = Xp_b;
  Xe_b = G_b * (val - Zp_b) + Xp_b; // "фильтрованное" значение
  return (Xe_b);
}


int ignore_threshold = 2300;
int rot_threshold = 4300;
float ax, ay;

// byte data[7] = {0,0,0,0,0,0,security_key};
byte data[7] = {0,0,0,0,0,0,SECURITY_KEY};


void loop() {
  int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
  float ay,gx;

  accel.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);


  byte data[6] = {0,0,0,0,0,0};

  ax = filter_a(constrain(ax_raw, -16000, 16000));
  ay = filter_b(constrain(ay_raw, -16000, 16000));
  if (abs(ay) > rot_threshold){
    if (ay < 0){
      data[2] = 1;
    }
    else{
      data[3] = 1;
    }
    // data[5] = map(abs(ay), 0,14000,200,255); //for good motor
    data[5] = 255; // for my bad motor
  }



  if (abs(ax_raw) > ignore_threshold){
    if (ax < 0){
      data[0] = 1;
    }
    else{
      data[1] = 1;
    }
    data[4] = map(abs(ax), 0,16000,40,255);
  }

    radio.write(&data, sizeof(data));
    Serial.println(data[4]);
    memset(data, 0, 6);

  }
