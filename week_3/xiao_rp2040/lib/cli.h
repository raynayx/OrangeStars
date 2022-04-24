#include <stdio.h>
#include <string.h>


#define arraySize 100
#define listNo 7

typedef struct sOptions
{
	char* flag;
	char*  definition;
} sOptions;

typedef struct sDeviceInfo
{
	char * deviceID;
	char * deviceSerial;
} sDeviceInfo;


typedef struct  cli
{
	sOptions options[arraySize];
	const char* progname;
	float battery;
	float humidity;
	float temperature;
	float voltage;
	int rssi;
	sDeviceInfo dev;

} cli;


void initCli(cli *c,const char *pname)
{	
	c->progname = pname;
	
	c->options[0].flag = (char*)"help"; 
	c->options[0].definition = (char*)"\tDisplays this help menu";

	c->options[1].flag = (char*)"device";
	c->options[1].definition = (char*)"Shows device information";

	c->options[2].flag = (char*)"voltage";
	c->options[2].definition = (char*)"Shows the reference voltage on this device";

	c->options[3].flag = (char*)"temp";
	c->options[3].definition = (char*)"\tDisplays current temperature reading from sensor";

	c->options[4].flag = (char*)"humidity";
	c->options[4].definition = (char*)"Displays current humidity reading from sensor";

	c->options[5].flag = (char*)"battery";
	c->options[5].definition =(char*)"Displays current battery level in percentage";

	c->options[6].flag = (char*)"rssi";
	c->options[6].definition = (char*)"\tDisplays signal strength of LoRa signal";

	c->options[7].flag = (char*)"transmit"; 
	c->options[7].definition = (char*)"\tTransmit data to LoRa peer";


	c->dev.deviceID = (char*)"NONE";
	c->dev.deviceSerial= (char*)"000000";
}

void showHelp(cli *c)
{
	printf("Welcome to %s.\n The program usage options are as follows:\n",c->progname);
	for(int i=0; i < listNo; i++)
	{
		printf(" -%s\t%s.\n",c->options[i].flag,c->options[i].definition);
	}
}

void showVoltage(cli *c)
{
	printf("Current voltage is %fV.\n",c->voltage);
}
	
void showTemperature(cli *c)
{
	printf("Current temperature reading is %fC.\n",c->temperature);
}
	
void showHumidity(cli *c)
{
	printf("Current Humidity reading is %f %%.\n",c->humidity);
}

void showBatteryLevel(cli *c)
{
	printf("Battery level %f %% left. \n",c->battery);
}

void showRSSI(cli *c)
{
	printf("RSSI is %d dB.\n",c->rssi);
}
	
void updateParams(cli *c,float v, float t, float H, float b, int r)
{
	c->voltage = v;
	c->temperature = t;
	c->humidity = H;
	c->battery = b;
	c->rssi = r;

}

void showPrompt(cli *c)
{
	printf("%s> ",c->progname);
}


void showDeviceInfo(cli *c)
{
	printf("This device's ID is %s and Serial number is %s.\n",c->dev.deviceID,c->dev.deviceSerial);
}

// void showLivelog()
// {

// }

void listen(cli *c,const char* s)
{
	if(strcmp(s,"help") == 0)
	{
		showHelp(c);
	}
	else if(strcmp(s,"device") == 0)
	{
		showDeviceInfo(c);
	}
	else if(strcmp(s,"voltage") == 0)
	{
		showVoltage(c);
	}
	else if(strcmp(s,"temp") == 0)
	{
		showTemperature(c);
	}
	else if(strcmp(s,"humidity") == 0)
	{
		showHumidity(c);
	}
	else if(strcmp(s,"battery") == 0)
	{
		showBatteryLevel(c);
	}
	else if(strcmp(s,"rssi") == 0)
	{
		showRSSI(c);
	}
	else
	{
		showHelp(c);
	}
}


void setBattery(cli *c,float b)
{
	c->battery=b;
}
void setHumidity(cli *c,float H)
{
	c->humidity=H;
}
void setTemperature(cli *c,float t)
{
	c->temperature=t;
}
void setVoltage(cli *c,float v)
{
	c->voltage=v;
}
void setRSSI(cli *c,int r)
{
	c->rssi=r;
}
void setDeviceInfo(cli *c, sDeviceInfo *d)
{
	c->dev.deviceID = d->deviceID;
	c->dev.deviceSerial= d->deviceSerial;
}
