#pragma systemFile

#ifdef _NXT2excel_MaxData
#else
  #define _NXT2excel_MaxData 50
#endif

#define HiWord(x) ( (int)((x & 0xffff0000) >> 16) )
#define LoWord(x) ( (int)(x & 0xffff) )

#define IntPart(x) ( (int)x )
#define FractPart(x) ( (int)(((long)(x*10000))%10000) )

#define InvalidIndex(i) (i<0 || i>=_NXT2excel_MaxData)

// admin items are two longs (current timestamp and sample number)
#define numAdminItems 2
#define sizeOfEaAdminItem 4
#define adminDataSize (numAdminItems * sizeOfEaAdminItem)

long _NXT2Excel_CurrTime;
long _NXT2Excel_SampleNum = 0;
int  _NXT2Excel_Index = 8; // user data starts at index 8...admin info is 2 longs at beginning of buffer

ubyte _NXT2excel_Buffer[ adminDataSize + _NXT2excel_MaxData * 2];

bool _NXT2excelAddValue(int val, int index)
{
  if ( InvalidIndex(index) ) assert();
  //memcpy(_NXT2excel_Buffer[index],val,sizeof(val));
  _NXT2Excel_Index += sizeof(val);
  return true;
}

bool NXT2excelAddInt(int val)
{
  return _NXT2excelAddValue(val, _NXT2Excel_Index);
}

bool NXT2excelAddFloat(float val)
{
  NXT2excelAddInt( IntPart(val) );
  NXT2excelAddInt( FractPart(val) );
  return true;
}

bool NXT2excelAddLong(long val)
{
  NXT2excelAddInt( LoWord(val) );
  NXT2excelAddInt( HiWord(val) );
  return true;
}

void NXT2excelSendMessage()
{
  int nNumBytesToSend = _NXT2Excel_Index;

  // Update the admin values
  _NXT2Excel_CurrTime = nPgmTime;
  _NXT2Excel_SampleNum++;

  // Add the admin values to the buffer
  _NXT2Excel_Index = 0;                   // Put these at the start of the buffer
  NXT2excelAddLong(_NXT2Excel_CurrTime);  // [0] - [3]
  NXT2excelAddLong(_NXT2Excel_SampleNum); // [4] - [7]

  // Send them via bluetooth
  while (bBTBusy) wait1Msec(10);
 	cCmdMessageWriteToBluetooth(_NXT2excel_Buffer, nNumBytesToSend, mailbox1);
  while (nBluetoothCmdStatus==ioRsltCommPending) wait1Msec(2);

  // Start over in the buffer...back to the beginning (not counting the auto added admin stuff)!
  _NXT2Excel_Index = 8;
}
