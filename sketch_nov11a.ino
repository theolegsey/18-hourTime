#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const int gpsRxPin = 2; // GPS TX to Arduino RX
const int gpsTxPin = 3; // GPS RX to Arduino TX
const int gpsPpsPin = 4; // Pulse Per Second pin
const int gpsEnPin = 5; // Enable pin
const int ledPin = 6;   // LED pin to indicate GPS fix status
double h1, m1;          //transformed time to 18-hour format
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
void transfotm18(int hours, int minutes, int seconds, float hour18, float minute18){
  int commonT;
  hours*=3600;          //hours to seconds
  minutes*=60;          //minutes to seconds
  commonT=(hours+minutes+seconds)*0,135; //multiply 5,4*5,4*1,5 & divide on 324
  hour18=commonT/324;                    //full part hours
  minute18=(commonT % 324)*324;          //reminder*324 -minutes
  }
void displayGPSInfo() {
  // Display time, latitude, longitude, and altitude
  if (gps.time.isValid()) {
    int adjustedHour = (gps.time.hour() + 2) %24;
    Serial.print(adjustedHour);
    Serial.print(":");
    printDigits(gps.time.minute());
    Serial.print(":");
    printDigits(gps.time.second()); 
    Serial.println();
    transfotm18(gps.time.hour(), gps.time.minute(),gps.time.second(), h1, m1);
    Serial.print(h1);
    Serial.print(":");
    Serial.println(m1);
  }
  
  Serial.print("Latitude: ");
  Serial.println(gps.location.lat(), 6);
  
  Serial.print("Longitude: ");
  Serial.println(gps.location.lng(), 6);
  
  Serial.print("Altitude (meters): ");
  Serial.println(gps.altitude.meters());

  if (gps.satellites.isValid()) {
    Serial.print("Number of satellites: ");
    Serial.println(gps.satellites.value());
  }

  // Print a blank line to separate data sets
  Serial.println();
}
void addZeros(int minutes){
  String updatedZeros;
  String stringMin = String(minutes, DEC);
 if(minutes<10){
  updatedZeros = "00"+ stringMin;
  }
 if(minutes<100&minutes>10){
  updatedZeros = "0"+ stringMin;
  }
  return updatedZeros;
  }
void printDigits(int digits) {
  if (digits < 10) {
    Serial.print('0');
  }
  Serial.print(digits);
}
