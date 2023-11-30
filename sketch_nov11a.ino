#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const int gpsRxPin = 4; // GPS TX to Arduino RX
const int gpsTxPin = 2; // GPS RX to Arduino TX
const int gpsPpsPin = 5; // Pulse Per Second pin
const int gpsEnPin = 6; // Enable pin
const int ledPin = 7;   // LED pin to indicate GPS fix status

TinyGPSPlus gps;
SoftwareSerial ssGPS(gpsRxPin, gpsTxPin);

void setup() {
  Serial.begin(9600);
  ssGPS.begin(9600);
  pinMode(gpsPpsPin, INPUT);
  pinMode(gpsEnPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Enable the GPS module
  digitalWrite(gpsEnPin, HIGH);
}

void loop() {
  // Read data from the GPS module
  while (ssGPS.available() > 0) {
    char c = ssGPS.read();
    if (gps.encode(c)) {
      // If new data is parsed, display information
      displayGPSInfo();
    }
  }

  // Check for GPS fix using Pulse Per Second signal
  if (digitalRead(gpsPpsPin) == HIGH) {
    digitalWrite(ledPin, HIGH); // Turn on the LED to indicate GPS fix
  } else {
    digitalWrite(ledPin, LOW); // Turn off the LED if no GPS fix
  }
}

void displayGPSInfo() {
  // Display time, latitude, longitude, and altitude
  if (gps.time.isValid()) {
    Serial.print("Time: ");
    Serial.print(gps.time.hour()+2);
    printDigits(gps.time.minute());
    printDigits(gps.time.second());
    Serial.println();
  }
  
  Serial.print("Latitude: ");
  Serial.println(gps.location.lat(), 6);
  
  Serial.print("Longitude: ");
  Serial.println(gps.location.lng(), 6);
  
  Serial.print("Altitude (meters): ");
  Serial.println(gps.altitude.meters());

  // Display additional information if available
  if (gps.speed.isValid()) {
    Serial.print("Speed (km/h): ");
    Serial.println(gps.speed.kmph());
  }
  
  if (gps.course.isValid()) {
    Serial.print("Course (degrees): ");
    Serial.println(gps.course.deg());
  }

  if (gps.satellites.isValid()) {
    Serial.print("Number of satellites: ");
    Serial.println(gps.satellites.value());
  }

  // Print a blank line to separate data sets
  Serial.println();
}

void printDigits(int digits) {
  if (digits < 10) {
    Serial.print('0');
  }
  Serial.print(digits);
}
