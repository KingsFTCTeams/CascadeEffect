#ifndef INDICATOR_H
#define INDICATOR_H

// These two must be replaced by their pragma...only here to allow compilation
#define initializeIndicator() { ResetIndicator(); }

#define Indicate() { 	LightsOff(); }
#define ResetIndicator() { 	LightsOn(); }

//#define LeftLightOn() motor[LeftLight]=100
//#define LeftLightOff() motor[LeftLight]=0
#define LeftLightOn() { }//pinMode(0, OUTPUT); digitalWrite(0, HIGH); }
#define LeftLightOff() { }//pinMode(0, OUTPUT); digitalWrite(0, LOW); }

//#define RightLightOn() motor[RightLight]=100
//#define RightLightOff() motor[RightLight]=0
#define RightLightOn() { }//pinMode(1, OUTPUT); digitalWrite(1, HIGH); }
#define RightLightOff() { }//pinMode(1, OUTPUT); digitalWrite(1, LOW); }

#define LightsOff() { LeftLightOff(); RightLightOff(); }
#define LightsOn() { LeftLightOn(); RightLightOn(); }

#endif // INDICATOR_H
