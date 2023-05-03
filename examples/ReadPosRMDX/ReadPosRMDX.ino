// A test of using the RMD-X actuator in velocity mode


#include <ACAN2515.h>
#include "RMDX.h"

unsigned char motorID1 = 1;
unsigned char motorID2 =2;
unsigned char motorID3 =3;
unsigned char motorID4  =4;

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
int32_t current_pos1 = 123;
int32_t current_pos3 = 123;
int32_t current_pos4 = 123;

int32_t current_pos_RAW = 123;

void setup () {
    // Use serial port for debug: use serial plotter to view the current / target speed curve.
    Serial.begin (115200) ;
    //Serial.print("speed(dps) target(dps)");
    // Configure MCP2515
    SPI.begin () ;
    ACAN2515Settings settings (QUARTZ_FREQUENCY, CAN_BAUDRATE);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode = can.begin (settings, [] { can.isr () ; });
    // Enable the motor with ID 1 (i.e. 141), and give it some time to start up.
    //motor.enable(motorID);
    delay(3000);

    current_pos1 = motor.getPosition(motorID1);
    Serial.print("POSITION: ");
    Serial.println(((float)current_pos1/100.0f));
    delay(200);    
    current_pos_RAW = motor.getEncoderPositionRAW(motorID1);
    Serial.print("READ: ");
    Serial.println(current_pos_RAW);
    delay(200);
    current_pos1 = motor.setEncoderZero(motorID1, current_pos_RAW);
    Serial.print("Set: ");
    Serial.println(current_pos1);
    Serial.println();

    motorID1 = 3;
    current_pos1 = motor.getPosition(motorID1);
    Serial.print("POSITION: ");
    Serial.println(((float)current_pos1/100.0f));
    delay(200);
    current_pos_RAW = motor.getEncoderPositionRAW(motorID1);
    Serial.print("READ: ");
    Serial.println(current_pos_RAW);
    delay(200);
    current_pos1 = motor.setEncoderZero(motorID1, current_pos_RAW);
    Serial.print("Set: ");
    Serial.println(current_pos1);
    Serial.println();

    motorID1 = 4;
    current_pos1 = motor.getPosition(motorID1);
    Serial.print("POSITION: ");
    Serial.println(((float)current_pos1/100.0f));
    delay(200);
    current_pos_RAW = motor.getEncoderPositionRAW(motorID1);
    Serial.print("READ: ");
    Serial.println(current_pos_RAW);
    delay(200);
    current_pos1 = motor.setEncoderZero(motorID1, current_pos_RAW);
    Serial.print("Set: ");
    Serial.println(current_pos1);
    Serial.println();


    Serial.println("RESET THE MOTORS!");
    for (int i=0; i<3; i++){
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    Serial.println("IN!");

}


void loop () {
  motorID1 = 1;
  // // Gradually increase motor target speed: +50 dps (motor side, the reduction
  // // has to be taken into account) every 5s.
  // if (increaseVelocityDate < millis())
  // {
  //   increaseVelocityDate += 1000;
  //   targetSpeed += 1000;
  //   targetSpeed = targetSpeed%50000;
  // }
  // // Send the target speed to the motor, the current speed measurement is returned.
  // int currentSpeed = motor.setSpeed(motorID, targetSpeed);

  // Serial.print(currentSpeed);
  // Serial.print(" ");
  // Serial.print(targetSpeed / 100); // targetSpeed's unit is 0.01dps.
  // Serial.print("\t");
  // Serial.println(targetSpeed, HEX);

  // current_pos =  motor.getPosition(motorID);

  current_pos1 = motor.getPosition(motorID1);
  Serial.print((current_pos1/100));
  Serial.print(',');
  delay(10);
 
  current_pos3 = motor.getPosition(motorID3);
  Serial.print((current_pos3/100));
  Serial.print(',');
  delay(10);

  current_pos4 = motor.getPosition(motorID4);
  Serial.print((current_pos4/100));
  Serial.println(',');

  delay(100);
 


 
}
