int irFindColumn()
{
	int i = 0;
	if(ReadIR1() == 7)
	{
		i = 3;
	}
	else if(ReadIR1() == 5)
	{
		i = 1;
	}
	else if (ReadIR1() == 6)
	{
		i = 2;
	}
	return i;
}
