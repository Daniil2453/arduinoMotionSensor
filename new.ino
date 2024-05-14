// Include the necessary libraries
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <./cfg/config.h>  
#define WIFI_SSID config.ssid
#define WIFI_PASSWORD config.password
#define BOT_TOKEN config.tlgrmToken
#define CHAT_ID config.tlgrmChatId
#define SERVER_URL config.API

WiFiClientSecure client;

UniversalTelegramBot bot(BOT_TOKEN, client);

const int ledPin = 4;

const int pirPin = 23;
bool motionDetected = false;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  pinMode(pirPin, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  client.setInsecure();
}

void loop() {
  motionDetected = digitalRead(pirPin);

  if (motionDetected) {
    digitalWrite(ledPin, HIGH); 
    Serial.println("Alert: Motion detected!");
    bot.sendMessage(CHAT_ID, "Alert: Motion detected!", "");

    
    HTTPClient http;
    // TODO PREPARE DATA FOR SERVER
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");
    String json = "{\"request\": \"motionSen\"}";
    int httpCode = http.POST(json);  
    if (httpCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error sending data to server: " + String(httpCode));
    }
    http.end();

    delay(1000); 
  } else {
    digitalWrite(ledPin, LOW); 
  }

 
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi not connected");
  }

  delay(350);
}
