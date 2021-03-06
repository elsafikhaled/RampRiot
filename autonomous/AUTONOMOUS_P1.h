#include "\Programs\RampRiot\HolomonicAPI.h"
#include "\Programs\RampRiot\utils\sensor.h"
#include "JoystickDriver.c"

int waitTime = 0;

//Direction[] directions = new Direction[5];
//int[] waits = new int[5];

typedef enum {//List of directions our robot can go
	normal,
	opposite
} heading;

heading h = normal;

bool breakLoop = false;

void setHeading(heading hd) {
	h = hd;
} // set if we start at right or left

void setWaitTime(int time)
{
	waitTime = time;
}

int getTimings(int bNum) {
	switch(bNum) {
	case 0:
		return 115;
	case 2:
		return 165;
	case 3:
		return 155;
		break;
	case 4:
		return 155;
		break;
	default:
	  return 135;

	}
}

task p1
{
	tHTIRS2DSPMode _mode = DSP_600;
	Direction l;
	Direction r;

	if(h == normal)
	{
		r = right;
		l = left; //assign normal direction
	} else
	{
		r = left;
		l = right; // flip directions for starting on the opposite sides
	}
	int basketNum = 0; //check which basket we are at
	eraseDisplay();
	disableDiagnosticsDisplay();
	//int qqKillMe = getDirection(); Onw of the many functions I ave used to identify the IR seeker problem.
	//int IR = HTIRS2readACDir(HTIRS2);
	//	nxtDisplayTextLine(5, "%d", IR);
	wait1Msec((waitTime * 1000));
	while(HTIRS2readACDir(HTIRS2) != 5) {  // we are not in front of the beacon, and we haven't checked every basket
		PlaySound(soundBlip);
		move(l, 25);
		nxtDisplayTextLine(1, "%d", HTIRS2readACDir(HTIRS2));
		HTIRS2setDSPMode(HTIRS2, _mode);
		ClearTimer(T1);
		while(time10[T1] < getTimings(basketNum))  // Used to use just T1 as opposed to time1[T1], which is the correct function for reading timers.
		{
			if(HTIRS2readACDir(HTIRS2) == 5)
			{
				breakLoop = true;
				PlaySound(soundBeepBeep);
				break;
			}
			if(basketNum >= 3)
				{
					move(reverse, 25);
					wait10Msec(3);
				}
			//		wait10Msec(80);  // move more to compensate the middle piece
			//	nxtDisplayTextLine(0, "%d", IR);
			nxtDisplayTextLine(3, "%d", basketNum);
		}
		move(none, 0);    // stop and check if we are near
		wait10Msec(5);
		//		qqKillMe = getDirection();
		//	IR = HTIRS2readACDir(HTIRS2);
		move(none, 0);
		wait1Msec(250);   // Spin to adjust for Robot Drift
		move((l == left ? spin_left : spin_right), 25);
		wait1Msec(150);
		move(none, 0);
		wait1Msec(250);
		//nxtDisplayTextLine(1, "%d", HTIRS2readACDir(HTIRS2));
		basketNum++; //We checked this basket, move on or store our number, depending on if the basket has a beacon on it
		if(breakLoop && basketNum != 0)
		{
			wait1Msec(500);
			break;
		}
	}
	nxtDisplayTextLine(7, "BasketNum = %d", basketNum);
	// Procedural movements to drop off our block, and then alert ourselves that we have dropped off the block. //
	move(l, 25); //
	wait10Msec(4);
	move((l != left ? spin_left : spin_right), 25);
	wait1Msec(80);
	move(none,0);
	move(forward, 30);//Drive forward to deposit block
	wait10Msec((basketNum != 1 ? 50 * basketNum : 300 );//Pause to move, and adjust for backwards drift
	move(none, 0);//Stop the robot.
	servo[servo5] = 75;//Activate the arm to throw the block.
	PlaySound(soundBeepBeep); // signal that we found the I/R
	wait10Msec(50);//Pause.
	servo[servo5] = 255;//Bringing the arm back
	wait10Msec(50);//Wait for completion

	//Heading back to get out of the way of the baskets. //
	move(reverse, 30); // move back from going forward
	wait10Msec(125); //Pause to move
	move(none, 0);// Stop the robot
move((basketNum > 2 ? diagonal_F_L : diagonal_F_R), 35);
	wait10Msec(100);

	//(basketNum <= 2 ? basketNum : 5 - basketNum)  insert for i <, debugging stuffs.

for (int i = 0; i < (basketNum <= 2 ? basketNum : 5 - basketNum); i++)
	{
	move((basketNum <= 2 ? r : l), 25);
		PlaySound(soundBeepBeep);
		wait10Msec(getTimings(basketNum) + 50);
	}  // Move our robot left or right to prepare for getting on the ramp;
	move(none,0);
	move((l != left ? spin_left : spin_right), 25);
	wait1Msec(125);
move((basketNum > 2 ? none : diagonal_F_R), 35);
wait10Msec(200 - (basketNum > 2 ? 0 : 75));
	move(forward, 35);
	wait10Msec(basketNum > 2 ? 250 : 100);
	move((basketNum < 2 ? spin_right : spin_right), 25);
	wait1Msec(150);
  move((basketNum <= 2 ? l : none), 50);  // CHANGE VALUE FROM 60 TO NOT BREAK OUR AXLESESESES
	wait10Msec((basketNum <= 2 ? 200 : 450));
	move(none, 0);
  motor[motor5] = 100;
	motor[motor6] = 100;


	/*	directions[0] = forward;
	directions[1] = none;
	directions[2] = r;
	directions[3] = l;
	directions[4] = none;
	*/
	//move(forward, 35);  Finish LATERRER
	//wait10Msec(125);
	//move((basketNum > 2, r : l), 35)

}
