
#ifndef _RAK4270_H_
#define _RAK4270_H_

#include "stdio.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"


#define DEV_VER "at+version\r\n"
#define DEV_STATUS "at+get_config=device:status\r\n"
#define LORA_STATUS "at+get_config=lora:status\r\n"
#define LORA_TXFER_MODE "at+set_config=lorap2p:transfer_mode:2\r\n"		//set device to send in p2p mode
#define LORA_P2P_SEND "at+send=lorap2p:1234\r\n"
// char cmd[256];
// char reply[512];

void rak4270_setup(void);
int rak4270_send_cmd(char* cmd_str);
void rak4270_get_resp(char * reply_str);



#endif