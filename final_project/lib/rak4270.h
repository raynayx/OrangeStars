/**
 * RAK4270 driver
 * 
 * 
 * 
 * */

#ifndef _RAK4270_H_
#define _RAK4270_H_

#include "stdio.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

#define uart_num uart0


#define DEV_VER "at+version\r\n"
#define DEV_STATUS "at+get_config=device:status\r\n"
#define LORA_STATUS "at+get_config=lora:status\r\n"
#define LORA_TXFER_MODE "at+set_config=lorap2p:transfer_mode:2\r\n"		//set device to send in p2p mode
#define LORA_P2P_SEND "at+send=lorap2p:"
#define CR_LF "\r\n"


bool rak4270_init(void);
void rak4270_send_cmd(char* cmd_str);

/**
 * Send commands with a payload
*/
void rak4270_send_cmd_payload(char* cmd_str,char* payload);


void rak4270_get_resp(char * reply_str);


#endif