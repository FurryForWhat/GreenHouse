#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int soilDigit= 4;
const int soilAna= A1;
const int Fan_One= A7;
const int Fan_Two= A6;
const int servoPin = 5 ;
const int dhtPin= 3;

const int pump_pin_one= A5;
const int pump_pin_two= A4;

float glo_temp;
float glo_humi;
float glo_soil;
byte glo_fan;
byte glo_pump_one;
byte glo_pump_two;

Servo servo;
DHT dht(dhtPin, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  //LCD
  lcd.backlight();
  lcd.init();
  //Soil
  pinMode(soilDigit,OUTPUT);
  digitalWrite(soilDigit,LOW);

  //SERVO
  servo.attach(servoPin);
  //Fan
  pinMode(Fan_One, OUTPUT);
  pinMode(Fan_Two, OUTPUT);

  //dht
  dht.begin();
  //pump
  pinMode(pump_pin_one,OUTPUT);
  pinMode(pump_pin_two,OUTPUT);
  
}

// void ServoOperate(){
//   for (int angle= 0; angle <=180 ; angle+= 30){
//       servo.write(angle);
//       Serial.print("Servo-Degree: ");
//       Serial.println(angle);
//       delay (500);
//     }
//   for (int angle = 150; angle >= 0; angle -=30){
//       servo.write(angle);
//       Serial.print("Servo-Degree: ");
//       Serial.println(angle);
//       delay (500);      
//     }
// }

void firstProcess(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Humidity:");
   lcd.print(h);
   lcd.print("%");
   lcd.setCursor(0,1);
   lcd.print("Temp: ");
   lcd.print(t);
   lcd.print("F");

   glo_temp= t;
   glo_humi= h;

  if ( t > 25){
    glo_fan= HIGH;
    digitalWrite(Fan_One, LOW);
    digitalWrite(Fan_Two, LOW); //temp high, fan starting
  }else{
    glo_fan= LOW;
    digitalWrite(Fan_One, HIGH);
    digitalWrite(Fan_Two, HIGH); // temp low, fan off
  }
  if ( h < 45){
    glo_pump_two= HIGH;
    digitalWrite(pump_pin_two,LOW);
  }else{
    glo_pump_two= LOW;
    digitalWrite(pump_pin_two,HIGH);
  }
}

void secondProcess(){
  digitalWrite(soilDigit,HIGH);
  int val= analogRead(soilAna);
  digitalWrite(soilDigit,LOW);
  glo_soil= val; 
  if( val < 500){
    glo_pump_one= LOW;
    digitalWrite(pump_pin_one, HIGH);
  }else if( val >= 500 && val <= 750){
    glo_pump_one= LOW;
    digitalWrite(pump_pin_one, HIGH);
  }else if( val > 750){
    glo_pump_one= HIGH;
    digitalWrite(pump_pin_one, LOW);
  }
}

void sendSensor(){
  Serial.print("<T:"); //Temperature
  Serial.print(glo_temp);
  Serial.print(",H:");// Humidity
  Serial.print(glo_humi);
  Serial.print(",S:");// Soil Moisture
  Serial.print(glo_soil);
  Serial.print(",F:"); // Fan State
  Serial.print(glo_fan ? "ON" : "OFF");
  Serial.print(",P1:"); // Pump State
  Serial.print(glo_pump_one ? "ON" : "OFF");
  Serial.print(",P2:");
  Serial.print(glo_pump_two ? "ON," : "OFF,");
  Serial.print('>');
  Serial.print('\n');
}

void loop() {
  // ServoOperate();
  firstProcess();
  secondProcess();
  sendSensor();
  delay(1000);
}