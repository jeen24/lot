/*



*/
//for Servo
#include <Servo.h> 
int servoPin = 9;
Servo servo;

//for DHT
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

//for Involt
int    involtPin[14] = {}; //equals involt.pin.P in app
String involtString[2] = {}; //equals involt.pin.S in app
char involt[16];
String fname;

//for DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //Connection speed must be same as app.
  Serial.begin(57600);
  
  dht.begin();

  servo.attach(servoPin); 
}

void loop() {
  involtReceive();

  //Work

  servo.write(involtPin[9]);

  //Send

  readDHT11();

  //Clear the function to trigger it only once.
  fname = "";
  delay(2000);

}

void involtReceive() {
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', involt, sizeof(involt));
    int pin;
    if (involt[0] == 'P') {
      int value;
      sscanf(involt, "P%dV%d", &pin, &value);
      involtPin[pin] = value;
    }
    else if (involt[0] == 'S') {
      char value[sizeof(involt)];
      sscanf(involt, "S%dV%s", &pin, &value);
      involtString[pin] = value;
    }
    else if (involt[0] == 'F') {
      char value[sizeof(involt)];
      sscanf(involt, "F%s", &value);
      fname = value;
    };
    memset(involt, 0, sizeof(involt));
  };
};

void involtSend(int pinNumber, int sendValue) {
  Serial.print('A');
  Serial.print(pinNumber);
  Serial.print('V');
  Serial.print(sendValue);
  Serial.println('E');
  Serial.flush();
};

void involtSendString(int pinNumber, String sendString) {
  Serial.print('A');
  Serial.print(pinNumber);
  Serial.print('V');
  Serial.print(sendString);
  Serial.println('E');
  Serial.flush();
};

void involtSendFunction(String functionName) {
  Serial.print('F');
  Serial.print(functionName);
  Serial.println('E');
  Serial.flush();
};

//Module
float readDHT11(void) {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 0;
  } else {
    involtSend(2, t);
    involtSend(3, h);
    return 1;
  }
}
