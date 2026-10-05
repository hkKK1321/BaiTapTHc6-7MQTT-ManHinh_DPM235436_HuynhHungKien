#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const char* topic_k = "agu/iot/nhietdo";

WiFiClient espClient;
PubSubClient client(espClient);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) { message += (char)payload[i]; }
  
  Serial.print("[Board 2 - LCD] Nhan du lieu: "); Serial.println(message);
  
  lcd.setCursor(0, 0);
  lcd.print("NHIET DO HE THONG");
  lcd.setCursor(0, 1);
  lcd.print("Temp: " + message + " C   ");
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32_Display_";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.subscribe(topic_k);
      Serial.println("[Board 2] Da Subscribe kenh (+k)!");
    } else { delay(2000); }
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Dang ket noi...");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) { reconnect(); }
  client.loop();
}
