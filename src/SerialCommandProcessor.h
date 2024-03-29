#ifndef SerialCommandProcessor_h
#define SerialCommandProcessor_h

#include "Arduino.h"

// Define the maximum length for command strings
#define MAX_COMMAND_LENGTH  10
#define MAX_COMMANDS 		5 // Update this based on the number of commands
#define MAX_ARGUMENTS 		3
#define DEFAULT_DELIMITER 	0x20

struct CommandData {
  int commandIndex; // Index of the command in the setCommandStrings array
  String args[MAX_ARGUMENTS+1];
  int argCount;
};

typedef void (*CommandHandler)(String*, int);

class SerialCommandProcessor {
public:
  SerialCommandProcessor(HardwareSerial *serialPort, int numCommands, const char* commandStrings[], char delimiter);

  CommandData readSerialCommand();
  
  void processSerialCommand(CommandHandler commandHandlers[], const char* helpTxt);
  
private:
  int numCommands;
  String* commandStrings;  
  const char* helpTxt;

  int parseCommand(String command);
  
  char delimiter = (char)DEFAULT_DELIMITER;
  
  HardwareSerial *_serialPort;
};

#endif
