#include <WiFi.h>
#include <esp_wifi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Set mode to AP + Station to wake up both radios
  WiFi.mode(WIFI_AP_STA);
  
  Serial.println("\n--- ESP32 MAC IDENTIFICATION ---");
  
  // Station MAC (Used for ESP-NOW)
  Serial.print("WiFi STA MAC: ");
  Serial.println(WiFi.macAddress());
  
  // Access Point MAC
  Serial.print("WiFi AP MAC:  ");
  Serial.println(WiFi.softAPmacAddress());
  
  Serial.println("---------------------------------");
}

void loop() {
  // Nothing here
}
