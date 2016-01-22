String unfinnished;
String temp;
bool reading = false;
bool done = false;

String makeCommand()
{
  char receivedCharacter = readSerialLine();
  if (receivedCharacter == '%' && !reading)
  {
    reading = true;
  }

  if (receivedCharacter == '#' && reading)
  {
    reading = false;
    done = true;
  }


  if (reading && !done)
  {
    if (receivedCharacter != '%'){
      unfinnished += String(receivedCharacter);
    }
  }


  if (done)
  {
    done = false;
    reading = false;
    temp = unfinnished;
    unfinnished = "";
    return temp;
  }
  return " ";
}


