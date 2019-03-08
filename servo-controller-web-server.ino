#include <ezTime.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

int RELAY_PIN = 5;
int RUN_DURATION = 5 * 1000; //to milliseconds

ESP8266WebServer server(80);

const char* ssid = "!!WIFI!!";
const char* password = "!!PASSWORD!!";

IPAddress staticIP(192, 168, 1, 60); //ESP static ip
IPAddress gateway(192, 168, 1, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS

Timezone manila;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  
  WiFi.config(staticIP, dns, gateway, subnet);
  
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("connecting...");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/on", HTTP_GET, turnOn);
  server.on("/off", HTTP_GET, turnOff);
  
  server.begin();

  digitalWrite(RELAY_PIN, LOW);

  delay(3500);

  Serial.println("setup finished.");

  waitForSync();

  manila.setLocation("Asia/Manila");
}

void loop() {
  //turnRelay();
  server.handleClient();

  Serial.println(manila.dateTime());
}

void turnOn(){
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("on");
  server.send(200, "text/json", "{\"status\": \"OK\", \"time\":  \"" + manila.dateTime() + "\"}");  
  delay(3500);
}

void turnOff(){
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("off");
  
  server.send(200, "text/json", "{\"status\": \"OK\", \"time\":  \"" + manila.dateTime() + "\"}");  

  delay(3500);
}

bool turnRelay(){
  delay(150);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("on");
  
  delay(RUN_DURATION);
  
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("off");
  
  delay(RUN_DURATION);
}
