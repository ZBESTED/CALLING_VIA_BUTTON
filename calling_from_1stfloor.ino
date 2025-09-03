#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>
void Line_Notify(String message) ;

#define WIFI_SSID "IntelR8" // ชื่อไวไฟ
#define WIFI_PASSWORD "12345678"//รหัสผ่านไวไฟ

// Line token
#define LINE_TOKEN "VnjHE3pFddSZkmNVizDYSE8mJxZOtLHsPckil9HsKdH"// ไลน์token
#define Switch D2 //สวิทช์
#define relay D3 //รีเลย์
String message = "Calling"; // เขียนข้อความที่ต้องการ

void setup() {
  pinMode(Switch, INPUT); 
  pinMode(relay, OUTPUT);
  Serial.begin(9600);
  digitalWrite(relay,LOW);
  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (digitalRead(Switch) == HIGH) {
//    while(digitalRead(Switch) == HIGH){ 
//   delay(10);
   Serial.println("Enter !");
   Line_Notify(message); //ฟังชั่นที่ใช้ในการส่ง
   delay(10);
   digitalWrite(relay,HIGH); //ไซเรนทำงาน
   delay(5000);// เวลาที่ตั้งให้ไซเรนดัง 5000= 5 วินาที
  }
  else
  {
   digitalWrite(relay,LOW); //ไซเรนหยุดทำงาน
  }
 }


void Line_Notify(String message) {
  WiFiClientSecure client; 

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Connection: close\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
}