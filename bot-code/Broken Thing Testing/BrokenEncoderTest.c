#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     testMotor,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	nxtDisplayCenteredTextLine(4, "Place motor on");
	nxtDisplayCenteredTextLine(5, "stop press start");
	while(nNxtButtonPressed != 3)
	{

	}
	while(nNxtButtonPressed == 3)
	{

	}
	nMotorEncoder[testMotor] = 0;
	nxtDisplayCenteredTextLine(4, "Turn motor to");
	nxtDisplayCenteredTextLine(5, "stop press start");
	while(nNxtButtonPressed != 3)
	{

	}
	while(nNxtButtonPressed == 3)
	{

	}
	eraseDisplay();
	if(abs(nMotorEncoder[testMotor]) > 1340 && abs(nMotorEncoder[testMotor]) < 1380)
	{
		nxtDisplayCenteredTextLine(4, "Encoder Good!");
	}
	else
	{
		nxtDisplayCenteredTextLine(4, "Encoder Bad!");
	}
	nxtDisplayCenteredTextLine(5, "Ticks: %d", nMotorEncoder[testMotor]);
	while(true)
	{

	}
}
