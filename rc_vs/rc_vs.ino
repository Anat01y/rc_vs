#include <ESP8266WiFi.h>
#include "ServoController.h"
#include "ConnectionController.h"

char localIP[20];

void setup() {
	wifiInit("Tolik", "qwe123rty", "esp8266");
	//wifiInit("ZTE-ca7e4e", "ZTEGC1D16B34", "esp8266");
	//wifiInit("Толик", "qwe123rty", "esp8266");
	pinMode(D0, OUTPUT);
	pinMode(D1, OUTPUT);
	digitalWrite(D0, HIGH);
	digitalWrite(D1, HIGH);
}

void loop() {
	static ServoController sc(0, 200, 0, 200);
	static ConnectionController connection(localIP, 4320, 4321, 1000);

	if (connection.getPacket()) {
		sc.setAngle(connection.getAngle());
		sc.setSpeed(connection.getSpeed());
		digitalWrite(D0, LOW);
	}
	else {
		sc.setDefaultValues();
		digitalWrite(D0, HIGH);
	}

	sc.processValues();

	
}

void wifiInit(char ssid[], char password[], char hostName[]) {
	WiFi.mode(WIFI_STA);//
	WiFi.hostname(hostName);//
	WiFi.begin(ssid, password); //
	Serial.begin(9600);
	int i = 0;
	while (WiFi.status() != WL_CONNECTED) //
	{
		Serial.println(".");//
		delay(1000);//
		i++;
		if (i > 15) {
			WiFi.mode(WIFI_AP);
			WiFi.softAP(ssid, password);
			break;
		}
	}
	Serial.print("IP address: ");
	if (i > 15)
		strcpy(localIP, WiFi.softAPIP().toString().c_str());
	else 
		strcpy(localIP, WiFi.localIP().toString().c_str());
	Serial.println(localIP);
}