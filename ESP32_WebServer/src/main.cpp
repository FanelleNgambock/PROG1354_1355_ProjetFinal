/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// Replace with your network credentials

const char* ssid = "NPF";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String getResponse() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  String tmp = "20";
  String hum = "50";
  String pression = "100";

  String response = "{\"temperature\":";
  response += tmp;
  response += ",\"Humidite\":";
  response += hum;
  response += ",\"Pression\":";
  response += pression;
  response += "}";
  return response;
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());


  // Route for root / web page
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getResponse().c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
  
}