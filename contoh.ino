#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>

const char* ssid = "TES";   // Nama Wifi
const char* password = "12345678"; // Password Wifi

ESP32WebServer server(80);
String command;             //String command, variable untuk menyimpan perintah


void HTTP_handleRoot() {
  if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "SERVER AKTIF GUYS" );
  delay(1);
}


void setup(void){
  Serial.begin(115200);  
  pinMode(2,OUTPUT); // OUTPUT LED PADA PIN 2
   WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Memulai WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin(); 
}

void loop(void){
  server.handleClient();
      command = server.arg("State");  // Command diawali dengan perintah State sehingga url (http://192.168.4.1/?State=)
      if (command == "A") {digitalWrite(2,HIGH);} // Jika command yang diterima huruf A maka LED ON
      if (command == "B") {digitalWrite(2,LOW);} // Jika command yang diterima huruf B maka LED OFF
}
