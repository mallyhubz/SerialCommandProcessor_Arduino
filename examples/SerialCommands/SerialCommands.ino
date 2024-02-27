#include "SerialCommandProcessor.h"

// ############### Update these with your commands  ###############
//
#define COMMAND_STRINGS {"hello", "led", "print"}
#define NUM_COMMANDS    3
#define DELIMITER       0x20
#define HELLO      0
#define PROCESS    1
#define BYE        2
#define HELP_TXT      "Valid commands are:  hello   led <0,1>   print <arg> <arg> <arg>"
//
// ############### Update these with your commands  ###############

const char HELP_TXT_ARRAY[] PROGMEM = HELP_TXT;
const char *HELP_TXT_PTR = (const char *) HELP_TXT_ARRAY;

const char* COMMAND_STRING_ARRAY[NUM_COMMANDS] = COMMAND_STRINGS;

SerialCommandProcessor scp(&Serial, NUM_COMMANDS,COMMAND_STRING_ARRAY,(char)DELIMITER);

void handleHello()
{
  Serial.println("Hello");
}

void handleLED(String* arguments, int argCount)
{
  if (argCount != 1)
  {
    Serial.println("set expects 1 argument");
    return;
  }
  bool state = static_cast<bool>(arguments[1].toInt());
  digitalWrite(LED_BUILTIN, state);  
}

void handlePrint(String* arguments, int argCount)
{
  Serial.println("Arguments");
  Serial.print("Argument 1 = ");Serial.println(arguments[1]);
  Serial.print("Argument 2 = ");Serial.println(arguments[2]);
  Serial.print("Argument 3 = ");Serial.println(arguments[3]);
}

// Array of function pointers corresponding to each command (has to be after the functions themselves)
CommandHandler commandHandlers[NUM_COMMANDS] = {handleHello, handleLED, handlePrint};

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{   
  scp.processSerialCommand(commandHandlers,HELP_TXT_PTR);
}
