// A test of using the RMD-X actuator in velocity mode


#include <ACAN2515.h>
#include "RMDX.h"


// Configure CAN bus adapter.
static const byte MCP2515_CS  = 10 ; // CS input of MCP2515 (adapt to your design)
static const byte MCP2515_INT =  3 ; // INT output of MCP2515 (adapt to your design)
ACAN2515 can (MCP2515_CS, SPI, MCP2515_INT) ;

// Select the quartz frequency of your MPC2515 - 8MHz or 16MHz are often used.
static const uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL ; // 8 MHz quartz
// Select CAN frequency: 500kbps or 1Mbps can be used.
// Note however that, if a 8MHz quartz is used on the MPC2515, only 500kbps can be used, 1MHz is too fast.
static const uint32_t CAN_BAUDRATE = 500UL * 1000UL; // 500kpbs CAN

RMDX motor(&can); // Create the motor

void setup () {
    // Use serial port for debug: use serial plotter to view the current / target speed curve.
    Serial.begin (38400) ;
    Serial.print("speed(dps) target(dps)");
    // Configure MCP2515
    SPI.begin () ;
    ACAN2515Settings settings (QUARTZ_FREQUENCY, CAN_BAUDRATE);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode = can.begin (settings, [] { can.isr () ; });
    // Enable the motor with ID 1 (i.e. 141), and give it some time to start up.
    motor.enable(1);
    delay(1000);
}

uint32_t increaseVelocityDate = 0;
int32_t targetSpeed = 0;
void loop () {

  // Gradually increase motor target speed: +50 dps (motor side, the reduction
  // has to be taken into account) every 5s.
  if (increaseVelocityDate < millis())
  {
    increaseVelocityDate += 5000;
    targetSpeed += 20000;
  }
  // Send the target speed to the motor, the current speed measurement is returned.
  int currentSpeed = motor.setSpeed(1, targetSpeed);

  Serial.print(currentSpeed);
  Serial.print(" ");
  Serial.println(targetSpeed / 100); // targetSpeed's unit is 0.01dps.
  delay(50);
}
