/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <sys/time.h>
#include "time.h"

const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

void autoTimeSetup(){
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void autoTimeLoop(){
  struct tm timeinfo;

  if(getLocalTime(&timeinfo)){
    Serial.println("%A, %H:%M:%S");
  }

  delay(60000);
}

// Set your manual time: Year, Month (0-11), Day, Hour, Min, Sec
void setManualTime(int year, int month, int day, int hour, int min, int sec) {
  struct tm tm;
  tm.tm_year = year - 1900; // Years since 1900
  tm.tm_mon = month - 1;    // Month 0-11
  tm.tm_mday = day;
  tm.tm_hour = hour;
  tm.tm_min = min;
  tm.tm_sec = sec;
  tm.tm_isdst = -1;        // Not using DST

  time_t t = mktime(&tm);
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}

void manualTimeSetup() {
  Serial.begin(115200);
  // Example: Set to Feb 12, 2026, 10:00:00
  setManualTime(2026, 2, 12, 10, 0, 0);
  Serial.println("Time Set Manually");
}

void manualTimeLoop() {
  time_t now;
  time(&now);
  struct tm *timeinfo = localtime(&now);
  
  // Print time
  Serial.println(asctime(timeinfo));
  delay(60000); // Print every second
}