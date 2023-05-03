// A test of using the RMD-X actuator in velocity mode


#include <ACAN2515.h>
#include "RMDX.h"


// Configure CAN bus adapter.
static const byte MCP2515_CS  = 5 ; // CS input of MCP2515 (adapt to your design)
static const byte MCP2515_INT =  17 ; // INT output of MCP2515 (adapt to your design)
ACAN2515 can (MCP2515_CS, SPI, MCP2515_INT) ;

// Select the quartz frequency of your MPC2515 - 8MHz or 16MHz are often used.
static const uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL ; // 8 MHz quartz
// Select CAN frequency: 500kbps or 1Mbps can be used.
// Note however that, if a 8MHz quartz is used on the MPC2515, only 500kbps can be used, 1MHz is too fast.
static const uint32_t CAN_BAUDRATE = 500UL * 1000UL; // 500kpbs CAN

RMDX motor(&can); // Create the motor

void setup () {
    // Use serial port for debug: use serial plotter to view the current / target speed curve.
    Serial.begin (115200) ;
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

long angulo = 0;
int16_t targetSpeed = 3000;  //dps
int currentAngle =0;
int prev_angle = 0;
double t = 0.0;
void loop () {

    angulo = (long)(90.0*sin(t)*100.0)+36000;
    currentAngle = motor.setAngle(1, angulo, targetSpeed);
    Serial.print(prev_angle);
    Serial.print(", ");
    Serial.println(angulo / 100); 
    
    t = t>=360.0? 0: t=t+0.01;
    prev_angle = currentAngle;
    delay(25);

}
