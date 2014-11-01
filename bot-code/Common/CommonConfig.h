#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// BUILD VERSION - BUMP THIS WITH CODE CHANGES...
//
//              MINOR CHANGES: INCREMENT THE DECIMAL PORTION
//              MAJOR CHANGES: INCREMENT THE INTEGER PORTION
//
#define BUILD_VERS 6.00

  //-------------------------------------------------------------------------------------------
  // Minimum required battery voltages
  //
  #define MIN_REQ_EXT_BATTERY_VOLTAGE 12800
  #define MIN_REQ_NXT_BATTERY_VOLTAGE 7000

  //-------------------------------------------------------------------------------------------
  // Sensor access and init ... these do not generally change ... enabled or disabled with
	// the sensor usage flags found in TeamConfig.h
  //
	// To activate a sensor:
	//	1) add the sensor to the pragma list via the Motors and Sensors Setup wizard.
	//		 * If the sensor is on the brick you may name it whatever you want...
	// 		 * If the sensor is out on a SMUX, you must use specific sensor access name:
	//					msensor_<NXT port>_<SMUX port>
	//					where <NXT port> is S1 for port 1 or S4 for port 4
	//					and <SMUX port> is 1 for port 1 or 3 for port 3
	//					Example: SMUX connected to NXT port 2, sensor on SMUX port 4: msensor_S2_3
	//		 * The sensor types are:
	//					- Any sensor on a SMUX: sensorI2CCustom
	//					- Otherwise if directly plugged into the NXT:
	//							+ Gyro: sensorAnalogInactive
	//				  		+ IR: sensorI2CCustom
	//				  		+ Accel: sensorI2CCustom
	//							+ Mag: sensorAnalogInactive
	//							+ US: sensorI2CCustom
	//
	//	2) Set the sensor name below.
	//		 * Example: adding gyro named GyroSensor ... set the GYRO #define value to GyroSensor
	//
	//	3) Uncomment the appropriate "using" #define in TeamConfig.h
	//

	// tested sensorAnalogInactive
	//comment
	#ifdef USING_GYRO
		#define GYRO     GyroSensor
	  #define GYRO_LOC ON_BRICK // ON_BRICK or ON_SMUX
	  #define InitGYRO_BRICK() { }
	  #define InitGYRO_SMUX()  { }
	  #define InitGYRO()       InitGYRO_BRICK()
	  #define ReadGYRO()       SensorValue[GyroSensor]
	#else
		#define GYRO     0
	  #define GYRO_LOC NOT_USED
	  #define InitGYRO_BRICK()  { }
	  #define InitGYRO_SMUX()   { }
	  #define InitGYRO()        { }
	  #define ReadGYRO()    		0
	#endif

	#ifdef USING_SUPER_PRO
		#define SUPER_PRO     HTSPB
	  #define SUPER_PRO_LOC ON_BRICK // ON_BRICK or ON_SMUX
	#else
		#define SUPER_PRO     0
	  #define SUPER_PRO_LOC NOT_USED // ON_BRICK or ON_SMUX
	#endif

	// tested sensorI2CCustom
	#ifdef USING_IR1
		#define IR1 IRsensor1
		#define InitIR1() { eraseDisplay(); 																	 \
												while (HTIRS2setDSPMode(IR1, DSP_1200) == false)   \
												{  																								 \
													nxtDisplayCenteredTextLine(4, "Plug In The IR!"); \
												}																									 \
												eraseDisplay();																		 \
											}
		#define ReadIR1() HTIRS2readACDir(IR1)
	#else
		#define IR1 0
		#define ReadIR1() 0
		#define InitIR1() { }
	#endif

	// tested sensorI2CCustom
	#ifdef USING_IR2
		#define IR2 IRsensor2
		#define InitIR2() { eraseDisplay(); 																	 \
												while (HTIRS2setDSPMode(IR2, DSP_1200) == false)   \
												{  																								 \
													nxtDisplayCenteredTextLine(4, "Plug In The IR!"); \
												}																									 \
												eraseDisplay();																		 \
											}
		#define ReadIR2() HTIRS2readACDir(IR2)
	#else
		#define IR2 0
		#define ReadIR2() 0
		#define InitIR2() { }
	#endif

	// tested sensorI2CCustom
	#ifdef USING_ACCEL
		#define ACCEL AccessSensor
		#define InitAccel() {  }
		#define ReadAccel() { HTACreadAllAxes(ACCEL, _nAccelSensorXValue, _nAccelSensorYValue,_nAccelSensorZValue); }
	#else
		#define ACCEL 0
		#define InitAccel() {  }
		#define ReadAccel() { }
	#endif

	// tested sensorAnalogInactive
	#ifdef USING_MAG
		#define MAG MagneticSensor
		#define InitMAG() { HTMAGstartCal(MAG); }
	  #define ReadMAG() HTMAGreadVal(MAG)

	#else
		#define MAG 0
		#define InitMAG() {  }
		#define ReadMAG() 0
	#endif

	// tested sensorI2CCustom
	#ifdef USING_US1
		#define US1 USsensor1
		#define InitUS1() {  }
		#define ReadUS1() USreadDist(US1)

	#else
		#define US1 0
		#define InitUS1() {  }
		#define ReadUS1() 0
	#endif

	// tested sensorI2CCustom
	#ifdef USING_US2
		#define US2 USsensor2
		#define InitUS2() {  }
		#define ReadUS2() USreadDist(US2)

	#else
		#define US2 0
		#define InitUS2() {  }
		#define ReadUS2() 0
	#endif

	//-------------------------------------------------------------------------------------------
  // Encoder spike protection settings...
	//
	#define MAX_VALID_ENCODER_DELTA (1.30 * 35 * ENCODER_TASK_SAMPLE_RATE) // 152RPM -> 3.648 encoder ticks/ms with 30% buffer

#endif // COMMON_CONFIG_H
