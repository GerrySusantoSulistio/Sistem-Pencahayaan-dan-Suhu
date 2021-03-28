#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>

//koneksi wifi dan blynk
char auth[] = "57gY-Q0laC-GE9a2N1u-nVn_ZxhFvVQa"; //token blynk
char ssid[] = "AndroidMe"; //nama wifi
char pass[] = "taeyeon89"; //password wifi


const int pinSensorCahaya = 21;
const int pinSensorSuhu = 5;
int pinCahaya = 22;
int pinSuhu = 23;
int sensorCahaya = 0;
int lux = 0;
float sensorSuhu = 0.0;
float celcius = 0;
int THCahaya = 0;
int THSuhu = 0;
int stateButton = 0;
DHT dht11(pinSensorSuhu, DHT11);

BLYNK_WRITE(V4) {
  THCahaya = param.asInt() * 0.009768 + 10;
}
BLYNK_WRITE(V5) {
  THSuhu = param.asInt();
}
WidgetLED ledKecerahan(V6);
WidgetLED ledSuhu(V7);
BLYNK_WRITE(V8) {
  stateButton = param.asInt();
}

void setup() {
  Serial.begin(9600);
  
  // pinMode
  pinMode(pinSensorSuhu, INPUT);
  pinMode(pinCahaya, OUTPUT);
  pinMode(pinSuhu, OUTPUT);

  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 43, 138), 8080);
  dht11.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  if(stateButton){
    sensorCahaya = analogRead(pinSensorCahaya);
    sensorSuhu = dht11.readTemperature();
    lux = 0.009768 * sensorCahaya + 10;
    if (!isnan (sensorSuhu) ) {
      celcius = sensorSuhu;
    }
    Blynk.virtualWrite(V0, lux);
    Blynk.virtualWrite (V1, celcius);
    Blynk.virtualWrite(V2, lux);
    Blynk.virtualWrite(V3, celcius);
    if (lux > THCahaya) {
      ledKecerahan.on();
      digitalWrite(pinCahaya, LOW);
    } else {
      ledKecerahan.off();
      digitalWrite(pinCahaya, HIGH);
    }
    if (celcius > THSuhu) {
      ledSuhu.on();
      digitalWrite(pinSuhu, LOW);
    } else {
      ledSuhu.off();
      digitalWrite(pinSuhu, HIGH);
    }
    Serial.print("Kecerahan: ");
    Serial.print(lux);
    Serial.print(" Suhu: ");
    Serial.print(celcius);
    Serial.print(" THCahaya: ");
    Serial.print(THCahaya);
    Serial.print(" THSuhu: ");
    Serial.println(THSuhu);
  } else {
    digitalWrite(pinCahaya, HIGH);
    digitalWrite(pinSuhu, HIGH);
  }
  delay(200);
}
