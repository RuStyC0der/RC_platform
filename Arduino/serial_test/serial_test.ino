#define PARSE_AMOUNT 5       // число значений в массиве, который хотим получить
#define INPUT_AMOUNT 80      // максимальное количество символов в пакете, который идёт в сериал
char inputData[INPUT_AMOUNT];  // массив входных значений (СИМВОЛЫ)
int intData[PARSE_AMOUNT];     // массив численных значений после парсинга
boolean recievedFlag;
boolean getStarted;
byte index;
String string_convert;
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
Serial.begin(9600);
}
void loop() {
parsing();       // функция парсинга
if (recievedFlag) {                           // если получены данные
recievedFlag = false;
for (byte i = 0; i < PARSE_AMOUNT; i++) { // выводим элементы массива
Serial.print(intData[i]); Serial.print(" ");
}
Serial.println();
}
}
