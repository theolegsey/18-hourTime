#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const int gpsRxPin = 2; // GPS TX to Arduino RX
const int gpsTxPin = 3; // GPS RX to Arduino TX
const int gpsPpsPin = 4; // Pulse Per Second pin
const int gpsEnPin = 5; // Enable pin
const int ledPin = 6;   // LED pin to indicate GPS fix status
TinyGPSPlus gps;
SoftwareSerial ssGPS(gpsRxPin, gpsTxPin);
long tH(int hours, int minutes, int seconds);
long tM(int hours, int minutes, int seconds);
int Min;
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
/*void transfotm18(int hours, int minutes, int seconds, float hour18, float minute18){
  int commonT;
  hours*=3600;          //hours to seconds
  minutes*=60;          //minutes to seconds
  commonT=(hours+minutes+seconds)*0,135; //multiply 5,4*5,4*1,5 & divide on 324
  hour18=commonT/324;                    //full part hours
  minute18=(commonT % 324)*324;          //reminder*324 -minutes
  }*/
long tH(int hours, int minutes, int seconds){ 
  long H;
  float fV = 43.74;
  double result;
  H = hours*3600+65536;
  H = H+minutes*60;
  H =H+seconds;
  result = H*fV;
  return result/(324*324);
  }
long tM(int hours, int minutes, int seconds){
  long H2;
  int H1;
  float fV = 43.74; // (36/24*324/60*324*60)
  double result;
  H2 = hours*3600+65536;
  H2 = H2+minutes*60;
  H2 =H2+seconds;
  result = H2*fV;
  result = result/324;
  H1 =int(result)%324;
  return H1;
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
    Serial.print(tH(adjustedHour, gps.time.minute(), gps.time.second()));
    Serial.print(":");
    Min = tM(adjustedHour, gps.time.minute(), gps.time.second());
    addZeros(Min);
    Serial.println();
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
 if(minutes<10){
  Serial.print("00");
  }
 if(minutes<100&minutes>10){
  Serial.print("0");
  }
  Serial.print(minutes);
  }
void printDigits(int digits) {
  if (digits < 10) {
    Serial.print('0');
  }
  Serial.print(digits);
}
