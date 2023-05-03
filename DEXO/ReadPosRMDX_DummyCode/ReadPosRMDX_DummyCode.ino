// A test of using the RMD-X actuator in velocity mode


#include <ACAN2515.h>
#include "RMDX.h"

unsigned char motorID1 = 1;
unsigned char motorID2 = 2;
unsigned char motorID3 = 3;
unsigned char motorID4 = 4;

// Configure CAN bus adapter.
static const byte MCP2515_CS  = 5;  // CS input of MCP2515 (adapt to your design)
static const byte MCP2515_INT = 17; // INT output of MCP2515 (adapt to your design)
ACAN2515 can (MCP2515_CS, SPI, MCP2515_INT) ;

// Select the quartz frequency of your MPC2515 - 8MHz or 16MHz are often used.
static const uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL ; // 8 MHz quartz
// Select CAN frequency: 500kbps or 1Mbps can be used.
// Note however that, if a 8MHz quartz is used on the MPC2515, only 500kbps can be used, 1MHz is too fast.
static const uint32_t CAN_BAUDRATE = 500UL * 1000UL; // 500kpbs CAN

RMDX motor(&can); // Create the motor

void set_offset(unsigned char motorID){
    int32_t current_pos = motor.getPosition(motorID);
    Serial.print("POSITION: ");
    Serial.print(((float)current_pos/100.0f));
    delay(100);    
    int32_t current_pos_RAW = motor.getEncoderPositionRAW(motorID);
    Serial.print("\tREAD: ");
    Serial.print(current_pos_RAW);
    delay(100);
    current_pos = motor.setEncoderZero(motorID, current_pos_RAW);
    Serial.print("\tSet: ");
    Serial.println(current_pos);

}

void setup () {
    Serial.begin (115200) ;
    // Configure MCP2515
    SPI.begin () ;
    ACAN2515Settings settings (QUARTZ_FREQUENCY, CAN_BAUDRATE);
    settings.mRequestedMode = ACAN2515Settings::NormalMode;
    const uint16_t errorCode = can.begin (settings, [] { can.isr () ; });
    delay(3000);

    // set_offset(motorID1);
    set_offset(motorID2);
    // set_offset(motorID3);
    // set_offset(motorID4);

    Serial.println("RESET THE MOTORS!");
    for (int i=0; i<5; i++){
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    Serial.println("IN!");

}

int32_t current_pos1 = 0;
int32_t current_pos2 = 0;
int32_t current_pos3 = 0;
int32_t current_pos4 = 0;
uint16_t increment = 0;
void loop () {
  //changing the decimal values to create a change in the dummy data sent
  // = (increment < 9000) ? increment + 100 : 0;

  current_pos1 = motor.getPosition(motorID1);
  Serial.print((float)(current_pos1+increment)/100.0f);
  Serial.print(',');
  delay(2);

  current_pos2 = motor.getPosition(motorID2);
  Serial.print((float)(current_pos2+increment)/100.0f);
  Serial.print(',');
  delay(2);
 
  current_pos3 = motor.getPosition(motorID3);
  Serial.print((float)(current_pos3+increment)/100.0f);
  Serial.print(',');
  delay(2);

  current_pos4 = motor.getPosition(motorID4);
  Serial.print((float)(current_pos4+increment)/100.0f);
  Serial.println();
  delay(2); 
}
