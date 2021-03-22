/* ESP32
    Sensor CJMCU-8128 mit:
    CCS811 -> 0x5A !!!
      bool begin()
      bool start(mode)
      read(eco2, etvoc, errstat, raw)
      int hardware_version()
      int bootloader_version()
      int application_version()
      int get_errorid()
      bool set_envdate(t,h)
      bool set_envdata210(t,h)
      bool get_baseline(baseline)
      bool set_baseline(baseline)
      bool flash(image, size)
      set_i2cdelay(us)
      get_i2cdelay(us)
      wake_init()
      wake_up()
      wake_down()
    SI7021 -> 0x40
      float readHumidity()
      float readTemperature()
      _readRevision
      readSerialNumber()
      reset()
    BMP280 -> 0x76
      bool (begin(addr)
      setSampling(mode)
      readCoefficients()
      float readTemperature()
      float readPressure()
      float readAltitude(seaLevelhPa)
      float seaLevelForAltitude
      takeForcedMeasurement()
*/

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

#include <IntechWIFI.h>

const char* ssid = INTECH_SSID;
const char* password = INTECH_PSWD;

#include <Arduino.h>
#include "ccs811.h"                     // include library for CCS811 - Sensor from martin-pennings https://github.com/maarten-pennings/CCS811
#include "Adafruit_Si7021.h"            // include main library for SI7021 - Sensor
#include "Adafruit_BMP280.h"            // include main library for BMP280 - Sensor
#include <Wire.h>                       // This library allows you to communicate with I2C

CCS811 ccs811;
Adafruit_BMP280 bmp280;                // I2C
Adafruit_Si7021 SI702x = Adafruit_Si7021();



float humidity = 0.0;
float temperature1 = 0.0;
float temperature2 = 0.0;
float pressure = 0.0;
float tvoc = 0.0;
float co2 = 0.0;
float wind;

long rssi = 0;

unsigned long lastrising;
unsigned long lastreport;

int laststate;

String ledState;
const int ledPin = LED_BUILTIN;
AsyncWebServer server(80);


String getTemperature() {

  Serial.println(temperature1);
  return String(temperature1);
}
  
String getHumidity() {

  Serial.println(humidity);
  return String(humidity);
}

String getPressure() {
  //float pressure = bme.readPressure()/ 100.0F;
  Serial.println(pressure);
  return String(pressure);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  else if (var == "TEMPERATURE"){
    //return getTemperature();
  }
  else if (var == "HUMIDITY"){
    //return getHumidity();
  }
  else if (var == "PRESSURE"){
    //return getPressure();
  }  
}

  
void setup() {

  laststate = 0;
  lastrising = 0;
  lastreport = 0;
  wind = 0;
  
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("AirQuality");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  //Serial.begin();
  // Enable I2C
  Wire.begin(D1,D2);                  // put here the Pins of I2C
  Serial.println("CCS811 test");      /* --- SETUP CCS811 on 0x5A ------ */

  pinMode(D7, INPUT);
  
  
  ccs811.set_i2cdelay(50); // Needed for ESP8266 because it doesn't handle I2C clock stretch correctly
  if (!ccs811.begin()) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    //while (true);
  }
  // Print CCS811 versions
  Serial.print("setup: hardware    version: "); Serial.println(ccs811.hardware_version(), HEX);
  Serial.print("setup: bootloader  version: "); Serial.println(ccs811.bootloader_version(), HEX);
  Serial.print("setup: application version: "); Serial.println(ccs811.application_version(), HEX);

  // Start measuring
  bool ok = ccs811.start(CCS811_MODE_1SEC);
  if ( !ok ) Serial.println("setup: CCS811 start FAILED");

  Serial.println("BMP280 test");     /* --- SETUP BMP on 0x76 ------ */
  if (!bmp280.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    //while (true);
  }

  Serial.println("Si7021 test!");     /* ---- SETUP SI702x ----- */
  if (!SI702x.begin()) {
    Serial.println("Did not find Si702x sensor!");
    //while (true);
  }
  Serial.print("Found model ");
  switch (SI702x.getModel()) {
    case SI_Engineering_Samples:
      Serial.print("SI engineering samples"); break;
    case SI_7013:
      Serial.print("Si7013"); break;
    case SI_7020:
      Serial.print("Si7020"); break;
    case SI_7021:
      Serial.print("Si7021"); break;
    case SI_UNKNOWN:
    default:
      Serial.print("Unknown");
  }
  Serial.print(" Revision(");
  Serial.print(SI702x.getRevision());
  Serial.print(")");
  Serial.print(" Serial #"); Serial.print(SI702x.sernum_a, HEX); Serial.println(SI702x.sernum_b, HEX);


  // START WESERVER
  
  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


   // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);    
    request->send_P(200, "text/plain", String("On").c_str());
  });
  
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);    
    request->send_P(200, "text/plain", String("Off").c_str());
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemperature().c_str());
  });
  
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHumidity().c_str());
  });
  
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getPressure().c_str());
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){

    String result = "";
    result += "{\"pressure\": \""+String(pressure)+"\", ";
    result += "\"temperature1\": \""+ String(temperature1) + "\", ";
    result += "\"temperature2\": \""+ String(temperature2) + "\", ";
    result += "\"humidity\": \""+ String(humidity) + "\", ";
    result += "\"co2\": \""+ String(co2) + "\", ";
    result += "\"tvoc\": \""+ String(tvoc) + "\", ";
    result += "\"wind\": \""+ String(wind) + "\", ";
    result += "\"rssi\": \""+ String(rssi) + "\" }";

//   String result = "json" + String(humidity);
    
    request->send_P(200, "text/plain", result.c_str());
  });

  
  server.begin();
}

void loop() {



 // digitalWrite(LED_BUILTIN, HIGH);
  if (millis() - lastreport > 500){

    rssi = WiFi.RSSI();
    
    ArduinoOTA.handle();

   // pinMode(LED_BUILTIN, OUTPUT);
   // digitalWrite(LED_BUILTIN, LOW);
    lastreport = millis();
  
    Serial.print("BMP280 => Temperature = ");
    Serial.print(bmp280.readTemperature());
    Serial.print(" °C, ");

    pressure = bmp280.readPressure() / 100;
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" Pa, ");

    temperature1 = SI702x.readTemperature();
    Serial.print("SI702x => Temperature = ");
    Serial.print(temperature1, 2);
    Serial.print(" °C, ");
    Serial.print("Humidity = ");

    humidity = SI702x.readHumidity();
    Serial.println(humidity, 2);

  uint16_t eco2, etvoc, errstat, raw;     // Read CCS811

  ccs811.set_envdata(bmp280.readTemperature(), SI702x.readHumidity());
  ccs811.read(&eco2, &etvoc, &errstat, &raw);
  if ( errstat == CCS811_ERRSTAT_OK ) {
    Serial.print("CCS811 => CO2 = ");
    Serial.print(eco2);
    Serial.print("ppm, TVOC = ");
    Serial.println(etvoc);

    tvoc = etvoc;
    co2 = eco2;
    
  }

  if (millis()- lastrising > 1000){
    wind = 0;
  }
  
  Serial.print("Wind RPM = ");
  
  Serial.println(wind);


 
  }

  int pinvalue = digitalRead(D7);

  if (1 == pinvalue && laststate == 0){
    //rising edge
    laststate = 1;
    unsigned long now = millis();
    wind = 1000.0/(now - lastrising);
    lastrising = now;

    //Serial.println("RISING");
      
  }else if (pinvalue == 0 && laststate == 1){
    //falling edge
    
    laststate = 0;  
    
  }
  
}
