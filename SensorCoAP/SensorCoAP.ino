#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

// Thông tin mạng WiFi
const char* ssid = "Phuong Huyen";
const char* password = "123456789";

// Địa chỉ IP của ESP32 gateway
IPAddress gateway_ip(192, 168, 1, 8); // Thay bằng địa chỉ IP của ESP32 gateway
const int gateway_port = 7000; // Cổng CoAP

// Pin kết nối cảm biến quang trở
const int ldrPin = 34;

// UDP và CoAP class
WiFiUDP udp;
Coap coap(udp);

void setup() {
  Serial.begin(115200);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Bắt đầu CoAP client
  coap.start();
}

void loop() {
  // Đọc giá trị từ cảm biến quang trở
  int ldrValue = analogRead(ldrPin);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Chuyển giá trị thành chuỗi
  String payload = String(ldrValue);
  
  // Gửi yêu cầu PUT đến ESP32 gateway
  coap.put(gateway_ip, gateway_port, "ldr", payload.c_str());

  // Chờ 10 giây trước khi gửi lại
  delay(3000);
}
