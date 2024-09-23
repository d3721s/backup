#include "interface.h"

void interfaceInit(void)
{
    static std::once_flag canInitializedFlag;
    std::call_once(canInitializedFlag, []
    {
        motorCan.init();
    }
                  );
}

int32_t interfaceGetParameter1(ti5Motor *motorHandle, ti5Motor::FunctionCodeTabSend1Receive4 code)
{
    can_frame frameSend;
    can_frame frameReceive;
    frameSend.can_id = motorHandle->getCanId();
    frameSend.can_dlc = 1;
    frameSend.data[0] = static_cast<uint8_t>(code);
    motorCan.sendFrame(frameSend);
    motorCan.receiveFrame(frameReceive);
    return frameReceive.data[0];
#warning "TODO : not frameReceive.data[0]"
}

int32_t interfaceGetParameter5(ti5Motor *motorHandle, ti5Motor::FunctionCodeTabSend5Receive4 code)
{
    can_frame frameSend;
    can_frame frameReceive;
    frameSend.can_id = motorHandle->getCanId();
    frameSend.can_dlc = 1;
    frameSend.data[0] = static_cast<uint8_t>(code);
    motorCan.sendFrame(frameSend);
    motorCan.receiveFrame(frameReceive);
    return frameReceive.data[0];
#warning "TODO : not frameReceive.data[0]"
}
void interfaceSetParameter1(ti5Motor *motorHandle, ti5Motor::FunctionCodeTabSend1Receive4 code)
{
    can_frame frameSend;
    frameSend.can_id = motorHandle->getCanId();
    frameSend.can_dlc = 1;
    frameSend.data[0] = static_cast<uint8_t>(code);
    motorCan.sendFrame(frameSend);
}
void interfaceSetParameter5(ti5Motor *motorHandle, ti5Motor::FunctionCodeTabSend5Receive4 code, uint32_t value)
{
    can_frame frameSend;
    frameSend.can_id = motorHandle->getCanId();
    frameSend.can_dlc = 5;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(value);
    frameSend.data[2] = static_cast<uint8_t>(value >> 8);
    frameSend.data[3] = static_cast<uint8_t>(value >> 16);
    frameSend.data[4] = static_cast<uint8_t>(value >> 24);
    motorCan.sendFrame(frameSend);

}
