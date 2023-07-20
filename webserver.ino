#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFi.h>

#include "index.h"

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

// setup gps pins
TinyGPSPlus gps;
#define RXD2 16
#define TXD2 17
int counter = 0;

// SSID & Password
const char* ssid = "SSID";  // Enter your SSID here
const char* password = "PASSWORD";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void setup() {
        // Initialize serial ports
        Serial.begin(115200); // Serial port of USB
        Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // GPS Serial, BAUD, SERIAL_8N1, RXD, TXD

        // Initialize LCD
        lcd.init();
        lcd.backlight(); // Turn on LCD backlight 

        // Initialize WiFi
        Serial.println("Trying to connect to ");
        Serial.println(ssid);
        WiFi.begin(ssid, password); // Connect to wi-fi
        // Loop to connect to WiFi, print a . at each attempt
        while (WiFi.status() != WL_CONNECTED) {
          delay(1000);
          Serial.print(".");
        }
        Serial.println(""); // New line
        Serial.println("WiFi connected successfully");
        Serial.print("Got IP: ");
        Serial.println(WiFi.localIP());  // Show ESP32 IP on serial

        // Initialize the web server
        server.on("/", handleRoot);  // Direct browser client to handleRoot loop
        server.on("/readHDOP", handleHDOP); // Update the HDOP value
        server.on("/readLat", handleLat); // Update latitude value
        server.on("/readLong", handleLong); // Update Longtitude value
        server.on("/readCounter", handleCounter); //Update counter
        server.begin(); // Start the webserver
        Serial.println("HTTP server started");
        delay(100);
}

void loop() {
server.handleClient();
    while (Serial2.available()) {
      gps.encode(Serial2.read());
    }
    Serial.println("****    START READING    ****");
    Serial.print("LAT=          "); Serial.println(gps.location.lat(), 6);
    Serial.print("LONG=         "); Serial.println(gps.location.lng(), 6);
    Serial.print("ALT=          "); Serial.println(gps.altitude.meters());
    Serial.print("Sats=         "); Serial.println(gps.satellites.value()); // Number of satellites in use (u32)
    Serial.print("HDOP=         "); Serial.println(gps.hdop.value()); // Horizontal Dim. of Precision (100ths-i32)
    Serial.print("RawLat=       "); Serial.print(gps.location.rawLat().negative ? "-" : "+");
    Serial.print("RawLat=       "); Serial.println(gps.location.rawLat().deg); // Raw latitude in whole degrees
    Serial.print("RawLat=       "); Serial.println(gps.location.rawLat().billionths);// ... and billionths (u16/u32)
    Serial.print("RawLng=       "); Serial.print(gps.location.rawLng().negative ? "-" : "+");
    Serial.print("RawLng=       "); Serial.println(gps.location.rawLng().deg); // Raw longitude in whole degrees
    Serial.print("RawLng=       "); Serial.println(gps.location.rawLng().billionths);// ... and billionths (u16/u32)
    Serial.print("Date=         "); Serial.println(gps.date.value()); // Raw date in DDMMYY format (u32)
    Serial.print("Year=         "); Serial.println(gps.date.year()); // Year (2000+) (u16)
    Serial.print("Month=        "); Serial.println(gps.date.month()); // Month (1-12) (u8)
    Serial.print("Day=          "); Serial.println(gps.date.day()); // Day (1-31) (u8)
    Serial.print("Time=         "); Serial.println(gps.time.value()); // Raw time in HHMMSSCC format (u32)
    Serial.print("Hour=         "); Serial.println(gps.time.hour()); // Hour (0-23) (u8)
    Serial.print("Minute=       "); Serial.println(gps.time.minute()); // Minute (0-59) (u8)
    Serial.print("Second=       "); Serial.println(gps.time.second()); // Second (0-59) (u8)
    Serial.print("CentiSec=     "); Serial.println(gps.time.centisecond()); // 100ths of a second (0-99) (u8)
    Serial.print("RawSpeed=     "); Serial.println(gps.speed.value()); // Raw speed in 100ths of a knot (i32)
    Serial.print("SpeedKnots=   "); Serial.println(gps.speed.knots()); // Speed in knots (double)
    Serial.print("MPH=          "); Serial.println(gps.speed.mph()); // Speed in miles per hour (double)
    Serial.print("MPS=          "); Serial.println(gps.speed.mps()); // Speed in meters per second (double)
    Serial.print("KPH=          "); Serial.println(gps.speed.kmph()); // Speed in kilometers per hour (double)
    Serial.print("RawCourse=    "); Serial.println(gps.course.value()); // Raw course in 100ths of a degree (i32)
    Serial.print("Course=       "); Serial.println(gps.course.deg()); // Course in degrees (double)
    Serial.print("AltCm=        "); Serial.println(gps.altitude.value()); // Raw altitude in centimeters (i32)
    Serial.print("AltM=         "); Serial.println(gps.altitude.meters()); // Altitude in meters (double)
    Serial.print("AltMi=        "); Serial.println(gps.altitude.miles()); // Altitude in miles (double)
    Serial.print("AltKm=        "); Serial.println(gps.altitude.kilometers()); // Altitude in kilometers (double)
    Serial.print("AltF=         "); Serial.println(gps.altitude.feet()); // Altitude in feet (double)
    Serial.print("Counter=      "); Serial.println(counter);
    Serial.println(WiFi.localIP());
    counter++;
    //delay(2000);

    lcd.clear();
    // set cursor to first column, first row
    lcd.setCursor(0, 0);
    lcd.print(gps.location.lat(), 6);
    lcd.print(" H"); lcd.println(gps.hdop.value());
    lcd.setCursor(0,1);
    lcd.print(gps.location.lng(), 6);
    lcd.print(" S"); lcd.println(gps.satellites.value());

    delay(2000);
}

void handleRoot() {
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void handleHDOP() {
  String HDOPValue = String(gps.hdop.value());
  // Serial.println(gps.hdop.value()); // Horizontal Dim. of Precision (100ths-i32)
  server.send(200, "text/plane", HDOPValue); // Send HDOP value only to client ajax request
}

void handleLat() {
  String LatValue = String(gps.location.lat());
  server.send(200, "text/plane", LatValue); // Send Lat value only to client ajax request
}

void handleLong() {
  String LongValue = String(gps.location.lng());
  server.send(200, "text/plane", LongValue); // Send Long value only to client ajax request
}

void handleCounter() {
  String CounterValue = String(counter);
  server.send(200, "text/plane", CounterValue); // Send Long value only to client ajax request
}
