#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTMC,           sensorI2CCustom)
#pragma config(Sensor, S4,     HTIRS2,         sensorNone)
#pragma config(Motor,  mtr_S1_C1_1,     Motor2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     Motor3,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     Motor4,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Motor1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Motor5,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     Motor6,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     motorJ,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorK,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(Tetrix)
//Include Dependien
#include "\Programs\RampRiot\utils\math.h"
#include "\Programs\RampRiot\utils\compassStorage.h"
#include "JoystickDriver.c"
#include "\Programs\RampRiot\HolomonicAPI.h"
#include "drivers/hitechnic-compass.h"

// I don't know why these are not defined in joystickdriver.c or something.
// The TJoystick structure includes a buttons field that is a bit map of any
// and all buttons that are currently pressed. Button 1 sets bit 1 and so on.
#define button1   0x01
#define button2   0x02
#define button3   0x04
#define button4   0x08
#define button5   0x10
#define button6   0x20
#define button7   0x40
#define button8   0x80
#define button9  0x100
#define button10 0x200

/*
Checks if the joystick is being currently controlled or moved to prevent
constant actions of being ran when in a 0 0 position.
*/
bool isJoystickBeingMoved(int axis)
{
	//Is the axis of the joystick between 10 and -10
	return (axis <= 10 && axis >= -10);
}

/*
Converts the Y Mapping of the Joystick to a speed from 0-100
*/
int getVelocityFromJoy2()
{
	//0, 0  on the joystick corilates to a power of 50
	//Top = 100
	//Bottom = 100
	return (((joystick.joy1_y2 + 128) * 100) / 256);
}

int getDirection(int angle) {
	int newAngle = (angle % 360)
return newAngle < 0 ? 360 - abs(newAngle) : newAngle;
}

/*
Combines all functions in the Library to set the robots motors
power to head in a direction.
*/
void assignMotorSpeedFromJoyStick(bool useCompass)
{
	//Get the direction in degrees that the robot needs to head from a coordinate map
	float direction = getDirectionFromLocation(joystick.joy1_x1, joystick.joy1_y1);
	// 0 is the 90 as of now, so it is normalized
	//direction += 90; NVM DO THAT SOMEWHERE ELSE
	// need to prevent values > 360
	//direction = 359 - direction;
	direction += 225;  // # CALIBRATE TO ACCOUNT FOR IMPERFECTION
	while(direction >= 360)
	{
		direction -= 360;
	}
	while(direction <= -1)
	{
		direction += 360;
	}
	nxtDisplayTextLine(7, "%d", direction);
	//read compass value
	float readCompass = HTMCreadHeading(HTMC); // lets try something
	//if its negative one its disconnected
	if (readCompass != -1) {
		int offset = (-1 * (readCompass - read());
		//since the motor is not disconected then we offset
		//direction += (int)(readCompass - read());  Testing a diff formula
		// direction += (-1 * (readCompass - read()); // maybe it w0rks? nahh
		if(useCompass)
		{
			direction += offset;
		}
		// int relHead = HTMCreadRelativeHeading(HTMC); // mbe relhead is be workng ?
		// if(relHead < 0)
		// 	{
		// 	  relHead = 180 - relHead;
		// 	}
		// 	relHead = 360 - relHead;   TRYING TO NORMALIZE RELHEADING

		// relHead += 179; //To make values properly btween 0 and 359
		//nxtDisplayTextLine(6, "RELHEAD : %d", relHead);
		nxtDisplayTextLine(6, "offset : %d", offset);
		//  direction += relHead;

		//using subtraction to prevent the value from going over 360.
		if(direction > 359)
		{
			direction -= 360;
		}
		else if(direction < 0)
		{
			direction += 360;
		}
		nxtDisplayTextLine(3, "%d", direction);
	}


	//Gets the velocity/speed from Joystick 2
	float velocity = getVelocityFromJoy2();
	float defaultOffset = 45;

	//Default motor speeds are 0 if the robots are at joystick 1 at 0 , 0
	int m1 = 0;
	int m2 = 0;
	int m3 = 0;
	int m4 = 0;
	//If the joysticks are NOT at 0,0 calculate the power of the motors
	//This is required because function sin returns 70 when the joystick is at 0,0
	if(!isJoystickBeingMoved(joystick.joy1_x1) || !isJoystickBeingMoved(joystick.joy1_y1))
	{
		//The joystick is not at 0,0 so we update the power
		m1 = getMotorOutput(velocity, direction, 1 * defaultOffset);
		m2 = getMotorOutput(velocity, direction, 3 * defaultOffset);
		m3 = getMotorOutput(velocity, direction, 5 * defaultOffset);
		m4 = getMotorOutput(velocity, direction, 7 * defaultOffset);
	}
	int readDisplay = read();
	int displayHeading = HTMCreadHeading(HTMC);
	//		nxtDisplayTextLine(1, "%d", readDisplay);
	nxtDisplayTextLine(2, "Compass: %d", displayHeading);


	//Apply new power
	control(m1, m2, m3, m4);
}

void rotateRobot()
{
	//int button = joystick.joy1_Buttons;
	//if (joystick.joy1_Buttons & button5)
	if(joy1Btn(5))
	{
		move(spin_left, getVelocityFromJoy2());
	} //else if (joystick.joy1_Buttons & button6)
	else if(joy1Btn(6))
	{
		move(spin_right, getVelocityFromJoy2());
	}
}

task main()
{
	bool useCompass = true;
	HTMCsetTarget(HTMC, 315);
	waitForStart();
	eraseDisplay();
	//float displayHeading;
	//float readDisplay;
	while(true)
	{
		if(joystick.joy2_Buttons & button5)
		{
			motor[motorJ] = 35;
			//motor[MotorK] = 100;
		} else if(joystick.joy2_Buttons & button7)
		{
			motor[motorJ] = -35;
			//	motor[MotorK] = -100;
		}else
		{
			motor[motorJ] = 0;
			//	motor[MotorK] = -0;
		}

		if(joystick.joy2_Buttons & button2)
		{  // dump blocks procedure (A BUTTON)
			servo[servo3] = 255;
			wait10Msec(20);
			servo[servo1] = 35;
			servo[servo2] = 220;
		}

		else if(joystick.joy2_Buttons & button3)
		{  // PICKUP position (B BUTTON)
			servo[servo3] = 255;
			wait10Msec(20);
			servo[servo1] = 70;
			servo[servo2] = 185;
		}
		else if(joystick.joy2_Buttons & button4)
		{  // HOLD POSITION (Y BUTTON)
			servo[servo3] = 67;
			wait10Msec(20);
			servo[servo1] = 70;
			servo[servo2] = 185;
		}
		else if(joystick.joy2_Buttons & button1)
		{  // RAISE POSITION (X BUTTON)
			servo[servo3] = 255;
			wait10Msec(20);
			servo[servo1] = 155;
			servo[servo2] = 110;
		}
		else if(joystick.joy1_Buttons & button5)
		{
			move(spin_right, getVelocityFromJoy2());
		}
		else if(joystick.joy1_Buttons & button6)
		{
			move(spin_left, getVelocityFromJoy2());
		}
		else
		{
			assignMotorSpeedFromJoyStick(useCompass);
		}

		if(joystick.joy1_Buttons & button7)
		{
			motor[Motor5] = 50;
			motor[Motor6] = 50;
		}
		else if(joystick.joy1_Buttons & button8)
		{
			motor[Motor5] = -50;
			motor[Motor6] = -50;
		}
		else
		{
      motor[Motor5] = -0;
			motor[Motor6] = -0;
		}

		if(joystick.joy1_Buttons & button1)
		{
			int val;
			move (spin_left, 25);
			while (true) {
				val = HTMCreadHeading(HTMC);
				nxtDisplayString(1, "%d", val);
				nxtDisplayString(2, "%d", getDirection(read() + 40));
				nxtDisplayString(3, "%d", getDirection(read() - 40));
				if (val <= getDirection(read() + 20) && val >= getDirection(read() - 20)) {
					break;
				}
			}
			move (none, 0);
			wait10Msec(20);
			HTMCsetTarget(HTMC);
			PlaySound(soundBeepBeep);
			wait1Msec(100);
		}
		else if(joystick.joy1_Buttons & button2)
		{
			useCompass = !useCompass;
			if(useCompass)
			{
				PlaySound(soundBlip);
				PlaySound(soundBlip);
				PlaySound(soundBlip);
			}
			else
			{
				playSound(soundFastUpwardTones);
			}
			wait10Msec(50);
		}
	}
}
