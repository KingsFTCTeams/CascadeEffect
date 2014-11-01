typedef struct
{
	TFileHandle   hFileHandle;        // will keep track of our file
	TFileIOResult nIOResult;          // will store our IO results
	string        sFileName;   				// the name of our file
	int           nFileSize;          // will store our file size
} tFile;

bool jointAnglesIdentical(tJointAngles angle1, tJointAngles angle2);
void readJointAnglesFromFile(tFile *file, tJointAngles *angles);

void writeJointAnglesToFile(tFile *file, tJointAngles angles)
{
	Delete(file->sFileName, file->nIOResult);

	// open the file for writing
  OpenWrite(file->hFileHandle, file->nIOResult, file->sFileName, file->nFileSize);
  WriteFloat(file->hFileHandle, file->nIOResult, angles.joint1);
  WriteFloat(file->hFileHandle, file->nIOResult, angles.joint2);
  WriteFloat(file->hFileHandle, file->nIOResult, angles.joint3);
  WriteFloat(file->hFileHandle, file->nIOResult, angles.joint4);
  WriteFloat(file->hFileHandle, file->nIOResult, angles.joint5);
  WriteFloat(file->hFileHandle, file->nIOResult, angles.joint6);
	// close the file (DON'T FORGET THIS STEP!)
  Close(file->hFileHandle, file->nIOResult);
  // begin data sanity check process
  //OpenRead(file->hFileHandle, file->nIOResult, file->sFileName, file->nFileSize);
  tJointAngles savedAngles;
  readJointAnglesFromFile(file, &savedAngles);
 	if(file->nIOResult != ioRsltSuccess || !jointAnglesIdentical(savedAngles, angles))
 	{
 		// bad things preform error reporting process
 		PlayImmediateTone(50, 2000);
 		writeDebugStreamLine("Error");
	}
	// close our file (DON'T FORGET THIS STEP!)
  //Close(file->hFileHandle, file->nIOResult);
}

bool jointAnglesIdentical(tJointAngles angle1, tJointAngles angle2)
{
	if(angle1.joint1 != angle2.joint1
		 || angle1.joint2 != angle2.joint2
		 || angle1.joint3 != angle2.joint3
		 || angle1.joint4 != angle2.joint4
		 || angle1.joint5 != angle2.joint5
		 || angle1.joint6 != angle2.joint6)
	{
		return false;
	}
	return true;
}

void writeJointAnglesToFile()
{

}

void readJointAnglesFromFile(tFile *file, tJointAngles *angles)
{
	OpenRead(file->hFileHandle, file->nIOResult, file->sFileName, file->nFileSize);
	ReadFloat(file->hFileHandle, file->nIOResult, angles->joint1);
	ReadFloat(file->hFileHandle, file->nIOResult, angles->joint2);
	ReadFloat(file->hFileHandle, file->nIOResult, angles->joint3);
	ReadFloat(file->hFileHandle, file->nIOResult, angles->joint4);
	ReadFloat(file->hFileHandle, file->nIOResult, angles->joint5);
	ReadFloat(file->hFileHandle, file->nIOResult, angles->joint6);
	Close(file->hFileHandle, file->nIOResult);
}


void readJointPoses()
{

}
