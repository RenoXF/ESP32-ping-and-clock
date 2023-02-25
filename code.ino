#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>               
#include <TimeLib.h>               
#include <LiquidCrystal_I2C.h> 
#include <ESP32Ping.h>     
LiquidCrystal_I2C lcd(0x27, 16, 2); 
 
const char *ssid     = "SSID";          //set your wifi ssid
const char *password = "PASSWORD";      //set your wifi password

const char* remote_host = "google.com"; //host to ping
 
WiFiUDP ntpUDP;
 
 
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", 25200, 60000);  //set your time server
 
char Time[ ] = "TIME:00:00:00";
byte last_second, second_, minute_, hour_;
 
void setup() {
 
  Serial.begin(115200);
  lcd.init();
  lcd.clear();
  lcd.backlight();                 
  lcd.setCursor(0, 0);
  lcd.print(Time);
 
  WiFi.begin(ssid, password);
  Serial.print("Connecting.");
 
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  timeClient.begin();
}
 
 
void loop() {
 
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
 
  second_ = second(unix_epoch);
  if (last_second != second_) {
 
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
 
 
 
    Time[12] = second_ % 10 + 48;
    Time[11] = second_ / 10 + 48;
    Time[9]  = minute_ % 10 + 48;
    Time[8]  = minute_ / 10 + 48;
    Time[6]  = hour_   % 10 + 48;
    Time[5]  = hour_   / 10 + 48;
 
    lcd.setCursor(0, 0);
    lcd.print(Time);
    if(Ping.ping(remote_host)) {

    lcd.setCursor(3, 1);
    lcd.print(Ping.averageTime());
    lcd.setCursor(10, 1);
    lcd.print(" ms");
    
  } else {
    lcd.setCursor(3, 1);
    lcd.print("ERROR");
  }

    last_second = second_;
 
  } 
  delay(500);
}