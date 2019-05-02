
int led1 = 2;
int led2 = 3;
int led3 = 4;


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  if (Serial.available() > 0) {  //если есть доступные данные
		// считываем байт
		incomingByte = Serial.read();

		// отсылаем то, что получили
		Serial.print(incomingByte);
	}
  // put your main code here, to run repeatedly:
}
