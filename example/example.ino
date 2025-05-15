#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#define DHTPIN 13 // Define which pin the DHT22 is connected to
#define DHTTYPE DHT22
// WiFi settings
const char* ssid = "IoTstudio_148";
const char* password = "IoT@admin";
// Google Script Deployment URL
String GAS_URL = "https://script.google.com/macros/s/AKfycbwtkWSyvFPRu9tlETPwi-h9SrV-L5GRTCpzqk2YQE-lIiji-nlE5e8apQgX8D4xmXBREw/exec?id=Sensor_Data&";

DHT dht(DHTPIN, DHTTYPE);
void setup() {
Serial.begin(115200);
dht.begin();
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println("Connecting to WiFi...");
}
Serial.println("Connected to WiFi");
}
void loop() {
float humidity = dht.readHumidity();
float temperature = dht.readTemperature();
if (isnan(humidity) || isnan(temperature)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
sendDataToGoogleSheet(temperature, humidity);
delay(100); // Send data every 1 minute
}
void sendDataToGoogleSheet(float temperature, float humidity) {
if(WiFi.status()== WL_CONNECTED){
HTTPClient http;
String url = GAS_URL + "Temperature=" + String(temperature) + "&Humidity=" + String(humidity);
http.begin(url);
int httpResponseCode = http.GET();
if(httpResponseCode>0){
String response = http.getString();
Serial.println(response);
} else {
Serial.print("Error on sending POST: ");
Serial.println(httpResponseCode);
}
http.end();
} else {
Serial.println("Error in WiFi connection");
}
}