#include "SerialCommandProcessor.h"

SerialCommandProcessor::SerialCommandProcessor(HardwareSerial *serialPort, int numCommands, const char* newCommandStrings[], const char delimiter) {

  // Set the number of commands
  this->numCommands = numCommands;
  this->delimiter = delimiter;
  _serialPort = serialPort;
  
  // Dynamically allocate memory for command strings
  commandStrings = new String[numCommands];

  // Copy command strings
  for (int i = 0; i < numCommands; i++) {
    commandStrings[i] = newCommandStrings[i];
  }
}

void SerialCommandProcessor::processSerialCommand(CommandHandler commandHandlers[], const char* helpTxt) {
	
  if (_serialPort->available()) {
    CommandData CDATA = readSerialCommand();

    if (CDATA.commandIndex >= 0 && CDATA.commandIndex < this->numCommands) {
      // Call the appropriate handler function using function pointer
      commandHandlers[CDATA.commandIndex](CDATA.args, CDATA.argCount);
    } else {
      _serialPort->println(helpTxt);
    }
  }
}

CommandData SerialCommandProcessor::readSerialCommand() {
  CommandData result;
  
  // Read input from serial port
  String inputString = _serialPort->readStringUntil('\n');
  
  // Split the input string  
  int partIndex = 0;
  int startIndex = 0;
  int argCount = 0;

  for (int i = 0; i < inputString.length(); i++) {
    char currentChar = inputString.charAt(i);
    if (currentChar == delimiter) {
      result.args[partIndex] = inputString.substring(startIndex, i);

      if (inputString.substring(startIndex, i).length() > 0) {
        argCount++;
      }

      startIndex = i + 1;

      if (partIndex == MAX_ARGUMENTS) {
        // Break if the maximum number of parts is reached
        break;
      }

      partIndex++;
    }
  }

  // Capture the last part after the last delimiter
  result.args[partIndex] = inputString.substring(startIndex);

  // Map command string to index
  result.commandIndex = parseCommand(result.args[0]);
  
  result.argCount = argCount;
  return result;
}

int SerialCommandProcessor::parseCommand(String command) {
  // Compare command string with user-defined commands
  for (int i = 0; i < numCommands; i++) {
    if (command == commandStrings[i]) {
      return i;
    }
  }
  return -1; // Return -1 for unknown command
}
