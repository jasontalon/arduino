#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

int RED_LED = 5;
int YELLOW_LED = 16;

ESP8266WebServer server(80);

const char* ssid = "!!WIFI!!";
const char* password = "!!PASSWORD!!";

IPAddress staticIP(192, 168, 1, 60); //ESP static ip
IPAddress gateway(192, 168, 1, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(8, 8, 8, 8);  //DNS

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  
  Serial.println("setting up static IP...");
  
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

  server.on("/", HTTP_GET, handleRoot);
  server.on("/led", HTTP_POST, handleLedRequest);
  
  server.begin();
  Serial.println("HTTP server started");
  int x = 0; 
  while(x < 3){
    blinkServerStarted();
     x++;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
server.handleClient();
}

void handleLedRequest(){
  long color = stringToLong(server.arg("color"));
  long duration = stringToLong(server.arg("duration"));

  if(color > 0){
    turnOnLed(color, duration);
  }
  else{
    turnOnLed(RED_LED, duration);
    turnOnLed(YELLOW_LED, duration);
  }
  
  delay(duration * 1000);
  turnOffLed(RED_LED);
  turnOffLed(YELLOW_LED);
  
  server.send(200, "text/json", "{\"status\": \"OK\"}");
}

void blinkServerStarted(){
  Serial.println("blinkServerStarted");
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  delay(250);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  delay(250);
}

void turnOnLed(int led, long duration){
  digitalWrite(led, HIGH);
}

void turnOffLed(int led){
  digitalWrite(led, LOW);
}

void handleRoot() {
  server.send(200, "text/html", "<html>\r\n    <form action=\"/led\" method=\"POST\">\r\n        <div>\r\n            <label>Color</label>\r\n            <select name=\"color\">\r\n                <option value=\"5\">Red</option>\r\n                <option value=\"16\">Yellow</option>\r\n                <option value=\"-1\">Red & Yellow</option>\r\n            </select>\r\n    </div>\r\n    <div>\r\n        <label>Duation (seconds)</label>\r\n        <select name=\"duration\">\r\n            <option value=\"5\">5</option>\r\n            <option value=\"10\">10</option>\r\n            <option value=\"15\">15</option>\r\n            <option value=\"20\">20</option>\r\n        </select>\r\n    </div>\r\n    <div>\r\n        <button type=\"submit\">Go</button>\r\n    </div>\r\n    </form>\r\n</html>");
}

long stringToLong(String s)
{
   char arr[12];
   s.toCharArray(arr, sizeof(arr));
   return atol(arr);
}
