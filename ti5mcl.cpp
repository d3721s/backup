#include "ti5mcl.h"
#include "tlog.h"
#include "canglue.h"
ti5MotorSetupData::ti5MotorSetupData(uint8_t canId, std::string name,
                                     // uint8_t reductionRatio,
                                     int32_t maxPositiveCurrent, int32_t maxNegativeCurrent,
                                     int32_t maxPositiveAcceleration, int32_t maxNegativeAcceleration,
                                     int32_t maxPositiveVelocity, int32_t maxNegativeVelocity,
                                     int32_t maxPositivePositon, int32_t maxNegativePositon,
                                     int32_t positionOffset)
{
    _canId = canId;
    _name = name;
    //_reductionRatio = reductionRatio;
    _maxPositiveCurrent = maxPositiveCurrent;
    _maxNegativeCurrent = maxNegativeCurrent;
    _maxPositiveAcceleration = maxPositiveAcceleration;
    _maxNegativeAcceleration = maxNegativeAcceleration;
    _maxPositiveVelocity = maxPositiveVelocity;
    _maxNegativeVelocity = maxNegativeVelocity;
    _maxPositivePositon = maxPositivePositon;
    _maxNegativePositon = maxNegativePositon;
    _positionOffset = positionOffset;
}
canid_t  ti5MotorSetupData::getCanId(void) const
{
    return _canId;
}
std::string ti5MotorSetupData::getName(void) const
{
    return _name;
}
// uint8_t ti5MotorSetupData::getReductionRatio(void) const
//{
//     return _reductionRatio;
// }
int32_t ti5MotorSetupData::getMaxPositiveCurrent(void) const
{
    return _maxPositiveCurrent;
}
int32_t ti5MotorSetupData::getMaxNegativeCurrent(void) const
{
    return _maxNegativeCurrent;
}
int32_t ti5MotorSetupData::getMaxPositiveAcceleration(void) const
{
    return _maxPositiveAcceleration;
}
int32_t ti5MotorSetupData::getMaxNegativeAcceleration(void) const
{
    return _maxNegativeAcceleration;
}
int32_t ti5MotorSetupData::getMaxPositivePositon(void) const
{
    return _maxPositivePositon;
}
int32_t ti5MotorSetupData::getMaxNegativePositon(void) const
{
    return _maxNegativePositon;
}
int32_t ti5MotorSetupData::getMaxPositiveVelocity(void) const
{
    return _maxPositiveVelocity;
}

int32_t ti5MotorSetupData::getMaxNegativeVelocity(void) const
{
    return _maxNegativeVelocity;
}

int32_t ti5MotorSetupData::getPositionOffset(void) const
{
    return _positionOffset;
}
void ti5MotorSetupData::setCanId(canid_t canId)
{
    _canId = canId;
}
void ti5MotorSetupData::setName(std::string name)
{
    _name = name;
}
// void ti5MotorSetupData::setReductionRatio(uint8_t reductionRatio)
// {
//     _reductionRatio = reductionRatio;
// }
void ti5MotorSetupData::setMaxPositiveCurrent(int32_t maxPositiveCurrent)
{
    _maxPositiveCurrent = maxPositiveCurrent;
}
void ti5MotorSetupData::setMaxNegativeCurrent(int32_t maxNegativeCurrent)
{
    _maxNegativeCurrent = maxNegativeCurrent;
}
void ti5MotorSetupData::setMaxPositiveAcceleration(int32_t maxPositiveAcceleration)
{
    _maxPositiveAcceleration = maxPositiveAcceleration;
}
void ti5MotorSetupData::setMaxNegativeAcceleration(int32_t maxNegativeAcceleration)
{
    _maxNegativeAcceleration = maxNegativeAcceleration;
}
void ti5MotorSetupData::setMaxPositivePositon(int32_t maxPositivePositon)
{
    _maxPositivePositon = maxPositivePositon;
}
void ti5MotorSetupData::setMaxNegativePositon(int32_t maxNegativePositon)
{
    _maxNegativePositon = maxNegativePositon;
}
void ti5MotorSetupData::setMaxPositiveVelocity(int32_t maxPositiveVelocity)
{
    _maxPositiveVelocity = maxPositiveVelocity;
}
void ti5MotorSetupData::setMaxNegativeVelocity(int32_t maxNegativeVelocity)
{
    _maxNegativeVelocity = maxNegativeVelocity;
}
void ti5MotorSetupData::setPositionOffset(int32_t positionOffset)
{
    _positionOffset = positionOffset;
}

static void preprocess(void)
{
    static std::once_flag motorInitializedFlag;
    std::call_once(motorInitializedFlag, []
    {
        if (tlog_init("ti5motor.log", 1048576, 8, 0, TLOG_SCREEN | TLOG_SCREEN_COLOR) != 0)
        {
            std::cerr << "log system init error" << std::endl;
        }
        tlog_setlevel(TI5MCLLOGLEVEL);
        int status = system("/usr/sh/ti5mclsetup.sh");
        if (status != 0)
        {
            tlog_warn << "shexec failed with status " << std::to_string(status) << std::endl;
        }
        else
        {
            if (WIFEXITED(status))
            {
                int exitStatus = WEXITSTATUS(status);
                if (exitStatus != 0)
                {
                    tlog_warn << "Command execution failed with status: " << std::to_string(exitStatus) << std::endl;
                }
                else
                {
                    tlog_info << "Command execution completed successfully." << std::endl;
                }
            }
            else
            {
                tlog_warn << "Command did not terminate normally." << std::endl;
            }
        }
    });
}
CanBus motorCan("vcan0");
static void canInit(void)
{
    static std::once_flag canInitializedFlag;
    std::call_once(canInitializedFlag, []
    {
        if(motorCan.init() == false)
        {
            tlog_fatal << "can bus init failed" << std::endl;
            exit(1);
        }
    }
                  );
}

ti5Motor::ti5Motor(void)
{
    preprocess();
    canInit();
    tlog_warn << "void ti5Motor created" << std::endl;
}

ti5Motor::ti5Motor(uint8_t canId)
{
    _canId = canId;
    preprocess();
    canInit();
    writeRegister(FunctionCodeTabSend1Receive0::setRestoreFromFlashCode);
    // restoreConfig();
    tlog_info << "ti5Motor(canid:" << std::to_string(canId) << ") created using hardware settings" << std::endl;
}

ti5Motor::ti5Motor(uint8_t canId, ti5MotorSetupData *deviceData)
{
    _deviceData = deviceData;
    preprocess();
    canInit();
    tlog_info << "ti5Motor created canID:" << std::to_string(deviceData->getCanId()) << " name:" << deviceData->getName() << "using software settings" << std::endl;
}

canid_t ti5Motor::getCanId(void)
{
    return _canId;
}

bool ti5Motor::writeRegister(FunctionCodeTabSend1Receive0 code)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 1;
    frameSend.data[0] = static_cast<uint8_t>(code);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(frameSend.data[0]) << std::endl;
}
bool ti5Motor::readRegister(FunctionCodeTabSend1Receive4 code)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 1;
    frameSend.data[0] = static_cast<uint8_t>(code);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(frameSend.data[0]) << std::endl;
    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _uitemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24;
    tlog_debug << "receive:" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[3]) << std::endl;
    tlog_debug << "read:" << std::to_string(_uitemp) << std::endl;
}

bool ti5Motor::readRegister(FunctionCodeTabSend1Receive8 code)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 1;
    frameSend.data[0] = static_cast<uint8_t>(code);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(frameSend.data[0]) << std::endl;
    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24
              | frameReceive.data[4] << 32 | frameReceive.data[5] << 40 | frameReceive.data[6] << 48 | frameReceive.data[7] << 56;
    tlog_debug << "receive:" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[7]) << std::endl;
    tlog_debug << "read:" << std::to_string(_ultemp) << std::endl;
} // csp

bool ti5Motor::writeRegister(FunctionCodeTabSend5Receive0 code, int32_t value)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 5;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(static_cast<uint32_t>(value));
    frameSend.data[2] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 8);
    frameSend.data[3] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 16);
    frameSend.data[4] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 24);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(frameSend.data[0]) << "..." << std::to_string(frameSend.data[4]) << std::endl;
}
bool ti5Motor::writeReadRegister(FunctionCodeTabSend5Receive4 code, int32_t value)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 5;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(static_cast<uint32_t>(value));
    frameSend.data[2] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 8);
    frameSend.data[3] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 16);
    frameSend.data[4] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 24);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(frameSend.data[0]) << "..." << std::to_string(frameSend.data[4]) << std::endl;
    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _uitemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24;
    tlog_debug << "receive:" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[3]) << std::endl;
    tlog_debug << "read:" << std::to_string(_uitemp) << std::endl;
}
bool ti5Motor::writeReadRegister(FunctionCodeTabSend5Receive8 code, int32_t value)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 5;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(static_cast<uint32_t>(value));
    frameSend.data[2] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 8);
    frameSend.data[3] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 16);
    frameSend.data[4] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 24);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(frameSend.data[0]) << "..." << std::to_string(frameSend.data[4]) << std::endl;
    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24
              | frameReceive.data[4] << 32 | frameReceive.data[5] << 40 | frameReceive.data[6] << 48 | frameReceive.data[7] << 56;
    tlog_debug << "receive:" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[7]) << std::endl;
    tlog_debug << "read:" << std::to_string(_ultemp) << std::endl;
}

bool ti5Motor::writeReadRegister(FunctionCodeTabSend6Receive4 code, int64_t value)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 6;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(frameSend.data[0]) << "..." << std::to_string(frameSend.data[5]) << std::endl;

    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _uitemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24;
    tlog_debug << "receive:" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[3]) << std::endl;
    tlog_debug << "read:" << std::to_string(_uitemp) << std::endl;
}
bool ti5Motor::writeReadRegister(FunctionCodeTabSend6Receive7 code, int64_t value)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 6;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(frameSend.data[0]) << "..." << std::to_string(frameSend.data[5]) << std::endl;
    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24
              | frameReceive.data[4] << 32 | frameReceive.data[5] << 40 | frameReceive.data[6] << 48;
    tlog_debug << "receive:" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[6]) << std::endl;
    tlog_debug << "read:" << std::to_string(_ultemp) << std::endl;
}

bool ti5Motor::writeReadRegister(FunctionCodeTabSend7Receive5 code, int64_t value)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 7;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    frameSend.data[6] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 40);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send" << std::to_string(frameSend.data[0]) << "..." << std::to_string(frameSend.data[6]) << std::endl;
    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24
              | frameReceive.data[4] << 32;
    tlog_debug << "receive" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[4]) << std::endl;
    tlog_debug << "read" << std::to_string(_ultemp) << std::endl;
}
bool ti5Motor::writeReadRegister(FunctionCodeTabSend7Receive6 code, int64_t value)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 7;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    frameSend.data[6] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 40);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send" << std::to_string(frameSend.data[0]) << "..." << std::to_string(frameSend.data[6]) << std::endl;
    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24
              | frameReceive.data[4] << 32 | frameReceive.data[5] << 40;
    tlog_debug << "receive" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[5]) << std::endl;
    tlog_debug << "read" << std::to_string(_ultemp) << std::endl;
}

bool ti5Motor::writeReadRegister(FunctionCodeTabSend8Receive8 code, int64_t value)
{
    frameSend.can_id = this->getCanId();
    frameSend.can_dlc = 6;
    frameSend.data[0] = static_cast<uint8_t>(code);
    frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    frameSend.data[6] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 40);
    frameSend.data[7] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 48);
    if(motorCan.sendFrame(frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send" << std::to_string(frameSend.data[0]) << "..." << std::to_string(frameSend.data[7]) << std::endl;
    if (motorCan.receiveFrame(frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = frameReceive.data[0] | frameReceive.data[1] << 8 | frameReceive.data[2] << 16 | frameReceive.data[3] << 24
              | frameReceive.data[4] << 32 | frameReceive.data[5] << 40 | frameReceive.data[6] << 48 | frameReceive.data[7] << 56;
    tlog_debug << "receive" << std::to_string(frameReceive.data[0]) << "..." << std::to_string(frameReceive.data[7]) << std::endl;
    tlog_debug << "read" << std::to_string(_ultemp) << std::endl;
}
