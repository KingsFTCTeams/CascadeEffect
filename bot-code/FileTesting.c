task main()
{
  TFileHandle   hFileHandle;              // will keep track of our file
  TFileIOResult nIOResult;                // will store our IO results
  string        sFileName = "test.txt";   // the name of our file
  int           nFileSize = 100;          // will store our file size

 	long writeTime = 0;
 	long readTime = 0;
 	long programTime = 0;

  Delete("test.txt",nIOResult);
  OpenWrite(hFileHandle, nIOResult, sFileName, nFileSize);    // open the file for writing (creates the file if it does not exist)
  WriteLong(hFileHandle, nIOResult, 23293842);
  //WriteByte(hFileHandle, nIOResult, CR);                      // write 'CR' to the file (carriage return)
  //WriteByte(hFileHandle, nIOResult, LF);                      // write 'LF' to the file (line feed)
  WriteLong(hFileHandle, nIOResult, 75822323);
  //WriteByte(hFileHandle, nIOResult, CR);                      // write 'CR' to the file (carriage return)
  //WriteByte(hFileHandle, nIOResult, LF);
  Close(hFileHandle, nIOResult);                              // close the file (DON'T FORGET THIS STEP!)
 	writeTime = (nPgmTime);

  OpenRead(hFileHandle, nIOResult, sFileName, nFileSize);     // open our file 'sFileName' for reading
 	if(nIOResult == ioRsltSuccess)
 	{
 		writeDebugStreamLine("SUCCESS");
	}
  long result1 = 0;
  long result2 = 0;
  for(int i = 0; i < nFileSize; i++)                          // iterate through the file until we've hit the end:
  {
  	ReadLong(hFileHandle, nIOResult, result1);
  	ReadLong(hFileHandle, nIOResult, result2);
  }
  Close(hFileHandle, nIOResult);                              // close our file (DON'T FORGET THIS STEP!)
 	readTime = (nPgmTime - writeTime);
 	programTime = nPgmTime;
  writeDebugStreamLine("Number 1: %d Number 2: %d", result1, result2);
  writeDebugStreamLine("Write Time: %d Read Time: %d Total Time: %d", writeTime, readTime, programTime);


  while(true);
}
