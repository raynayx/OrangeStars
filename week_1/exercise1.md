<!-- # Question
Tell us about an embedded system that does not exist. It can be grandiose or strange; it can be
something just one step beyond what you have around you. Ideally it is something you
personally want to build (even if physics doesn’t allow it). If you have a final project in mind, you
can start on that. If not, be imaginative, reality does not need to apply here.
What is it? What does it do? What general components do you need?
This should be summarized in 3-6 sentences. Then make at least one block diagram describing
the system as discussed in Chapter 2 and the associated lectures. More diagrams are better. -->

# Forest Fire Detection
Forest fires can be expensive to contain when the weather fans the combustion. The extent of the damage can be limited when detected early and attended to.

The bush fire detection system detects precursors to a fire. Variations in ambient parameters like: **temperature**, **humidity**,**C0 gas**, **C0<sub>2</sub> gas** values in an area can help detect forest fires as early as posible.

This project aims to measure the above mentioned parameters and transmit them over a long range communication technology to a gateway.
The long range communication technology being considered is LoRa.
The data is collated from different nodes and the decision is made to raise a fire alarm or not. When a fire alarm is raised by the system, the fire service can be deployed to check it out and forestall the spread of the fire.

The system has edge devices that gather ambient data and a gateway that collates them for deciding if there is a potential fire.

### Components required for the edge devices are as follows:
- Sensors(Temperature,Humidity,CO,CO<sub>2</sub>)
- Processing module(Microcontroller)
- Communication module(LoRa)
- Power supply module

# Hardware block diagram
```
                                                                                                              ▲
                                                                                                              │
                                                                                                              │
                                                                                                              │  ANTENNA
                                                                                                              │
┌─────────────────┐                                                           ┌──────────────────┐            │
│                 │                                                           │                  │            │
│   Sensors       │                                                           │                  │            │
│                 │    SPI/I2C       ┌────────────────────────┐     SPI       │    LoRa module   ├────────────┘
│  (Temperature)  ├──────────────────┤                        ├───────────────┤                  │
│                 │                  │                        │               │                  │
└───────────────┬─┘                  │                        │               └─┬────────────────┘
                │                    │      Microcontroller   │                 │
                │                    │                        │                 │
                │                    │                        │                 │
                │                    │                        │                 │
                │                    │                        │                 │
                │                    └──────────────┬─────────┘                 │
                │                                   │                           │
                │   ┌───────────────────────────────┴───────────────────────────┴────┐
                └───┤                                                                │
                    │                        Power Supply                            │
                    │                                                                │
                    └────────────────────────────────────────────────────────────────┘
```

# Software block diagram

```
                             ┌───────────────────────────────────────────────┐
                             │ Microcontroller                               │
                             │                               ┌───────────┐   │
┌───────────────────┐        │                               │   Read    │   │
│                   │        │                               │    Sensor │   │
│    Sensors        ├──┐     │            ┌────────────┐     │   values  │   │
│                   │  │     │            │  Sensors   │     └───────────┘   │
└───────────────────┘  │     │  ┌───────┐ │   Driver   │                     │
                       │     │  │       │ └────────────┘   ┌──────────────┐  │
┌───────────────────┐  ├─────┤  │  SPI  │                  │ Send sensor  │  │
│  Communication    │  │     │  └───────┘ ┌────────────┐   │  values      │  │
│    Module         ├──┘     │            │ Comms      │   └──────────────┘  │
│                   │        │            │  Driver    │                     │
│                   │        │            └────────────┘   ┌──────┐ ┌─────┐  │
└───────────────────┘        │                             │  CLI │ │ UART│  │
                             │                             └──────┘ │     │  │
                             │                                      └─────┘  │
                             │                                               │
                             └───────────────────────────────────────────────┘
```

# Hierarchy of Control diagram
```
                           ┌──────┐
                           │ MAIN │
                           └───┬──┘
                               │
                               │
      ┌──────────────────────┬─┴───────────────────┐
      │                      │                     │
      │                  ┌───┴────┐                │
      │                  │        │                │
      │                  │  CLI   │                │
      │                  │        │                │
      │                  └┬──┬──┬─┘                │
      │                   │  │  │                  │
      │      ┌────────────┘  │  │                  │
      │      │               │  └────────────────┐ │
      │      │               │                   │ │
┌─────┴──────┴─┐             │                 ┌─┴─┴───────┐
│  Read        │             │                 │ Send      │
│   Sensor     │             │                 │   Sensor  │
│   Values     │             │                 │    Values │
└────┬─────────┘             │                 └────┬──────┘
     │                  ┌────┴──┐                   │
     │                  │ UART  │                   │
     │                  └───────┘                   │
 ┌───┴─────┐                                    ┌───┴─────┐
 │ Sensor  │                                    │  Comms  │
 │  Driver │                                    │  Driver │
 └───────┬─┘                                    └┬────────┘
         │                                       │
         │                                       │
         │                                       │
         └────────────────┐    ┌─────────────────┘
                          │    │
                        ┌─┴────┴┐
                        │  SPI  │
                        └───────┘
```

# Layered block diagram
```
┌────────┐  ┌─────────┐
│Read    │  │ Send    │
│ sensor │  │  sensor │
│  values│  │   values│
└────────┘  └─────────┘
 ┌──────┐    ┌────────┐
 │Sensor│    │Comms   │ ┌─────┐
 │      │    │        │ │ CLI │
 │Driver│    │ Driver │ │     │
 └──────┘    └────────┘ └─────┘
        ┌────┐           ┌────┐
        │SPI │           │UART│
        └────┘           └────┘
```