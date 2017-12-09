#pragma once
#include <WiFiUdp.h>

class ConnectionController {
private:
	WiFiUDP Udp, UdpBroadcast;
	boolean connected;
	IPAddress broadcastIP, localIP;
	int broadcastPort, messagesPort;
	unsigned long timeout;
	unsigned char incomingPacket[2];
	void broadcastHelloByte() {
		UdpBroadcast.beginPacketMulticast(broadcastIP, broadcastPort, localIP);
		UdpBroadcast.write(250);
		UdpBroadcast.endPacket();
	}
public:
	ConnectionController(char myIP[], int BroadcastPort, int MessagesPort, unsigned long timeoutMicroseconds) {
		localIP.fromString(myIP);
		broadcastIP = ~WiFi.subnetMask() | WiFi.gatewayIP();
		broadcastPort = BroadcastPort;
		messagesPort = MessagesPort;
		connected = false;
		timeout = timeoutMicroseconds;
		Udp.begin(messagesPort);
		UdpBroadcast.begin(broadcastPort);
	}
	boolean getPacket() {
		static unsigned long lastGetTime = millis();
		if (!connected) {
			broadcastHelloByte();
		}
		int packetSize = Udp.parsePacket();
		if (packetSize > 0) {
			lastGetTime = millis();
			connected = true;
			int len = Udp.read(incomingPacket, 2);
			Serial.printf("angle %d speed %d\n", incomingPacket[0], incomingPacket[1]);
		}
		if ((millis() - lastGetTime) > timeout) {
			connected = false;
		}
		return connected;
	}
	boolean getConnectionState() {
		return connected;
	}
	unsigned char getSpeed() {
		return incomingPacket[1];
	}
	unsigned char getAngle() {
		return incomingPacket[0];
	}
};