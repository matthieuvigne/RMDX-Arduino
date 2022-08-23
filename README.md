# Arduino RMDX driver

A library to drive [My Actuator's RMD-X brushless motors](https://www.myactuator.com/).
These are geared brushless motors, with integrated encoder & driver, performing field-oriented control.

These motors use a CAN bus for communication: as Arduinos do not have any CAN port, an adapter is needed. This library makes use of the classical MCP2515 chip.

*Note: this library implement the V2.4 version of MyActuator's protocol, corresponding to the
RMDX V1 serie. The RMDX-V2 uses their V3 driver, with a slightly different protocol.*

This library was made mostly with simple, "Hello World", applications in mind: a basic example of torque control and
velocity control is shown. More advanced applications would require additional development to unlock all features.
