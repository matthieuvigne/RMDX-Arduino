#include "RMDX.h"

// Utility functions
CANMessage createMessage(byte const& motorId, byte const& command, long const& data = 0)
{
    CANMessage message;

    message.id = 0x140 + motorId;
    message.len = 8;
    message.data[0] = command;
    message.data[4] = data & 0xFF;
    message.data[5] = (data >> 8) & 0xFF;
    message.data[6] = (data >> 16) & 0xFF;
    message.data[7] = (data >> 24) & 0xFF;
    return message;
}


int32_t messageToInt32(CANMessage const& message)
{
    return message.data[4] + (message.data[5] >> 8) +  (message.data[6] >> 16) +  (message.data[7] >> 24);
}

RMDX::RMDX(ACAN2515 *canDriver):
    canDriver_(canDriver)
{

}


void RMDX::reset(byte const& motorId)
{
    CANMessage message = createMessage(motorId, MyActuator::commands::RESET);
    canReadWrite(message);
}



void RMDX::enable(byte const& motorId)
{
    CANMessage message = createMessage(motorId, MyActuator::commands::MOTOR_ENABLE);
    canReadWrite(message);
}

void RMDX::disable(byte const& motorId)
{
    CANMessage message = createMessage(motorId, MyActuator::commands::MOTOR_DISABLE);
    canReadWrite(message);
}


int32_t RMDX::getAccelerationCommand(byte const& motorId)
{
    CANMessage message = createMessage(motorId, MyActuator::commands::READ_ACCEL);
    if (canReadWrite(message) == 0)
        return messageToInt32(message);
    return 0;
}

int32_t RMDX::getPosition(byte const& motorId)
{
    CANMessage message = createMessage(motorId, MyActuator::commands::READ_MULTITURN_ANGLE);
    if (canReadWrite(message) == 0)
        return messageToInt32(message);
    return 0;
}


int RMDX::setSpeed(byte const& motorId, int32_t const& targetSpeed)
{
    CANMessage message = createMessage(motorId, MyActuator::commands::SPEED_COMMAND, targetSpeed);
    int result = canReadWrite(message);
    if (result != 0)
        return 0;
    return message.data[4] + (message.data[5] << 8);
}

int RMDX::setTorque(byte const& motorId, int32_t const& targetTorque)
{
    CANMessage message = createMessage(motorId, MyActuator::commands::TORQUE_COMMAND, targetTorque);
    int result = canReadWrite(message);
    if (result != 0)
        return 0;
    return message.data[2] + (message.data[3] << 8);
}

int RMDX::canReadWrite(CANMessage& message, bool const& waitForReply)
{
    CANMessage dump;
    while (canDriver_->available())
        canDriver_->receive(dump);

    if (!canDriver_->tryToSend(message))
        return -1;
    if (waitForReply)
    {
      unsigned long startTime = micros();
       while (!canDriver_->available() && micros() - startTime < 100000)
           delay(1);
       if (canDriver_->available())
          canDriver_->receive(message);
       else
          return -2;
    }
    return 0;
}
