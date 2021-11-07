#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL079VvQfH"
#define BLYNK_DEVICE_NAME "AutoDoor"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#define sensor 10
#define led 8
#define led_control 11
char auth[] = "p8KQLSu6MOswBoi0Dw2HBTHVhhPrMVhV";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Phuong";
char pass[] = "12345678";
int temp_manual = 0;
WidgetLED ledStatus(V0);
WidgetLED ledDoor(V2);


BlynkTimer timer;

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(pinValue);
  Blynk.virtualWrite(V2, pinValue);
  if (pinValue==1) {
    ledDoor.on();
    temp_manual = 1;
  } else if(pinValue ==0) {
    ledDoor.off();
    temp_manual = 0;
  }
}

BLYNK_WRITE(V6)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(pinValue);
  if (pinValue==1) {
    temp_manual = 2;
  } else if(pinValue ==0) {
    temp_manual = 0;
  }
}

//V1 LED Widget is blinking
void blinkLedWidget()
{
  if (ledStatus.getValue()) {
    ledStatus.off();
  } else {
    ledStatus.on();
  }
}
void setup()
{
  Serial.begin(9600);
  Wire.begin(D1, D2);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, blinkLedWidget);
  digitalWrite(led,LOW);
  digitalWrite(sensor,LOW);

}

void loop()
{
  String t;
  Blynk.run();
  timer.run();
  
  Wire.beginTransmission(8);//
  Wire.write(temp_manual);          // gửi sang arduino 
  Wire.endTransmission();   // 
  
  Wire.requestFrom(8, 13);
  if(Wire.available()){
    int c = Wire.read();
    Serial.print(c);
    if(c==0){
      t = "THẤP";    
      Blynk.virtualWrite(V5,t);
    }else if(c==1){
      t = "CAO";
      Blynk.virtualWrite(V5,t);
      delay(1000);
    }
  }
  Serial.println();
  delay(1000);
}
