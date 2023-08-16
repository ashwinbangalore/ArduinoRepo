#define BLYNK_TEMPLATE_ID "BlynkTemplateID-UseYourOwn"
#define BLYNK_TEMPLATE_NAME "BlynkTemplateName-UseYourOwn"
#define BLYNK_AUTH_TOKEN "AuthToken-UseYourOwn"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// Define the virtual pins here.
#define VPIN_TEMP_C     V0
#define VPIN_TEMP_F     V1
#define VPIN_HUMIDITY   V2
#define VPIN_PRESSURE   V3
#define VPIN_ALTIMETER  V4
#define OTA_HOSTNAME "Backyard Weather"
#define HEADER_SEPARATOR "              "
BlynkTimer timer;

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;

char ssid[] = "WIFI-SSID-UseYourOwn";  // Enter SSID here
char password[] = "WIFIPass-UseYourOwn";  //Enter Password here
 
void setup()
{
  Serial.begin(115200);
  delay(100);
  
  bme.begin(0x76);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED)
  {
     delay(1000);
     Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  timer.setInterval(30000L, sendSensor);
}

void loop() 
{
  Blynk.run(); // Running the blynk code
  timer.run();
}

void sendSensor()
{
  temperature = bme.readTemperature();  //temp is read in Celsius
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F; //pressure is read as hPa
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  int inHG = pressure * 2.952998057228486F;
  int altInFt = altitude * 3.28084F;
  float tempInF = (temperature * 9 / 5) + 32.0;

  Serial.println(HEADER_SEPARATOR);
  Serial.println(temperature);
  Serial.println(tempInF);
  Serial.println(humidity);
  Serial.println(pressure);
  Serial.println(inHG);
  Serial.println(altInFt);
  Serial.println(HEADER_SEPARATOR);


//  if (isnan(humidity) || isnan(temperature))
//  {
//    Serial.println("Failed to read from BME280 sensor!");
//    return;
//  }

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(VPIN_TEMP_C, temperature);
  Blynk.virtualWrite(VPIN_TEMP_F, tempInF);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity);
  Blynk.virtualWrite(VPIN_PRESSURE, inHG);
  Blynk.virtualWrite(VPIN_ALTIMETER, altInFt);
}
