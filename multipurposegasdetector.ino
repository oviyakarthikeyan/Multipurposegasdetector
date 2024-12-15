#define BLYNK_TEMPLATE_ID "TMPL3ikUlUI7v"
#define BLYNK_TEMPLATE_NAME "Multipurpose Gas detector"
#define BLYNK_AUTH_TOKEN "pU61D714sITnYJMCh-Beo9LeLfczECHl"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN; // Blynk Auth Token

char ssid[] = "K2500"; // WiFi SSID
char pass[] = "127.0.001"; // WiFi Password

#define MQ2_PIN D0 // Analog pin for MQ2
#define MQ7_PIN D1 // Analog pin for MQ7
#define MQ135_PIN D2 // Analog pin for MQ135

BlynkTimer timer;

void sendSensorData()
{
  float mq2Value = analogRead(MQ2_PIN);
  float mq7Value = analogRead(MQ7_PIN);
  float mq135Value = analogRead(MQ135_PIN);

  // Convert analog readings to PPM values
  float mq2PPM = mq2Value / 1023.0 * 1000; // MQ2 output in PPM
  float mq7PPM = mq7Value / 1023.0 * 1000; // MQ7 output in PPM
  float mq135PPM = mq135Value / 1023.0 * 1000; // MQ135 output in PPM

  // Send sensor data to Blynk
  Blynk.virtualWrite(V0, mq2PPM); // Send MQ2 data to V0
  Blynk.virtualWrite(V1, mq7PPM); // Send MQ7 data to V1
  Blynk.virtualWrite(V2, mq135PPM); // Send MQ135 data to V2
  
  // Check for gas alerts
  checkMQ2Alert(mq2PPM);
  checkMQ7Alert(mq7PPM);
  checkMQ135Alert(mq135PPM);
}

void checkMQ2Alert(float mq2PPM)
{
  // Define threshold for MQ2 sensor
  float mq2Threshold = 200; // Define your threshold here
  
  // Send alert to Blynk if MQ2 reading exceeds threshold
  if (mq2PPM > mq2Threshold) {
    Blynk.logEvent("MQ2_Alert", "High concentration detected at mq2");
  }
}

void checkMQ7Alert(float mq7PPM)
{
  // Define threshold for MQ7 sensor
  float mq7Threshold = 100; // Define your threshold here
  
  // Send alert to Blynk if MQ7 reading exceeds threshold
  if (mq7PPM > mq7Threshold) {
    Blynk.logEvent("MQ7_Alert", "High concentration detected at mq7"); 
  }
}

void checkMQ135Alert(float mq135PPM)
{
  // Define threshold for MQ135 sensor
  float mq135Threshold = 200; // Define your threshold here
  // Send alert to Blynk if MQ135 reading exceeds threshold
  if (mq135PPM > mq135Threshold) {
    Blynk.logEvent("MQ135_Alert", "High concentration detected at mq135");
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
 
  // Set up timer to send sensor data to Blynk every 1 second
  timer.setInterval(1000L, sendSensorData);
}

void loop()
{
  Blynk.run();
  timer.run();
}
