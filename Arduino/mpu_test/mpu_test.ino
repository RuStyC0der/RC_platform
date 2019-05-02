#include "I2Cdev.h"
#include "MPU6050.h"

#define TO_DEG 57.29577951308232087679815481410517033f
#define T_OUT 20

MPU6050 accel;

void setup() {
    Serial.begin(9600);

    accel.initialize(); // первичная настройка датчика
    Serial.println(accel.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

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


/* Control has 6 item,
0 is forward
1 is back
2 is left flag
3 is right flag
4 is motor power
5 is rotation sensivity
*/

// ax_raw is a speed
// ay_raw is a rotation

int ignore_threshold = 2300;

byte data[6] = {0,0,0,0,0,0};
float ax, ay;

void loop() {

      int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
      float ay,gx;

      accel.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);

      byte data[6] = {0,0,0,0,0,0};

      ax = filter_a(constrain(ax_raw, -16000, 16000));
      ay = filter_b(constrain(ay_raw, -16000, 16000));
      if (abs(ax) > ignore_threshold){
        if (ax < 0){
          data[2] = 1;
        }
        else{
          data[3] = 1;
        }
        data[4] = map(abs(ax), 0,16000,0,255);
      }



      if (abs(ay_raw) > ignore_threshold){
        if (ay < 0){
          data[0] = 1;
        }
        else{
          data[1] = 1;
        }
        data[5] = map(abs(ay), 0,16000,0,255);
      }

      Serial.print(data[0]);
      Serial.print(" ");
      Serial.print(data[1]);
      Serial.print(" ");
      Serial.print(data[2]);
      Serial.print(" ");
      Serial.print(data[3]);
      Serial.print(" ");
      Serial.print(data[4]);
      Serial.print(" ");
      Serial.println(data[5]);
      delay(100);
}
