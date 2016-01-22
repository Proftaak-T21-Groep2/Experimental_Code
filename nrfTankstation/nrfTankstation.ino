/*-----( Import needed libraries )-----*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN   9 // Arduino Uno 9  | Mega 9
#define CSN_PIN 53 // Arduino Uno 10 | Mega 53
/*-----( Communication Protocol )-----*/
typedef struct {
  int commandType;
  int param1;
  char param2[9];
  int param3;
} COMMAND;

#define COMMAND_MOVE_TO_PUMP 1
#define COMMAND_START_FUELING 2
#define COMMAND_PAYMENT_COMPLETE 3
#define COMMAND_MOVE_TO_EXIT 4
#define COMMAND_ARRIVED_AT_PUMP 5
#define COMMAND_FINISHED_FUELING 6
/*-----( Communication Pipes )-----*/
const uint64_t pipeTX = 0xE8E8F0F0E1LL; // Define the transmit pipe
const uint64_t pipeRX = 0xE8E8F0F0D2LL; // Define the receive pipe

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

COMMAND received_command;
COMMAND command_to_send;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int inputLength = 0;

void writeCommand(int commandType, int param1, String param2, int param3){
 command_to_send.commandType = commandType;
 command_to_send.param1 = param1;
 for(int i = 0; i < 8; i++){
  command_to_send.param2[i] = param2.charAt(i); 
 }
 command_to_send.param3= param3; 
}

void sendCommand()
{
  radio.stopListening();
  radio.write(&command_to_send, sizeof(COMMAND));
  radio.startListening();
}

void receiveCommand()
{
  if (radio.available()) {
    radio.read(&received_command, sizeof(COMMAND));
    Serial.print("{");
    Serial.print("\n\tCommandType: "); Serial.print(received_command.commandType);
    Serial.print("\n\tP1: "); Serial.print(received_command.param1);
    Serial.print("\n\tP2: "); Serial.print(received_command.param2);
    Serial.print("\n\tP3: "); Serial.print(received_command.param3);
    Serial.println("\n}");
  }
}

void checkRFID() {
  if (stringComplete) {
    if (inputLength == 12) {
      if (inputString == "3D0045BE64A2") {
        Serial.println("Blue");
        command_to_send.commandType = COMMAND_MOVE_TO_PUMP;
        command_to_send.param1 = 1;
        command_to_send.param2[0] = 'A';
        command_to_send.param2[0] = 'B';
        command_to_send.param2[0] = '-';
        command_to_send.param2[0] = '1';
        command_to_send.param2[0] = '2';
        command_to_send.param2[0] = '-';
        command_to_send.param2[0] = 'C';
        command_to_send.param2[0] = 'D';
        command_to_send.param3 = 0;
        sendCommand();
        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13, LOW);
      }
      else if(inputString == "0100026F402C") {
        Serial.println("Yellow"); 
      }
      else if(inputString == "3D00A8972321") {
         Serial.println("Red"); 
      }
      else if(inputString == "41003D45B68F") {
         Serial.println("Tag 17846"); 
      }
      else if(inputString == "41003D882EDA") {
         Serial.println("Tag 34862"); 
      }
      else {
       Serial.print("Unknown: "); Serial.println(inputString); 
      }
    }

    inputLength = 0;
    inputString = "";
    stringComplete = false;
  }
}

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
  inputString.reserve(64);

  radio.begin();
  radio.openWritingPipe(pipeTX);
  radio.openReadingPipe(1, pipeRX);
  radio.startListening();
}

void loop() {
  //  delay(500);
  //  testSend();
  checkRFID();
}

void testSend() {
  command_to_send.commandType = COMMAND_MOVE_TO_PUMP;
  command_to_send.param1 = 1;
  command_to_send.param2[0] = 'D';
  command_to_send.param2[1] = 'A';
  command_to_send.param2[2] = '-';
  command_to_send.param2[3] = '5';
  command_to_send.param2[4] = '6';
  command_to_send.param2[5] = '-';
  command_to_send.param2[6] = 'B';
  command_to_send.param2[7] = 'D';
  command_to_send.param2[8] = '\0';
  command_to_send.param3 = 0;
  sendCommand();
}

void serialEvent1() {
  while (Serial1.available()) {
    char inChar = (byte)Serial1.read();
    if ((int)inChar != 3 && (int)inChar != 2 && (int)inChar != 13) {
      if (inChar == '\n') {
        stringComplete = true;
      }
      else {
        inputString += inChar;
        inputLength++;
      }
    }
  }
}


