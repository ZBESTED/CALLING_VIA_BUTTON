
// LINE_TOKEN
void Line_Notify(String message);


#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>

// Config connect WiFi
#define WIFI_SSID "IntelR8"
#define WIFI_PASSWORD "12345678"

// Line config
#define LINE_TOKEN "VnjHE3pFddSZkmNVizDYSE8mJxZOtLHsPckil9HsKdH"

#define SW D2

String message = "1st Floor"; 

void setup() {
	pinMode(SW, INPUT);

	Serial.begin(9600);

	// connect to wifi.
	WiFi.mode(WIFI_STA);

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

	// เว็บที่ไว้แปลงภาษาไทย https://meyerweb.com/eric/tools/dencoder/
	LINE.notify("Calling");

	if (digitalRead(SW) == HIGH) {
		while (digitalRead(SW) == HIGH) delay(10);

		Serial.println("Enter !");
		LINE_Notify(message);
	}
	delay(10);
}

void LINE_Notify(String message) {
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
	while (client.connected()) {
		String line = client.readStringUntil('\n');
		if (line == "\r") {
			break;
		}
		//Serial.println(line);
	}
	// Serial.println("-------------");
}