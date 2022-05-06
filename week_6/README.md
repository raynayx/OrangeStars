<!-- # Application description -->
# Forest Fire Detection
Forest fires can be expensive to contain when the weather fans the combustion. The extent of the damage can be limited when the fires are detected early and attended to.

The bush fire detection system detects precursors to a fire. Variations in ambient parameters like: **temperature**, **humidity**,**C0 gas**, **C0<sub>2</sub> gas** values in an area can help detect forest fires as early as posible.

This project aims to measure the above mentioned parameters and transmit them over a long range communication technology to a gateway.
The long range communication technology being considered is LoRa.
The data is collated from different nodes and the decision is made to raise a fire alarm or not. When a fire alarm is raised by the system, the fire service can be deployed to check it out and forestall the spread of the fire.

The system has edge devices that gather ambient data and a gateway that collates them for deciding if there is a potential fire.




# Hardware description
The hardware setup of the system consists of two main parts:
1. The Edge node and
2. The Gateway


### Edge node:
The Edge node is the part of the system that collects the data in the forest. It is made of:
- Microcontroller
- Sensor:
	- Temperature and Humidity sensor
- Button
- LoRa transciever
- Power supply

### Gateway:
The Gateway is a central station to which the edge nodes send the data gathered. It is made of:
- LoRa transciever
- SBC

## Hardware Parts Requirements
### Edge node

#### Microcontroller
- Available
- Cortex M series based
- Low power 
#### Sensor
- Cheap
- Wide temperature range
- 

#### LoRa transciever
- Frequency
- Availability
- Low cost
- Low power

#### Power Supply
- 
### Gateway:
#### LoRa transciever
- 
#### SBC
- Low cost
- Fairly powerful
- Available

## Hardware Selection
- Edge node:
	- Microcontroller: 
		- RP2040( Arm Cortex-M0+)
			- 264KB internal RAM
			- 2MB external flash
		
	- Sensor: 
		- AHT21 temperature and humidity sensor
	- LoRa transciver: 
		- RAK4200 breakout board with antenna
	- Power supply:


# Software description
- Describe code in general

<!-- - Describe parts you wrote in some detail(3-5 sentences per module) -->
I wrote the code to gather the sensor data, store it and transmit at intervals. I also wrote code to put the device to sleep and wake at predetermined intervals.
I implemented a state machine to cycle through various states in the life of the edge device.


<!-- - Describe code you re-used from other sources, including licenses for those -->
I used a library from the vendor of the LoRa module (RAK Wireless).
I also used the Raspberry Pi Pico SDK as a HAL.


# Diagram of the system architecture
## System Block diagram
![System overview diagram](./img/systemOverview.drawio.svg)

## Hardware Block diagram of Edge node
![Edge node hardware diagram](./img/hardwareDiagram.drawio.svg)

## Software Block diagram of Edge node
![Software block diagram of Edge node](./img/softwareBlockDiagram.drawio.svg)

## Software Hierarchy of Control diagram
![Software Hierarchy of control diagram](./img/hierarchyOfControlDiagram.drawio.svg)
## Software Layered Block diagram
![Software Layered Block diagram](./img/layeredBlockDiagram.drawio.svg)

# Project To-Dos
- [ ] Edge node code
	- [] Sensor data gathering code
	- [] Data transmission code
	- [] Data storage code
	- [] Implement state machine
- [ ] Hardware assembly for edgee node
- [ ] Gateway assembly

# Build instructions
- How to build the system(including toolchain)
	- Hardware
	- Software
- How you debugged and tested the system
- How you powered it (and how you might in the future)

# Future
- What would be needed to get this project ready for production?
- How would you extend this project to do something more?
	- Are there other features you'd like to add?
		- How would you go about adding them?
# Grading
- Self assessment of the project: 
- Have you gone beyond the basic requirement? How so?

# Link to the code