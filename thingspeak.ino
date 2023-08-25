#include <WiFiNINA.h>
#include "ThingSpeak.h"
#include <DHT.h>

#define DHTPIN 4      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT sensor type we are using
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "MyWifiCar";     // Replace with your WiFi SSID
char pass[] = "jhanvi12345"; // Replace with your WiFi password

unsigned long myChannelNumber = 2241075;  // Replace with your ThingSpeak channel number
const char *myWriteAPIKey = "OAM4XQEGHLRWYIE7"; // Replace with your ThingSpeak write API key

WiFiClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // Wait for Serial monitor to open
  }

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
 

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  
  ThingSpeak.setField(2, t); // Field 2 for temperature
  ThingSpeak.setField(1, h); // Field 1 for humidity

  int updateStatus = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (updateStatus == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(updateStatus);
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C\t");

  delay(2000); // Adjust delay as needed
}