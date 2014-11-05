#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#pragma systemFile

const float j1z = KINEMATICS_j1z;
const float j1x = KINEMATICS_j1x;
const float j2y = KINEMATICS_j2y;
const float j2z = KINEMATICS_j2z;
const float j3y = KINEMATICS_j3y;
const float j3z = KINEMATICS_j3z;
const float j4y = KINEMATICS_j4y;
const float j4z	= KINEMATICS_j4z;
const float j5x = KINEMATICS_j5x;
const float j5y = KINEMATICS_j5y;
const float j5z = KINEMATICS_j5z;

const float j1z_squared = KINEMATICS_j1z * KINEMATICS_j1z;
const float j1x_squared = KINEMATICS_j1x * KINEMATICS_j1x;

const float j2z_squared = KINEMATICS_j2z * KINEMATICS_j2z;
const float j2y_squared = KINEMATICS_j2y * KINEMATICS_j2y;

const float j3z_squared = KINEMATICS_j3z * KINEMATICS_j3z;
const float j3y_squared = KINEMATICS_j3y * KINEMATICS_j3y;

const float j4z_squared = KINEMATICS_j4z * KINEMATICS_j4z;
const float j4y_squared = KINEMATICS_j4y * KINEMATICS_j4y;

const float j5z_squared = KINEMATICS_j5z * KINEMATICS_j5z;
const float j5x_squared = KINEMATICS_j5x * KINEMATICS_j5x;
const float j5y_squared = KINEMATICS_j5y * KINEMATICS_j5y;

const float r2d = (180 / PI);
const float d2r = (PI / 180);

typedef struct {
	float fArray[4];
} vectorArrayStruct;

typedef struct {
	float fArray[4][4];
} fourByFourMatrixStruct;

typedef struct {
	float fArray[3][3];
} threeByThreeMatrixStruct;

typedef struct {
	float fArray[2][2];
} twoByTwoMatrixStruct;

void buildDH(dhStateMatrix *pDHout, float n, float theta, float d, float a, float alpha);
void matrixMatrixDot(dhStateMatrix matrix1, dhStateMatrix matrix2, dhStateMatrix *pOutputMatrix);
void bldToolTipFmStateMatrix(dhStateMatrix stateMatrix, tToolTip *pTP);
void matrixVectorDot(dhStateMatrix matrix, vectorArrayStruct vector, vectorArrayStruct *pOutputVector);
void dumpVector(vectorArrayStruct vector);
float calculateDet3by3(threeByThreeMatrixStruct matrix);
float calculateDet4by4(fourByFourMatrixStruct matrix);
float calculateDet2by2(twoByTwoMatrixStruct matrix);
void dumpStateMatrix(dhStateMatrix matrix, const string s);
void dump4by4Matirx(fourByFourMatrixStruct matrix);
void adjoinMatrix(fourByFourMatrixStruct *pMatrix);
void dotNumberWithMatrix(fourByFourMatrixStruct *pMatrix, float determinate);
void invert4x4HomogeneousMatrix(dhStateMatrix inputMatrix, dhStateMatrix *pMatrix);
void bldStateMatrixFmToolTip(dhStateMatrix *pMatrix, tToolTip point);
void SafeMemcpy(void *pToBuffer, const void *pFromBuffer, const short nNumOfBytes);
void dumpToolTip(tToolTip tp, const string s);
void dumpJointAngles(tJointAngles a, const string s);
void dumpArmJointGoals(const string s);

/////////////////////////////////////////////////////////////////////////////////////
void invert4x4HomogeneousMatrix(dhStateMatrix inputMatrix,  dhStateMatrix *pMatrix) {
	//LOOK HERE FOR ERRORS
	dhStateMatrix tmpMatrix;
	dhStateMatrix tmpTransMatrix;
	SafeMemcpy(tmpMatrix, inputMatrix, sizeof(dhStateMatrix));

  pMatrix->dhArray[0][0] = tmpMatrix.dhArray[0][0];
  pMatrix->dhArray[1][1] = tmpMatrix.dhArray[1][1];
  pMatrix->dhArray[2][2] = tmpMatrix.dhArray[2][2];

  pMatrix->dhArray[0][1] = tmpMatrix.dhArray[1][0];
  pMatrix->dhArray[0][2] = tmpMatrix.dhArray[2][0];
  pMatrix->dhArray[1][0] = tmpMatrix.dhArray[0][1];
  pMatrix->dhArray[1][2] = tmpMatrix.dhArray[2][1];
  pMatrix->dhArray[2][0] = tmpMatrix.dhArray[0][2];
  pMatrix->dhArray[2][1] = tmpMatrix.dhArray[1][2];

  SafeMemcpy(tmpTransMatrix, pMatrix, sizeof(dhStateMatrix));

  /*col4[0] = -(mTranspose[0][0] * InMatrix_4x4[0][3] + mTranspose[0][1] *
  							InMatrix_4x4[1][3] + mTranspose[0][2] * InMatrix_4x4[2][3]);
    col4[1] = -(mTranspose[1][0] * InMatrix_4x4[0][3] + mTranspose[1][1] *
    						InMatrix_4x4[1][3] + mTranspose[1][2] * InMatrix_4x4[2][3]);
    col4[2] = -(mTranspose[2][0] * InMatrix_4x4[0][3] + mTranspose[2][1] *
    						InMatrix_4x4[1][3] + mTranspose[2][2] * InMatrix_4x4[2][3]);
  */

  pMatrix->dhArray[0][3] = -(tmpTransMatrix.dhArray[0][0] * tmpMatrix.dhArray[0][3] + tmpTransMatrix.dhArray[0][1] *
  							tmpMatrix.dhArray[1][3] + tmpTransMatrix.dhArray[0][2] * tmpMatrix.dhArray[2][3]);
  pMatrix->dhArray[1][3] = -(tmpTransMatrix.dhArray[1][0] * tmpMatrix.dhArray[0][3] + tmpTransMatrix.dhArray[1][1] *
  						tmpMatrix.dhArray[1][3] + tmpTransMatrix.dhArray[1][2] * tmpMatrix.dhArray[2][3]);
  pMatrix->dhArray[2][3] = -(tmpTransMatrix.dhArray[2][0] * tmpMatrix.dhArray[0][3] + tmpTransMatrix.dhArray[2][1] *
  						tmpMatrix.dhArray[1][3] + tmpTransMatrix.dhArray[2][2] * tmpMatrix.dhArray[2][3]);


	pMatrix->dhArray[3][0] = 0.0;
	pMatrix->dhArray[3][1] = 0.0;
	pMatrix->dhArray[3][2] = 0.0;
	pMatrix->dhArray[3][3] = 1.0;

}

/////////////////////////////////////////////////////////////////////////////////////
void dotNumberWithMatrix(fourByFourMatrixStruct *pMatrix, float determinate) {
	fourByFourMatrixStruct tmpMatrix;
	SafeMemcpy(tmpMatrix, pMatrix, sizeof(fourByFourMatrixStruct));
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j ++) {
			pMatrix->fArray[i][j] = determinate * tmpMatrix.fArray[i][j];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void dumpStateMatrix(dhStateMatrix matrix, const string s) {
	writeDebugStreamLine("%s:", s);
	writeDebugStreamLine("{  %f,  %f,  %f,  %f  }", matrix.dhArray[0][0],
												matrix.dhArray[0][1], matrix.dhArray[0][2],
												matrix.dhArray[0][3]);
	writeDebugStreamLine("{  %f,  %f,  %f,  %f  }", matrix.dhArray[1][0],
												matrix.dhArray[1][1], matrix.dhArray[1][2],
												matrix.dhArray[1][3]);
	writeDebugStreamLine("{  %f,  %f,  %f,  %f  }", matrix.dhArray[2][0],
												matrix.dhArray[2][1], matrix.dhArray[2][2],
												matrix.dhArray[2][3]);
	writeDebugStreamLine("{  %f,  %f,  %f,  %f  }", matrix.dhArray[3][0],
												matrix.dhArray[3][1], matrix.dhArray[3][2],
												matrix.dhArray[3][3]);
	writeDebugStreamLine("");
}

/////////////////////////////////////////////////////////////////////////////////////
void dump4by4Matirx(fourByFourMatrixStruct matrix) {
	writeDebugStreamLine("{  %f,  %f,  %f,  %f  }", matrix.fArray[0][0],
												matrix.fArray[0][1], matrix.fArray[0][2],
												matrix.fArray[0][3]);
	writeDebugStreamLine("{  %f,  %f,  %f,  %f  }", matrix.fArray[1][0],
												matrix.fArray[1][1], matrix.fArray[1][2],
												matrix.fArray[1][3]);
	writeDebugStreamLine("{  %f,  %f,  %f,  %f  }", matrix.fArray[2][0],
												matrix.fArray[2][1], matrix.fArray[2][2],
												matrix.fArray[2][3]);
	writeDebugStreamLine("{  %f,  %f,  %f,  %f  }", matrix.fArray[3][0],
												matrix.fArray[3][1], matrix.fArray[3][2],
												matrix.fArray[3][3]);
	writeDebugStreamLine("");
}

/////////////////////////////////////////////////////////////////////////////////////
void dumpVector(vectorArrayStruct vector) {
	writeDebugStreamLine("{  %f   }", vector.fArray[0]);
	writeDebugStreamLine("{  %f   }", vector.fArray[1]);
	writeDebugStreamLine("{  %f   }", vector.fArray[2]);
	writeDebugStreamLine("{  %f,  }", vector.fArray[3]);
	writeDebugStreamLine("");
}

/////////////////////////////////////////////////////////////////////////////////////
void buildDH(dhStateMatrix *pDHout, float n, float theta, float d, float a, float alpha) {
	theta = theta * (PI/180);
	alpha = alpha * (PI/180);
	pDHout->dhArray[0][0] = cos(theta);
	pDHout->dhArray[1][0] = sin(theta);
	pDHout->dhArray[2][0] = 0.0;
	pDHout->dhArray[3][0] = 0.0;
	pDHout->dhArray[0][1] = -cos(alpha)*sin(theta);
	pDHout->dhArray[1][1] = cos(alpha)*cos(theta);
	pDHout->dhArray[2][1] = sin(alpha);
	pDHout->dhArray[3][1] = 0.0;
	pDHout->dhArray[0][2] = sin(alpha)*sin(theta);
	pDHout->dhArray[1][2] = sin(alpha) * -cos(theta);
	pDHout->dhArray[2][2] = cos(alpha);
	pDHout->dhArray[3][2] = 0.0;
	pDHout->dhArray[0][3] = a * cos(theta);
	pDHout->dhArray[1][3] = a * sin(theta);
	pDHout->dhArray[2][3] = d;
	pDHout->dhArray[3][3] = 1.0;
}

/////////////////////////////////////////////////////////////////////////////////////
void bldToolTipFmStateMatrix(dhStateMatrix stateMatrix, tToolTip *pTP) {
	pTP->rz = atan2(stateMatrix.dhArray[0][1],stateMatrix.dhArray[0][0]) * (-180/PI);
	pTP->rx = atan2(stateMatrix.dhArray[1][2],stateMatrix.dhArray[2][2]) * (-180/PI);
	pTP->ry = atan2(stateMatrix.dhArray[0][2],(sqrt(pow(stateMatrix.dhArray[1][2], 2) + pow(stateMatrix.dhArray[2][2], 2)))) * (180/PI);
	pTP->x = stateMatrix.dhArray[0][3];
	pTP->y = stateMatrix.dhArray[1][3];
	pTP->z = stateMatrix.dhArray[2][3];
}

/////////////////////////////////////////////////////////////////////////////////////
void createRyStateMatrix(dhStateMatrix *stateMatix, float ry) {
	ry = ry * (PI/180);
	stateMatix->dhArray[0][0] = cos(ry);
	stateMatix->dhArray[0][1] = 0.0;
	stateMatix->dhArray[0][2] = sin(ry);
	stateMatix->dhArray[1][0] = 0.0;
	stateMatix->dhArray[1][1] = 1.0;
	stateMatix->dhArray[1][2] = 0.0;
	stateMatix->dhArray[2][0] = -sin(ry);
	stateMatix->dhArray[2][1] = 0.0;
	stateMatix->dhArray[2][2] = cos(ry);
	stateMatix->dhArray[0][3] = 0.0;
	stateMatix->dhArray[1][3] = 0.0;
	stateMatix->dhArray[2][3] = 0.0;
	stateMatix->dhArray[3][3] = 1.0;
	stateMatix->dhArray[3][0] = 0.0;
	stateMatix->dhArray[3][1] = 0.0;
	stateMatix->dhArray[3][2] = 0.0;
}

/////////////////////////////////////////////////////////////////////////////////////
void matrixVectorDot(dhStateMatrix matrix, vectorArrayStruct vector, vectorArrayStruct *pOutputVector) {
	dhStateMatrix tmpMatrix;
	vectorArrayStruct tmpVector;
	SafeMemcpy(tmpMatrix, matrix, sizeof(dhStateMatrix));
	SafeMemcpy(tmpVector, vector, sizeof(vectorArrayStruct));
	for(int i = 0; i < 4; i++) {
		pOutputVector->fArray[i] = 0.0;
		for(int j = 0; j < 4; j++) {
			pOutputVector->fArray[i] += tmpMatrix.dhArray[i][j] * tmpVector.fArray[j];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void bldStateMatrixFmToolTip(dhStateMatrix *pMatrix, tToolTip point) {
	tToolTip pt;
	SafeMemcpy(pt, point, sizeof(tToolTip));
	pt.rz = point.rz * (PI/180);
	pt.ry = point.ry * (PI/180);
	pt.rx = point.rx * (PI/180);
	pMatrix->dhArray[0][0] =  cos(pt.ry) * cos(pt.rz);
	pMatrix->dhArray[0][1] = -cos(pt.ry) * sin(pt.rz);
	pMatrix->dhArray[0][2] =  sin(pt.ry);
	pMatrix->dhArray[0][3] = pt.x;
	pMatrix->dhArray[1][0] =  cos(pt.rx) * sin(pt.rz) + cos(pt.rz) * sin(pt.rx) * sin(pt.ry);
	pMatrix->dhArray[1][1] =  cos(pt.rx) * cos(pt.rz) - sin(pt.rx) * sin(pt.ry) * sin(pt.rz);
	pMatrix->dhArray[1][2] = -cos(pt.ry) * sin(pt.rx);
	pMatrix->dhArray[1][3] = pt.y;
	pMatrix->dhArray[2][0] = sin(pt.rx) * sin(pt.rz) - cos(pt.rx) * cos(pt.rz) * sin(pt.ry);
	pMatrix->dhArray[2][1] = cos(pt.rz) * sin(pt.rx) + cos(pt.rx) * sin(pt.ry) * sin(pt.rz);
	pMatrix->dhArray[2][2] = cos(pt.rx) * cos(pt.ry);
	pMatrix->dhArray[2][3] = pt.z;
	pMatrix->dhArray[3][0] = 0.0;
	pMatrix->dhArray[3][1] = 0.0;
	pMatrix->dhArray[3][2] = 0.0;
	pMatrix->dhArray[3][3] = 1.0;
}

/////////////////////////////////////////////////////////////////////////////////////
void matrixMatrixDot(dhStateMatrix matrix1, dhStateMatrix matrix2,
											dhStateMatrix *pOutputMatrix) {
	/*for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			pOutputMatrix->dhArray[i][j] = 0.0;
			for(int k = 0; k < 4; k++) {
				pOutputMatrix->dhArray[i][j] += matrix1.dhArray[i][k] * dhArrayContainer2Tmp.dhArray[k][j];
			}
		}
	}*/
	dhStateMatrix matrixTmp1;
	dhStateMatrix matrixTmp2;
	SafeMemcpy(matrixTmp1, matrix1, sizeof(matrixTmp1));
	SafeMemcpy(matrixTmp2, matrix2, sizeof(matrixTmp2));
	//1st row: (3 mult 2 add) * 4 + 1 add = 12 mult, 9 add total
   pOutputMatrix->dhArray[0][0] = matrixTmp1.dhArray[0][0]*matrixTmp2.dhArray[0][0] + matrixTmp1.dhArray[0][1]*matrixTmp2.dhArray[1][0] + matrixTmp1.dhArray[0][2]*matrixTmp2.dhArray[2][0]; //matrixTmp2.dhArray[3][0] is always 0
   pOutputMatrix->dhArray[0][1] = matrixTmp1.dhArray[0][0]*matrixTmp2.dhArray[0][1] + matrixTmp1.dhArray[0][1]*matrixTmp2.dhArray[1][1] + matrixTmp1.dhArray[0][2]*matrixTmp2.dhArray[2][1]; //matrixTmp2.dhArray[3][1] is always 0
   pOutputMatrix->dhArray[0][2] = matrixTmp1.dhArray[0][0]*matrixTmp2.dhArray[0][2] + matrixTmp1.dhArray[0][1]*matrixTmp2.dhArray[1][2] + matrixTmp1.dhArray[0][2]*matrixTmp2.dhArray[2][2]; //matrixTmp2.dhArray[3][2] is always 0
   pOutputMatrix->dhArray[0][3] = matrixTmp1.dhArray[0][0]*matrixTmp2.dhArray[0][3] + matrixTmp1.dhArray[0][1]*matrixTmp2.dhArray[1][3] + matrixTmp1.dhArray[0][2]*matrixTmp2.dhArray[2][3] + matrixTmp1.dhArray[0][3];  //matrixTmp2.dhArray[3][3] is always 1

   //2nd row:  (3 mult 2 add) * 4 + 1 add = 12 mult, 9 add total
   pOutputMatrix->dhArray[1][0] = matrixTmp1.dhArray[1][0]*matrixTmp2.dhArray[0][0] + matrixTmp1.dhArray[1][1]*matrixTmp2.dhArray[1][0] + matrixTmp1.dhArray[1][2]*matrixTmp2.dhArray[2][0]; //matrixTmp2.dhArray[3][0] is always 0
   pOutputMatrix->dhArray[1][1] = matrixTmp1.dhArray[1][0]*matrixTmp2.dhArray[0][1] + matrixTmp1.dhArray[1][1]*matrixTmp2.dhArray[1][1] + matrixTmp1.dhArray[1][2]*matrixTmp2.dhArray[2][1]; //matrixTmp2.dhArray[3][1] is always 0
   pOutputMatrix->dhArray[1][2] = matrixTmp1.dhArray[1][0]*matrixTmp2.dhArray[0][2] + matrixTmp1.dhArray[1][1]*matrixTmp2.dhArray[1][2] + matrixTmp1.dhArray[1][2]*matrixTmp2.dhArray[2][2]; //matrixTmp2.dhArray[3][2] is always 0
   pOutputMatrix->dhArray[1][3] = matrixTmp1.dhArray[1][0]*matrixTmp2.dhArray[0][3] + matrixTmp1.dhArray[1][1]*matrixTmp2.dhArray[1][3] + matrixTmp1.dhArray[1][2]*matrixTmp2.dhArray[2][3] + matrixTmp1.dhArray[1][3];             //matrixTmp2.dhArray[3][3] is always 1


   //3rd row: (3 mult 2 add) * 4 + 1 add = 12 mult, 9 add total
   pOutputMatrix->dhArray[2][0] = matrixTmp1.dhArray[2][0]*matrixTmp2.dhArray[0][0] + matrixTmp1.dhArray[2][1]*matrixTmp2.dhArray[1][0] + matrixTmp1.dhArray[2][2]*matrixTmp2.dhArray[2][0]; //matrixTmp2.dhArray[3][0] is always 0
   pOutputMatrix->dhArray[2][1] = matrixTmp1.dhArray[2][0]*matrixTmp2.dhArray[0][1] + matrixTmp1.dhArray[2][1]*matrixTmp2.dhArray[1][1] + matrixTmp1.dhArray[2][2]*matrixTmp2.dhArray[2][1]; //matrixTmp2.dhArray[3][1] is always 0
   pOutputMatrix->dhArray[2][2] = matrixTmp1.dhArray[2][0]*matrixTmp2.dhArray[0][2] + matrixTmp1.dhArray[2][1]*matrixTmp2.dhArray[1][2] + matrixTmp1.dhArray[2][2]*matrixTmp2.dhArray[2][2]; //matrixTmp2.dhArray[3][2] is always 0
   pOutputMatrix->dhArray[2][3] = matrixTmp1.dhArray[2][0]*matrixTmp2.dhArray[0][3] + matrixTmp1.dhArray[2][1]*matrixTmp2.dhArray[1][3] + matrixTmp1.dhArray[2][2]*matrixTmp2.dhArray[2][3] + matrixTmp1.dhArray[2][3];             //matrixTmp2.dhArray[3][3] is always 1

   //4th row: no math
   pOutputMatrix->dhArray[3][0] = 0.0;
   pOutputMatrix->dhArray[3][1] = 0.0;
   pOutputMatrix->dhArray[3][2] = 0.0;
   pOutputMatrix->dhArray[3][3] = 1.0;
}

/////////////////////////////////////////////////////////////////////////////////////
float calculateDet2by2(twoByTwoMatrixStruct matrix) {
	return (matrix.fArray[0][0] * matrix.fArray[1][1]) - (matrix.fArray[0][1] * matrix.fArray[1][0]);
}

/////////////////////////////////////////////////////////////////////////////////////
float calculateDet3by3(threeByThreeMatrixStruct matrix) {
	return (matrix.fArray[0][0] * ((matrix.fArray[1][1] * matrix.fArray[2][2]) - (matrix.fArray[2][1] * matrix.fArray[1][2]))) -
					 (matrix.fArray[0][1] * ((matrix.fArray[1][0] * matrix.fArray[2][2]) - (matrix.fArray[2][0] * matrix.fArray[1][2]))) +
					 (matrix.fArray[0][2] * ((matrix.fArray[1][0] * matrix.fArray[2][1]) - (matrix.fArray[2][0] * matrix.fArray[1][1])));
}

/////////////////////////////////////////////////////////////////////////////////////
/*float calculateDet4by4(fourByFourMatrixStruct matrix) {
	return (matrix.fArray[0][0] *
		((matrix.fArray[1][1] * ((matrix.fArray[2][2] * matrix.fArray[3][3]) - (matrix.fArray[3][2] * matrix.fArray[2][3]))) -
		(matrix.fArray[1][2] * ((matrix.fArray[2][1] * matrix.fArray[3][3]) - (matrix.fArray[3][1] * matrix.fArray[2][3]))) +
		(matrix.fArray[1][3] * ((matrix.fArray[2][1] * matrix.fArray[3][2]) - (matrix.fArray[3][1] * matrix.fArray[2][2])))))
		-
		(matrix.fArray[0][1] *
		((matrix.fArray[1][0] * ((matrix.fArray[2][2] * matrix.fArray[3][3]) - (matrix.fArray[3][2] * matrix.fArray[2][3]))) -
		(matrix.fArray[1][2] * ((matrix.fArray[2][0] * matrix.fArray[3][3]) - (matrix.fArray[3][0] * matrix.fArray[2][3]))) +
		(matrix.fArray[1][3] * ((matrix.fArray[2][0] * matrix.fArray[3][2]) - (matrix.fArray[3][0] * matrix.fArray[2][2])))))
		+
		(matrix.fArray[0][2] *
		((matrix.fArray[1][0] * ((matrix.fArray[2][1] * matrix.fArray[3][3]) - (matrix.fArray[3][1] * matrix.fArray[2][3]))) -
		(matrix.fArray[1][1] * ((matrix.fArray[2][0] * matrix.fArray[3][3]) - (matrix.fArray[3][0] * matrix.fArray[2][3]))) +
		(matrix.fArray[1][3] * ((matrix.fArray[2][0] * matrix.fArray[3][1]) - (matrix.fArray[3][0] * matrix.fArray[2][1])))))
		-
		(matrix.fArray[0][3] *
		((matrix.fArray[1][0] * ((matrix.fArray[2][1] * matrix.fArray[3][2]) - (matrix.fArray[3][1] * matrix.fArray[2][2]))) -
		(matrix.fArray[1][1] * ((matrix.fArray[2][0] * matrix.fArray[3][2]) - (matrix.fArray[3][0] * matrix.fArray[2][2]))) +
		(matrix.fArray[1][2] * ((matrix.fArray[2][0] * matrix.fArray[3][1]) - (matrix.fArray[3][0] * matrix.fArray[2][1])))));
}*/

/////////////////////////////////////////////////////////////////////////////////////
/*void adjoinMatrix(fourByFourMatrixStruct *pMatrix) {
	fourByFourMatrixStruct tmpMatrix;
	SafeMemcpy(tmpMatrix, pMatrix, sizeof(tmpMatrix));
	pMatrix->fArray[0][0] =
										(tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][1]) +
										(tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][1]);
	pMatrix->fArray[0][1] =
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][2]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][1]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][1]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][2]);
	pMatrix->fArray[0][2] =
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[3][1]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[3][1]);
	pMatrix->fArray[0][3] =
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][2]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][3]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][1]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][3]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][1]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][2]);
	pMatrix->fArray[1][0] =
										(tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][2]) +
										(tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][2]);
	pMatrix->fArray[1][1] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][0]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][0]);
	pMatrix->fArray[1][2] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[3][2]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[3][2]);
	pMatrix->fArray[1][3] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][3]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][0]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][2]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][2]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][3]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][0]);
	pMatrix->fArray[2][0] =
										(tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][0]) +
										(tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][1]) -
										(tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][1]) -
										(tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][0]);
	pMatrix->fArray[2][1] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][1]) +
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[2][3]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][1]);
	pMatrix->fArray[2][2] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[3][3]) +
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[3][0]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[3][1]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[3][1]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[3][3]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[3][0]);
	pMatrix->fArray[2][3] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][1]) +
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][3]) +
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][0]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][3]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][3]*tmpMatrix.fArray[2][0]) -
										(tmpMatrix.fArray[0][3]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][1]);
	pMatrix->fArray[3][0] =  //possible error
										(tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][1]) +
										(tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][2]) +
										(tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][1]);
	pMatrix->fArray[3][1] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][2]) +
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][0]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][1]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[2][2]*tmpMatrix.fArray[3][1]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[2][0]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[2][1]*tmpMatrix.fArray[3][0]);
	pMatrix->fArray[3][2] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[3][1]) +
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[3][2]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[3][2]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[3][0]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[3][1]);
	pMatrix->fArray[3][3] =
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][2]) +
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][0]) +
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][1]) -
										(tmpMatrix.fArray[0][0]*tmpMatrix.fArray[1][2]*tmpMatrix.fArray[2][1]) -
										(tmpMatrix.fArray[0][1]*tmpMatrix.fArray[1][0]*tmpMatrix.fArray[2][2]) -
										(tmpMatrix.fArray[0][2]*tmpMatrix.fArray[1][1]*tmpMatrix.fArray[2][0]);
}*/

void SafeMemcpy(void *pToBuffer, const void *pFromBuffer, const short nNumOfBytes)
{
	hogCPU();
	memcpy(pToBuffer, pFromBuffer, nNumOfBytes);
	releaseCPU();
}

#endif
