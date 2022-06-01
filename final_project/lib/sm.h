/**
 * State table for EDGE NODE
 * 
*/

#ifndef _SM_H_
#define _SM_H_

//Various states in the state machine
typedef enum
{
	STATE_POWER_UP,
	STATE_FIELD_MODE,
	STATE_STORE_DATA,
	STATE_TRANSMIT_DATA,
	STATE_WAIT_TRANSMIT_COMPLETE,
	STATE_LORA_SLEEP,
	STATE_DEBUG_LISTEN,
	STATE_DEBUG_RESPOND
}STATE_T;

typedef  int action_t;		//actions to take in each state
typedef  int button_t;		//button behaviour
typedef  int uart_done_t;		//uart behaviour
#define STATE_NONE 0

//state machine blueprint
typedef struct
{
	STATE_T current_state;
	char* debug_name;
	action_t action;
	button_t button;
	uart_done_t uart_done;
}STATE_ACTION_T;


STATE_ACTION_T state_action_table[]=
{
//State										On_Completion					Button					Uart_complete
{STATE_POWER_UP,		"power_up",			STATE_FIELD_MODE,				STATE_DEBUG_LISTEN,		STATE_NONE},
{STATE_FIELD_MODE,		"field_power",		STATE_STORE_DATA,				STATE_DEBUG_LISTEN,		STATE_NONE},
{STATE_STORE_DATA,		"store_data",		STATE_TRANSMIT_DATA,			STATE_DEBUG_LISTEN,		STATE_NONE},
{STATE_TRANSMIT_DATA,	"transmit_data",	STATE_WAIT_TRANSMIT_COMPLETE,	STATE_DEBUG_LISTEN,		STATE_NONE},
{STATE_LORA_SLEEP,		"lora_sleep",		STATE_FIELD_MODE,				STATE_DEBUG_LISTEN,		STATE_NONE},
{STATE_DEBUG_LISTEN,	"debug_listen",		STATE_NONE,						STATE_FIELD_MODE,		STATE_DEBUG_RESPOND},
{STATE_DEBUG_RESPOND,	"debug_respond",	STATE_DEBUG_LISTEN,				STATE_FIELD_MODE,		STATE_NONE},
};

#endif	/*_SM_H_*/