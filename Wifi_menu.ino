#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#ifndef APSSID
#define APSSID "BadgeSFW1"  //SSID
#define APPSK  "Password"   //PSK
#endif
void handleRoot();              // function prototypes for HTTP handlers
void handleLogin();
void handleNotFound();
LiquidCrystal_I2C lcd(0x27, 20, 4);
const char *ssid = APSSID;
const char *password = APPSK;
ESP8266WebServer server(80);

// Go to http://192.168.4.1 in a web browser
void handleRoot() {
server.send(200, "text/html", "<head> <meta name='viewport' content='width=device-width,initial-scale=1'><title>Insert Text</title>"
"<style>html, body {width: 300%;height: 300%;;margin: 0;padding: 0;background-color: red;}h1{margin: 0;padding: 0;</style></head>"
"<form action=\"/login\" method=\"POST\">"
"<input type=\"text\" name=\"Line1\" placeholder=\"Line1\" size='20' maxlength='20'></br>"
"<input type=\"text\" name=\"Line2\" placeholder=\"Line2\" size='20' maxlength='20'></br>"
"<input type=\"text\" name=\"Line3\" placeholder=\"Line3\" size='20' maxlength='20'></br>"
"<input type=\"text\" name=\"Line4\" placeholder=\"Line4\" size='20' maxlength='20'></br>"
"<input type=\"submit\" value=\"Continue\">"
"</p>");
}

void handleLogin() {                         // If a POST request is made to URI /login
server.send(200, "text/html", "<head> <meta name='viewport' content='width=device-width,initial-scale=1'>"
"<title>Text Complete</title><style>html,body {width: 100%;height: 100%;margin: 0;padding: 0;background-color: red;}"
"h1 {margin: 0;padding: 0;</style></head>"
"<h1></br>" + server.arg("Line1") + server.arg("Line2") + server.arg("Line3") + server.arg("Line4") + "<form action=\"/\" method=\"POST\">"
"</br> <input type=\"submit\" value=\"Continue\">");

lcd.clear();
lcd.setCursor(0,0);
lcd.print(server.arg("Line1"));
lcd.setCursor(0,1);
lcd.print(server.arg("Line2"));
lcd.setCursor(0,2);
lcd.print(server.arg("Line3"));
lcd.setCursor(0,3);
lcd.print(server.arg("Line4"));
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connect to BadgeSFW1");
  lcd.setCursor(0,1);
  lcd.print("And goto 192.168.4.1");
  delay(1000);
  WiFi.softAP(ssid, password);
  server.on("/", HTTP_GET, handleRoot);// Call the 'handleRoot' function when a client requests URI "/"
  server.on("/", HTTP_POST, handleRoot); 
  server.on("/login", HTTP_POST, handleLogin); // Call the 'handleLogin' function when a POST request is made to URI "/login"
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();                            // Actually start the server;
}

void loop() {
  server.handleClient();
}
