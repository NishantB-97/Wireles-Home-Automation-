


/////////////////////  Project ///////////////////////////////
///////////////// Wireless Smart Hub /////////////////////////

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


int relay1Pin = D1;//define a pin for relay
int relay2Pin = D2;//define a pin for relay

int relay1State = 1;//initial state . 1 ON, 0 OFF
int relay2State = 1;//initial state . 1 ON, 0 OFF


String button1Lable1 ="Switch1_ON";
String button1Lable2 ="Switch1_OFF";
String button2Lable1 ="Switch2_ON";
String button2Lable2 ="Switch2_OFF";



#define APSSID "AP_Name" // your WiFi SSID
#define APPSK  "AP_Password" //your WiFi password


const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

/////////////////////////////////////////////////////////////////////////////////////

void handleRoot() {
String HTML ="<!DOCTYPE html>\<html>\<head>\<title> Control Panel</title>\<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\<style>\html,body{width:100%\;height:100%\;margin:0}*{box-sizing:border-box}.colorAll{background-color:#90ee90}.colorBtn{background-color:#add8e6}.angleButtdon,a{font-size:30px\;border:1px solid #ccc\;display:table-caption\;padding:7px 10px\;text-decoration:none\;cursor:pointer\;padding:5px 6px 7px 10px}a{display:block}.btn{margin:5px\;border:none\;display:inline-block\;vertical-align:middle\;text-align:center\;white-space:nowrap}";

HTML +="</style></head><body><h1> Control Panel </h1>";

if(relay1State){
HTML +="<div class=\"btn\"><a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/control?r1=off\">";
HTML +=button1Lable2;
}else{
HTML +="<div class=\"btn\"><a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/control?r1=on\">";
HTML +=button1Lable1;
}
HTML +="</a></div>";

if(relay2State){
HTML +="<div class=\"btn\"><a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/control?r2=off\">";
HTML +=button2Lable2;
}else{
HTML +="<div class=\"btn\"><a class=\"angleButton \" style=\"background-color:#90ee90\"  href=\"/control?r2=on\">";
HTML +=button2Lable1;
}
HTML +="</a></div></body></html>";
server.send(200, "text/html", HTML);
}

////////////////////////////////////////////////////////////////////////////////////////////

void handleNotFound() {

String message = "File Not Found";
message += "URI: ";
message += server.uri();
message += "Method: ";
message += (server.method() == HTTP_GET) ? "GET" : "POST";
message += "Arguments: ";
message += server.args();
message += "";

for (uint8_t i = 0; i < server.args(); i++) {
message += " " + server.argName(i) + ": " + server.arg(i) + "";
}

server.send(404, "text/plain", message);

}

/////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void) {
pinMode(relay1Pin, OUTPUT);  // define a pin as output for relay
pinMode(relay2Pin, OUTPUT);  // define a pin as output for relay

Serial.begin(115200);  //initialize the serial monitor

WiFi.mode(WIFI_AP);   //Setting WiFi Mode as Access Point
WiFi.softAP(ssid, password); //WiFi AP setup

Serial.println("Server Started  ");
Serial.print("To Access Contro Panel you'll have to connect to AP by using SSID = ");
Serial.print(ssid);
Serial.print("   &  PassKey = ");
Serial.print(password);

//The mDNS responder is used to provide a domain name for the ESP8266 on the local network

if (MDNS.begin("ESP8266_Control-Panel")) {
Serial.println("  MDNS responder started");
Serial.println("access it via http://ESP8266_Control-Panel");
}

server.on("/", handleRoot);
server.on("/control", HTTP_GET, relayControl);
server.onNotFound(handleNotFound);
server.begin();
Serial.println("HTTP server started");

}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void loop(void) {

server.handleClient();
MDNS.update();

if(relay1State ==1)
{
digitalWrite(relay1Pin, LOW);
}else
{
digitalWrite(relay1Pin, HIGH);
}

if(relay2State ==1)
{
digitalWrite(relay2Pin, LOW);
}else{
digitalWrite(relay2Pin, HIGH);
}

delay(100);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////


//fuction for updating relaystate variables state..


void relayControl() {
if(server.arg("r1") == "on")
{
relay1State = 1;// set state of relay1 to ON
}else if(server.arg("r1") == "off"){
relay1State = 0;  // set state of relay1 to OFF
}

if(server.arg("r2") == "on") {
relay2State =1; // set state of relay2 to ON
}else if(server.arg("r2") == "off"){
relay2State =0;  // set state of relay2 to OFF
}

handleRoot();
}


///////////////////////////////////////   END   ///////////////////////////////////////////////////////////////
