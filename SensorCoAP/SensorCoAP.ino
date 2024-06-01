#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

const char* ssid = "Phuong Huyen";
const char* password = "123456789";

IPAddress gateway_ip(192, 168, 1, 8); // Thay bằng địa chỉ IP của ESP32 gateway
const int gateway_port = 7000; // Cổng CoAP

const int ldrPin = 34;
const int mq02Pin = 35;

WiFiUDP udp;
Coap coap(udp);

void setup() {
  Serial.begin(115200);

  pinMode(ldrPin, INPUT);
  pinMode(mq02Pin, INPUT);
  // Kết nối WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  coap.start();
}

void loop() {
  
  int ldrValue = analogRead(ldrPin);
  int mq02Value = analogRead(mq02Pin);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  Serial.print("MQ02 Value: ");
  Serial.println(mq02Value);

  String payload = "{ldr: " + String(ldrValue) + ", airquality: " + String(mq02Value) + "}";
  
  // Gửi yêu cầu PUT đến ESP32 gateway
  coap.put(gateway_ip, gateway_port, "coap", payload.c_str());
  delay(3000);
}
