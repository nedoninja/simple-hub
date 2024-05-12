#include <Arduino.h>
#include "WiFi.h"
#include <WebServer.h>


const char* ssid = "SSID";
const char* password = "Pass";


WiFiServer server(80);
String header;


const int relePin = 26;


uint32_t timer = 0;
#define T_PERIOD 165000


void setup() {  
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW);

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  server.begin();
}


void loop() {
  WiFiClient client = server.available(); 
  if (client) { 
    Serial.println("New Client."); 
    String currentLine = "";
    while (client.connected()) { 
      if (client.available()) { 
        char c = client.read(); 
        Serial.write(c); 
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println();
            if (header.indexOf("GET /sock1/start") >= 0) {
              client.stop();
              Serial.println("start");
              digitalWrite(relePin, HIGH);
              delay(T_PERIOD);
              Serial.println("stop");
              digitalWrite(relePin, LOW);
            } 
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c; 
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}