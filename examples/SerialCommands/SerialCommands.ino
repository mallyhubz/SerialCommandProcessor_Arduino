#include "SerialCommandProcessor.h"

#define NUM_COMMANDS 3
#define COMMAND_STRINGS {"hello", "process", "bye"}
#define DELIMITER 0x20

#define HELLO 0
#define PROCESS 1
#define BYE 2

const char* COMMAND_STRING_ARRAY[NUM_COMMANDS] = COMMAND_STRINGS;
SerialCommandProcessor scp(&Serial, NUM_COMMANDS,COMMAND_STRING_ARRAY,(char)DELIMITER);

void handleHello()
{
  Serial.println("Hello");
}

void handleProcess(String* arguments, int argCount)
{
  if (argCount != 2)
  {
    Serial.println("set expects 2 arguments");
    return;
  }
  String arg1 = arguments[1];
  String arg2 = arguments[2];     
  
  Serial.println(arg1);
  Serial.println(arg2);
}

void handleBye()
{
  Serial.println("Bye");
}

// Define function pointer type
typedef void (*CommandHandler)(String*, int);

// Array of function pointers corresponding to each command
CommandHandler commandHandlers[NUM_COMMANDS] = {handleHello, handleProcess, handleBye};

void setup()
{
  Serial.begin(115200);
}

void process_serial() {
  if (Serial.available()) {
    CommandData CDATA = scp.readSerialCommand();

    if (CDATA.commandIndex >= 0 && CDATA.commandIndex < NUM_COMMANDS) {
      // Call the appropriate handler function using function pointer
      commandHandlers[CDATA.commandIndex](CDATA.args, CDATA.argCount);
    } else {
      Serial.println("Help information here");
    }
  }
}

void loop(){
    process_serial();
}
