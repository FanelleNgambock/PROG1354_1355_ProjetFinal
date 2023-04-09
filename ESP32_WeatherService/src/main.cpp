/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

using namespace std;

// Import required libraries
#include "WiFi.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <AsyncElegantOTA.h>

#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

float tempValue = 0;
float PressValue = 0;
float altValue = 0;

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

// Replace with your network credentials

const char* ssid = "NPF";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String getResponse();


void setup(){

  Serial.begin(9600);
  Serial.println(F("BMP280 Forced Mode Test."));

  //if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */



  // Serial port for debugging purposes
  
  Serial.begin(9600);
pinMode(LED_BUILTIN, OUTPUT);
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


  AsyncElegantOTA.begin(&server);    // Start ElegantOTA

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);

}


String getResponse() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // must call this to wake sensor up and get new measurement data
  // it blocks until measurement is complete
  if (bmp.takeForcedMeasurement()) {
    // can now print out the new measurements

    tempValue = bmp.readTemperature();
    PressValue = bmp.readPressure();
    altValue = bmp.readAltitude(1013.25);
    Serial.println();
  } else {
    Serial.println("Forced measurement failed!");
  }

  String tmp = String(tempValue, 2);

  String pression = String(PressValue, 2);
  String alt = String(altValue, 2);

  String response = "{\"Temperature\":";
  response += tmp;
  response += ",\"Altitude\":";
  response += alt;
  response += ",\"Pression\":";
  response += pression;
  response += "}";
  return response;
}