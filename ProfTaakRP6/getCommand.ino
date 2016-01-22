String unfinished;
String temp;
bool reading = false;
bool done = false;

char readSerialLine()
{
  if (Serial.available() > 0)
  {
    int incomingChar = Serial.read();

    char receivedCharacter = (char) incomingChar;
    return receivedCharacter;
  } 
  return NULL;
}

String getCommand()
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
      unfinished += String(receivedCharacter);
    }
  }


  if (done)
  {
    done = false;
    reading = false;
    temp = unfinished;
    unfinished = "";
    return temp;
  }
  return " ";
}


