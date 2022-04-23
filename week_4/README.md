<!-- 1. Start putting together a plan for your final project. Given the chapter, the main goal is to
put together state machine documentation. This can be a flowchart or (preferably) a
state table. You may also need to create the diagrams from Lesson 2 to flesh out the
design. Note: this design planning, thinking through the system, what you have and
what you need. Keep in mind that these will not be the final version!

2. Extra credit: Al Sweigart wrote many small games in Python. They are keyboard input so
they don’t have very fancy graphics. As with toys, most games have a huge state
machine. Document the state machine for one of the games, suggested examples are
blackjack.py, conwaysgameoflife.py, snailrace.py, zombiebitefight.py,and montyhall.py.
You do not need it to run to play the game but if you do and don’t have python set up,
you can put them in a new Python notebook at https://colab.research.google.com/ -->

# Forest fire detection EDGE NODE state machine
For my final project, I want to create a forest fire detection edge node and a receiving gateway in peer-to-peer mode.
The edge will have a temperature and humidity sensor talking to an MCU over I2C and a LoRa transciever talking to the MCU over UART. There will be a push button which raises interrupts.

The state machine for the edge node is as follows:

STATE | ACTION | LoRa | TIMER | BUTTON |
------|--------|------------|------|-------| 
POWER_UP | init system; go to FIELD_MODE| x | x | x |
FIELD_MODE | poll sensors and read values; go to STORE_DATA | x | start time_to_sleep| go to DEBUG_LISTEN|
STORE_DATA | store data in buffer; increment buffer usage;go to TRANSMIT_MODE| x | countdown time_to_sleep|go to DEBUG_LISTEN|
TRANSMIT_DATA | send data to gateway |LoRa up and transmit;go to FIELD_MODE | x | go to DEBUG_LISTEN|
SLEEP_MODE | put sytem to sleep | LoRa sleep | start time_to_wake| got to FIELD_MODE
WAKE_UP | go to FIELD_MODE | x | reset time_to_sleep | x |
DEBUG_LISTEN | listen for command; go to DEBUG_RESPOND| x | x | go to FIELD_MODE
DEBUG_RESPOND | run command; respond over UART on USB; go to DEBUG_LISTEN | x | x | go to FIELD_MODE



# GATEWAY STATE MACHINE
STATE | ACTION |  LoRa | BUTTON|
------|--------|------------|------
POWER_UP | init system; go to FIELD_MODE| x | x 
FIELD_MODE | listen for LoRa packets | recieve packets | DEBUG_LISTEN
SEND_TO_PC | display data on pc| x | DEBUG_LISTEN|
DEBUG_LISTEN| listen for command;go to DEBUG_RESPOND | x | FIELD_MODE
DEBUG_RESPOND | run command; respond UART on USB; go to DEBUG_LISTEN | x | FIELD_MODE





