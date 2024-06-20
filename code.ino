#define BLYNK_TEMPLATE_ID "TMPL3pGwPNuSy"
#define BLYNK_TEMPLATE_NAME "Baby Monitoring System"
#define BLYNK_AUTH_TOKEN "riOEsUCgiXWFfUQzQ7Y5Te970p3qa1Tc"

#define BLYNK_PRINT Serial
#include <DHT.h>
//#include <BlynkSimpleWifi.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;
char ssid[] = "SSN";
char pass[] = "Ssn1!Som2@Sase3#";

#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int soundPin = A0; // Analog pin for sound sensor
const float calibrationFactor = 3.5; // Calibration factor (adjust as per your calibration)
int KY033_PIN = D6; // Pin where the KY-033 sensor signal output is connected
int BUZZ = D4;
int previousSensorValue = LOW; // Variable to store the previous sensor reading

void sendsensor(){
  int soundValue = analogRead(soundPin); // Read the sound sensor value
  float voltage = soundValue * (3.3 / 1023); // Convert analog reading to voltage
  float dB = voltage * calibrationFactor; // Convert voltage to dB using calibration factor
  
  Serial.print("Analog Value: ");
  Serial.print(soundValue);
  Serial.print(", Voltage: ");
  Serial.print(voltage);
  Serial.print("V, dB: ");
  Serial.println(dB); // Print the dB value
  if (soundValue>960) {
    //Blynk.email("divyadharshinir257@gmail.com","subject","Sound level exceeded threshold!");
    Blynk.logEvent("Noise","Noise detected!!");
    delay(1000); // To prevent continuous notifications
  }

  
  delay(1000); // Delay for stability
}

void sensor1() {
  // Read the state of the KY-033 sensor
  int sensorValue = digitalRead(KY033_PIN);

  // Check for movement by comparing current and previous sensor readings
  if (sensorValue != previousSensorValue) {
    // Movement detected
    Serial.println("Movement Detected");
    Blynk.logEvent("Movement","Movement detected!!");
    tone(BUZZ,2000);
    // Update previous sensor value
    previousSensorValue = sensorValue;
  }
  // Add a short delay before the next reading
  delay(1000);
}

void temp(){
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.println("Temperature: ");
  Serial.println(temperature);
  Serial.println(" °C, Humidity: ");
  Serial.println(humidity);
  Serial.println(" %");
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V0, temperature);
  delay(1000); // Update every 2 seconds
}
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(soundPin, INPUT); // Set sound sensor pin as input
  pinMode(KY033_PIN, INPUT);  // Set the KY-033 sensor pin as input
  pinMode(BUZZ, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L,sendsensor);
  timer.setInterval(1000L,sensor1);
  timer.setInterval(1000L,temp);
}

void loop() {
  Blynk.run();
  timer.run();
}
