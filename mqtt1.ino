#include <WiFi.h>
#include <PubSubClient.h>

// --- Ganti dengan kredensial Anda ---
const char* ssid = "TEI_1";
const char* password = "risol2000";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;


// Pin GPIO yang terhubung ke relay
const int RELAY_PIN = 2;

WiFiClient espClient;
PubSubClient client(espClient);

// --- Fungsi Callback: Menerima Pesan MQTT ---
void callback(char* topic, byte* payload, unsigned int length) {
  // Ubah payload (byte) menjadi String untuk kemudahan
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Pesan diterima di topik: ");
  Serial.println(topic);
  Serial.println("Pesan: " + message);

  // Periksa isi pesan
  if (message == "on") {
    digitalWrite(RELAY_PIN, HIGH); // Mengaktifkan relay (menyalakan lampu)
    Serial.println("Lampu menyala!");
  } else if (message == "off") {
    digitalWrite(RELAY_PIN, LOW); // Menonaktifkan relay (mematikan lampu)
    Serial.println("Lampu mati!");
  }
}

// --- Fungsi Reconnect ---
void reconnect() {
  while (!client.connected()) {
    Serial.println("Mencoba koneksi MQTT...");
    if (client.connect("ESP32_Lampu")) {
      Serial.println("Terhubung!");
      // Langganan ke topik untuk mengendalikan lampu
      client.subscribe("tei/lampu");
      Serial.println("Berhasil langganan topik: " + String(mqtt_topic_sub));
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

// --- Fungsi Setup ---
void setup() {
  Serial.begin(115200);
  
  // Set pin GPIO sebagai OUTPUT dan atur status awal OFF
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  
  // Koneksi Wi-Fi
  Serial.print("Menghubungkan ke Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi terhubung!");
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// --- Fungsi Loop ---
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Ini wajib agar MQTT berjalan
}
