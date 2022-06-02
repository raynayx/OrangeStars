// ConsoleCommands.c
// This is where you add commands:
//		1. Add a protoype
//			static eCommandResult_T ConsoleCommandVer(const char buffer[]);
//		2. Add the command to mConsoleCommandTable
//		    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
//		3. Implement the function, using ConsoleReceiveParam<Type> to get the parameters from the buffer.

#include <string.h>
#include "consoleCommands.h"
#include "console.h"
#include "consoleIo.h"
#include "version.h"


#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "rak4270.h"
#include "aht21.h"
#include "ws2812.h"
#include "batteryLevel.h"

#define IGNORE_UNUSED_VARIABLE(x)     if ( &x == &x ) {}

static eCommandResult_T ConsoleCommandComment(const char buffer[]);
static eCommandResult_T ConsoleCommandVer(const char buffer[]);
static eCommandResult_T ConsoleCommandHelp(const char buffer[]);
static eCommandResult_T ConsoleCommandParamExampleInt16(const char buffer[]);
static eCommandResult_T ConsoleCommandParamExampleHexUint16(const char buffer[]);

//Project specific commands
static eCommandResult_T ConsoleCommand_LED(const char* buffer);
static eCommandResult_T ConsoleCommand_RAK4270(const char*buffer);
static eCommandResult_T ConsoleCommand_AHT21(const char*buffer);
static eCommandResult_T ConsoleCommand_device(const char* buffer);


//Command table
static const sConsoleCommandTable_T mConsoleCommandTable[] =
{
    {";", &ConsoleCommandComment, HELP("Comment! You do need a space after the semicolon. ")},
    {"help", &ConsoleCommandHelp, HELP("Lists the commands available")},
    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
    {"int", &ConsoleCommandParamExampleInt16, HELP("How to get a signed int16 from params list: int -321")},
    {"u16h", &ConsoleCommandParamExampleHexUint16, HELP("How to get a hex u16 from the params list: u16h aB12")},
	{"led",&ConsoleCommand_LED,HELP("Turn on board LED on or off. Eg: led on")},
	{"aht",&ConsoleCommand_AHT21,HELP("Get readings from AHT21. Eg: aht temp; aht hum")},
	{"rak",&ConsoleCommand_RAK4270,HELP("Interract with lora xceiver. Eg. ")},
	{"device",&ConsoleCommand_device,HELP("Get device info.")},

	CONSOLE_COMMAND_TABLE_END // must be LAST
};

static eCommandResult_T ConsoleCommandComment(const char buffer[])
{
	// do nothing
	IGNORE_UNUSED_VARIABLE(buffer);
	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandHelp(const char buffer[])
{
	uint32_t i;
	uint32_t tableLength;
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	tableLength = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
	for ( i = 0u ; i < tableLength - 1u ; i++ )
	{
		ConsoleIoSendString(mConsoleCommandTable[i].name);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(" : ");
		ConsoleIoSendString(mConsoleCommandTable[i].help);
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}

static eCommandResult_T ConsoleCommandParamExampleInt16(const char buffer[])
{
	int16_t parameterInt;
	eCommandResult_T result;
	result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
	if ( COMMAND_SUCCESS == result )
	{
		ConsoleIoSendString("Parameter is ");
		ConsoleSendParamInt16(parameterInt);
		ConsoleIoSendString(" (0x");
		ConsoleSendParamHexUint16((uint16_t)parameterInt);
		ConsoleIoSendString(")");
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}
static eCommandResult_T ConsoleCommandParamExampleHexUint16(const char buffer[])
{
	uint16_t parameterUint16;
	eCommandResult_T result;
	result = ConsoleReceiveParamHexUint16(buffer, 1, &parameterUint16);
	if ( COMMAND_SUCCESS == result )
	{
		ConsoleIoSendString("Parameter is 0x");
		ConsoleSendParamHexUint16(parameterUint16);
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}

static eCommandResult_T ConsoleCommandVer(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	ConsoleIoSendString(VERSION_STRING);
	ConsoleIoSendString(STR_ENDLINE);
	return result;
}

static eCommandResult_T ConsoleCommand_LED(const char* buffer)
{
	eCommandResult_T result = COMMAND_ERROR;
	char param[CONSOLE_COMMAND_MAX_COMMAND_LENGTH];
	result = ConsoleReceiveParamString(buffer,1,param);

	if(result == COMMAND_SUCCESS)
	{
		if(strcmp("on",param) == 0)
		{
			gpio_put(25,true);
		}
		else if(strcmp("off",param) == 0)
		{
			gpio_put(25,false);
		}
	}
	return result;
}

static eCommandResult_T ConsoleCommand_AHT21(const char* buffer)
{
	AHT21 s;
	AHT21_init(&s,i2c0);
	eCommandResult_T result = COMMAND_ERROR;
	char param[CONSOLE_COMMAND_MAX_COMMAND_LENGTH];
	result = ConsoleReceiveParamString(buffer,1,param);
	float reading = 0.0f;
	char payload[10];

	if(result == COMMAND_SUCCESS)
	{
		if(strcmp("temp",param) == 0)
		{
			reading = AHT21_get_temperature(&s);
			sprintf(payload,"%.1fC",reading);
		}
		else if(result == COMMAND_SUCCESS)
		{
			reading = AHT21_get_humidity(&s);
			sprintf(payload,"%.1f%%",reading);
		}
	}
	
	ConsoleIoSendString(payload);
	ConsoleIoSendString(STR_ENDLINE);

	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommand_device(const char* buffer)
{
	eCommandResult_T result = COMMAND_SUCCESS;
	battery_adc_setup();
	char payload[30];

	ConsoleIoSendString(DEVICE_NAME);
	ConsoleIoSendString(STR_ENDLINE);

	ConsoleIoSendString(DEVICE_NUM);
	ConsoleIoSendString(STR_ENDLINE);

	sprintf(payload,"Battery level:%.1fV",battery_reading());
	ConsoleIoSendString(payload);
	ConsoleIoSendString(STR_ENDLINE);

	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommand_RAK4270(const char* buffer)
{
	return COMMAND_SUCCESS;
}
const sConsoleCommandTable_T* ConsoleCommandsGetTable(void)
{
	return (mConsoleCommandTable);
}


