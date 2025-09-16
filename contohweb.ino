#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <WiFi.h>

const char* ssid = "TES";   // Nama Wifi
const char* password = "12345678"; // Password Wifi

ESP32WebServer server(80);
String command;             //String command, variable untuk menyimpan perintah


void HTTP_handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32 Control</title>
      <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
        .button {
          display: inline-block;
          padding: 15px 30px;
          font-size: 24px;
          cursor: pointer;
          text-decoration: none;
          color: white;
          border-radius: 5px;
          margin: 10px;
        }
        .on { background-color: #4CAF50; } /* Green */
        .off { background-color: #f44336; } /* Red */
      </style>
    </head>
    <body>
      <h1>Kontrol LED</h1>
      <a href="/?State=A" class="button on">ON</a>
      <a href="/?State=B" class="button off">OFF</a>
    </body>
    </html>
  )rawliteral";
  server.send(200, "text/html", html);
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
  command = server.arg("State");  // Command diawali dengan perintah State sehingga url (http://192.168.4.1/?State=)
  if (command == "A") {
    digitalWrite(2,HIGH);
    Serial.println("LED ON");
  } 
  if (command == "B") {
    digitalWrite(2,LOW);
    Serial.println("LED OFF");
  }
}
