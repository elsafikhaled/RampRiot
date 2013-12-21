#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     HTIRS2,         sensorI2CCustom)
#pragma config(Sensor, S3,     USonic,         sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     Motor1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     Motor2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     Motor3,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Motor4,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Motor5,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     Motor6,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(Tetrix)
//Include Dependien
#include "\Programs\RampRiot\utils\math.h"
#include "JoystickDriver.c"
#include "\Programs\RampRiot\HolomonicAPI.h"

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

/*
Combines all functions in the Library to set the robots motors
power to head in a direction.
*/
void assignMotorSpeedFromJoyStick()
{
	//Get the direction in degrees that the robot needs to head from a coordinate map
	int direction = getDirectionFromLocation(joystick.joy1_x1, joystick.joy1_y1);
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

	waitForStart();

	while(true)
	{
		if(joystick.joy2_Buttons & button5)
		{
			motor[Motor5] = -100; // For future ref., Motor 5 is left, Motor 6 is right.

		} else if(joystick.joy2_Buttons & button7) // For future ref., Motor 5 is left, Motor 6 is right.
		{
			motor[Motor5] = 100;
		}
		else if(joystick.joy2_Buttons & button3){ // For future ref., Motor 5 is left, Motor 6 is right.
			motor[Motor5] = 0;
		}
		if(joystick.joy2_Buttons & button6){
			motor[Motor6] = -100; // For future ref., Motor 5 is left, Motor 6 is right.
		}
		else if(joystick.joy2_Buttons & button8){
			motor[Motor6] = 100; // For future ref., Motor 5 is left, Motor 6 is right.
		}
		else if(joystick.joy2_Buttons & button4){
			motor[Motor6] = 0; // For future ref., Motor 5 is left, Motor 6 is right.
		}
		//if(joystick.joy2_Buttons & button2)
		//{
		//} else if(joystick.joy2_Buttons & button4)
		//{
		//}

		//if (joystick.joy2_Buttons & button1)
		//{

		//} else if (joystick.joy2_Buttons & button3)
		//{
		//}  Used at Ramp Riot to power our dino arm block movers. R.I.P. Dino arms

		if(joystick.joy1_Buttons & button5)
		{
			move(spin_right, getVelocityFromJoy2());
		} else if(joystick.joy1_Buttons & button6)
		{
			move(spin_left, getVelocityFromJoy2());
		} else
		{
			assignMotorSpeedFromJoyStick();
		}
	}
}
