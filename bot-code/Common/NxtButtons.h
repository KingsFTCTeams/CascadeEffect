#pragma systemFile

void WaitForNxtButtonRelease()
{
  TButtons btn;
  while (true) // wait for button release
  {
  	btn = nNxtButtonPressed;
  	if (btn == -1)
  	  break;
  }
}

TButtons WaitForNxtButton()
{
  TButtons btn = kNoButton;
  while (btn == kNoButton)
  {
    btn = nNxtButtonPressed;
  }
  WaitForNxtButtonRelease();
  return btn;
}

TButtons GetNxtButton()
{
  return nNxtButtonPressed;
}
