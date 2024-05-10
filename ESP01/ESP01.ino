#include <DHT11.h>
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>

#define mqtt_server "192.168.1.54"
#define ssid        "Thu Trang" 
#define pass        "88888888"

#define humidity_topic    "sensor/DHT11/humidity"
#define temperature_topic "sensor/DHT11/temperature"

#define DHTPIN 2
DHT11 dht(DHTPIN);

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
   // Xử lý dữ liệu nhận được
}

void setup() {

  Serial.begin(115200);
  setup_wifi();

  client.setServer(mqtt_server, 1883);
  
  //client.setCallback(callback);
}

void loop() {
  
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  delay(1000);

  float humi = 1;//= dht.readHumidity();
  float temp = 1;//= dht.readTemperature();
  if (isnan(humi) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }
  
  Serial.print("Temperature:");
  Serial.println(String(temp).c_str());
  client.publish(temperature_topic, String(temp).c_str(), true);

  Serial.print("Humidity:");
  Serial.println(String(humi).c_str());
  client.publish(humidity_topic, String(humi).c_str(), true);
}

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}