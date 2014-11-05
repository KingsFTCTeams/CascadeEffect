#include "../Common/Hitech Drivers/hitechnic-superpro.h"

typedef enum
{
	INPUT,
	OUTPUT
} tPinModes;

typedef enum
{
	HIGH,
	LOW
} tPinStates;

typedef enum
{
	A00,
	A01
} tAnalogWritePins;

ubyte digitalPinStates;

///////////////////////////////////////////////////////////////////////////////////////////////
//analog write works on pins 00 and 01 only takes a duty cycle from 0-8193
//and a voltage from 0-1023 also I think there are diffrent wave types
//however we defualt to SQUARE
//
void analogWrite(tAnalogWritePins pin, int dutyCycle, int voltage, byte mode = DAC_MODE_SQUAREWAVE)
{
	if(pin == A00)
	{
		HTSPBwriteAnalog(SUPER_PRO, HTSPB_DACO0, mode, dutyCycle, voltage);
	}
	else if(pin == A01)
	{
		HTSPBwriteAnalog(SUPER_PRO, HTSPB_DACO1, mode, dutyCycle, voltage);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Required to use digital pins
//Sets the pin mode for digital pins to either input or output... Must be called
//once for each pin.
//
void pinMode(int pin, tPinModes pinMode)
{
	if(pinMode == INPUT)
	{
		//b = (byte) (b & ~(1 << 5)); 6 bit to 0
		digitalPinStates = (byte)(digitalPinStates & ~(1 << pin));
		HTSPBsetupIO(SUPER_PRO, digitalPinStates);
	}
	else if(pinMode == OUTPUT)
	{
		//b = (byte)(b | (1 << 6)) 7 bit to 1
		digitalPinStates = (byte)(digitalPinStates | (1 << pin));
		HTSPBsetupIO(SUPER_PRO, digitalPinStates);
	}
}

////////////////////////////////////////////////////////////////////////////
//Simple digital write takes the physical pin number and the
//state either high or low
//
void digitalWrite(int pin, tPinStates pinState)
{
	if(pinState == LOW)
	{
		//b = (byte) (b & ~(1 << 5)); 6 bit to 0
		digitalPinStates = (byte)(digitalPinStates & ~(1 << pin));
		HTSPBwriteIO(SUPER_PRO, digitalPinStates);
	}
	else if(pinState == HIGH)
	{
		//b = (byte)(b | (1 << 6)) 7 bit to 1
		digitalPinStates = (byte)(digitalPinStates | (1 << pin));
		HTSPBwriteIO(SUPER_PRO, digitalPinStates);
	}
}

//////////////////////////////////////////////////////////////////////////
//Reads a digital pin... Takes the pysical pin number
//and returns the state as either a 1 or a 0
//
byte digitalRead(int pin)
{
	ubyte tmpByte = 0;
	tmpByte = (byte)(tmpByte | (1 << pin));
	return HTSPBreadIO(SUPER_PRO, tmpByte);
}

//////////////////////////////////////////////////////////////////////////
//Reads an analog pin... Takes the physical pin number and returns
//an intiger between 0-1023
//
int analogRead(int pin)
{
	return HTSPBreadADC(SUPER_PRO, pin, 10);
}
