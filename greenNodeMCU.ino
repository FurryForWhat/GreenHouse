#define BLYNK_TEMPLATE_ID "TMPL62MALuw9y"
#define BLYNK_TEMPLATE_NAME "GreenHouse101"
#define BLYNK_AUTH_TOKEN "umDLnD7tn85iG1A6FQYjn4zGO5gyqCXi"

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

char auth[] = BLYNK_AUTH_TOKEN;
const char ssid[] = "NCC_InstituteOfScience"; 
const char password[] = "CrazySci3ntist"; 

BlynkTimer timer;

void receiveAndSplitData(){
  if (Serial.available() > 0) {
  String receiveData = Serial.readStringUntil('\n');
  int startMarker = receiveData.indexOf('<');
  int endMarker = receiveData.indexOf('>');
  if (startMarker != -1 && endMarker != -1) {
    String data = receiveData.substring(startMarker + 1, endMarker);
    // Serial.println(data);
    int Temp_colon1= data.indexOf(':');
    int Temp_comma1= data.indexOf(',',Temp_colon1);
    int Humid_colon2= data.indexOf(':',Temp_comma1);
    int Humid_comma2= data.indexOf(',',Humid_colon2);
    int Soil_colon3= data.indexOf(':',Humid_comma2);
    int Soil_comma3= data.indexOf(',',Soil_colon3);
    int Fan_colon4= data.indexOf(':',Soil_comma3);
    int Fan_comma4= data.indexOf(',',Fan_colon4);
    int Pump_colon5= data.indexOf(':',Fan_comma4);
    int Pump_comma5= data.indexOf(',',Pump_colon5);
    int Pump_colon6= data.indexOf(':',Pump_comma5);
    int Pump_comma6= data.indexOf(',',Pump_colon6);

    if( Temp_colon1 != -1 || Temp_comma1 != -1 || Humid_colon2 != -1 || Humid_comma2 != -1 || Soil_colon3 != -1 || Soil_comma3 != -1 || Fan_colon4 != -1 || Fan_comma4 != -1 || Pump_colon5 != -1 || Pump_comma5 != -1){
      String temp= data.substring(Temp_colon1+1, Temp_comma1);
      String humidity= data.substring(Humid_colon2+1, Humid_comma2);
      String Soil= data.substring(Soil_colon3+1, Soil_comma3);
      String fan= data.substring(Fan_colon4+1, Fan_comma4);
      String pump_one= data.substring(Pump_colon5+1, Pump_comma5);
      String pump_two= data.substring(Pump_colon6+1, Pump_comma6);

      float blynk_temp= temp.toFloat();
      float blynk_humidity= humidity.toFloat();
      float blynk_soil= Soil.toFloat();
      
      Blynk.virtualWrite(V0, blynk_temp);
      Blynk.virtualWrite(V1, blynk_humidity);
      Blynk.virtualWrite(V2, blynk_soil);
      Blynk.virtualWrite(V3, fan);
      Blynk.virtualWrite(V4, pump_one);
      Blynk.virtualWrite(V5, pump_two);



      Serial.print(" Temp : ");
      Serial.print(blynk_temp);
      
      Serial.print(" Humidity : ");
      Serial.print(humidity);

      Serial.print(" Moisture : ");
      Serial.print(Soil);

      Serial.print(" Fan : ");
      Serial.print(fan);

      Serial.print(" Pump(1) : ");
      Serial.print(pump_one);

      Serial.print(" Pump(2): ");
      Serial.print(pump_two);
      Serial.println("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    }
  }else{
    Serial.println("serial data sent by Arduino are unavaliable!!!!!");
  }
  }
}

void setup() {
  Serial.setRxBufferSize(256); 
  Serial.begin(9600);
  Blynk.begin(auth, ssid, password);
  timer.setInterval(100L, receiveAndSplitData);
}

void loop() {
  Blynk.run();
  timer.run();
  delay(1000);
}
