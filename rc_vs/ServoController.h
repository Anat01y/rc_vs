#pragma once
#include <Servo.h>
#define N 1500
#define MAXspd 2200
#define MINspd 800
#define MAXang 55
#define MINang 125

class ServoController
{
private:
	Servo servo, engine;
	unsigned char angle, spd, minA, maxA, minS, maxS;
public:
	ServoController(unsigned char angleMin, unsigned char angleMax, unsigned char speedMin, unsigned char speedMax)
	{
		minA = angleMin;
		maxA = angleMax;
		minS = speedMin;
		maxS = speedMax;
		spd = N; 
		angle = map(100, 0, 200, MINang, MAXang);
		servo.attach(D5);
		servo.write(angle);
		engine.attach(D6, MINspd, MAXspd);
		engine.writeMicroseconds(spd);
	}
	int getSpeed() {
		return spd;
	}
	int getAngle() {
		return angle;
	}
	void setSpeed(unsigned char newSpeed) {
		if ((newSpeed > maxS) || (newSpeed < minS)) return;
		spd = newSpeed;
	}
	void setAngle(unsigned char newAngle) {
		if ((newAngle > maxA) || (newAngle < minA)) return;
		angle = newAngle;
	}
	void setDefaultValues() {
		spd = (minS + maxS) / 2;
		angle = (minA + maxA) / 2;
	}
	void processValues() {
		servo.write(map(angle, minA, maxA, MINang, MAXang));
		engine.writeMicroseconds(map(spd, minS, maxS, MINspd, MAXspd));
		//Serial.printf("angle %d speed %d\n", map(angle, minA, maxA, MINang, MAXang), map(spd, minS, maxS, MINspd, MAXspd));
	}
	~ServoController() {

	}
};

