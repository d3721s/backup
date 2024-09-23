#include "ti5mcl.h"
#include "tlog.h"
#include "canglue.h"
ti5MotorSetupData::ti5MotorSetupData(uint8_t canId, std::string name,
                                     uint8_t reductionRatio,
                                     int32_t maxPositiveCurrent, int32_t maxNegativeCurrent,
                                     int32_t maxPositiveAcceleration, int32_t maxNegativeAcceleration,
                                     int32_t maxPositiveVelocity, int32_t maxNegativeVelocity,
                                     int32_t maxPositivePositon, int32_t maxNegativePositon,
                                     int32_t positionOffset)
{
    _canId = canId;
    _name = name;
    _reductionRatio = reductionRatio;
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
CanBus motorCan(CANDEVICE);
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

ti5Motor::ti5Motor(uint8_t canId, reductionRatio reductionRatioValue)
{
    _canId = canId;
    _reductionRatio=reductionRatioValue;
    preprocess();
    canInit();
    writeRegister(FunctionCodeTabSend1Receive0::setRestoreFromFlashCode);
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

ti5Motor::reductionRatio ti5Motor::getReductionRatio(void)
{
    return _reductionRatio;
}

/// @brief
/// @param
/// @return
bool ti5Motor::getMotorMode(MotorMode* mode)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getModeCode)==false)
    {
        tlog_error << "getMotorMode failed" << std::endl;
        return false;
    }
    tlog_info << "getMotorMode: " << std::to_string(_uitemp) << std::endl;
    *mode = static_cast<MotorMode>(_uitemp);
    return true;
}
bool ti5Motor::setMotorMode(MotorMode mode)
{
    MotorMode oldMode;
    if(getMotorMode(&oldMode)==false)
        return false;
    if(mode != oldMode)
    {
        switch(mode)
        {
        case MotorMode::modeStop:
            if(writeRegister(FunctionCodeTabSend1Receive0::setStopModeCode) == false)
        {
            tlog_error << "setMotorMode failed" << std::endl;
            return false;
        }
        case MotorMode::modeCurrent:
            if(writeRegister(FunctionCodeTabSend5Receive0::setCurrentModeCode, 0) == false)
        {
            tlog_error << "setMotorMode failed" << std::endl;
            return false;
        }
        case MotorMode::modeVelocity:
            if(writeReadRegister(FunctionCodeTabSend5Receive4::setVelocityModeCode, 0) == false)
        {
            tlog_error << "setMotorMode failed" << std::endl;
            return false;
        }
        case MotorMode::modePosition:
            if(writeReadRegister(FunctionCodeTabSend5Receive4::setPositionModeCode, 0) == false)
        {
            tlog_error << "setMotorMode failed" << std::endl;
            return false;
        }
        default:
            tlog_error << "unsupported mode:" << std::to_string(static_cast<uint8_t> (mode)) << std::endl;
            return false;
        }
        MotorMode newMode;
        if(getMotorMode(&newMode)==false)
        {
            tlog_error << "setMotorMode failed" << std::endl;
            return false;
        }
        if(newMode != mode)
        {
            tlog_error << "setMotorMode failed" << std::endl;
            return false;
        }
    }
    tlog_info << "setMotorMode: " << std::to_string(_uitemp) << std::endl;
    return true;
}

bool ti5Motor::getCurrent(int32_t* current)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getCurrentCode) == false)
    {
        tlog_error << "getCurrent failed" << std::endl;
        return false;
    }
    tlog_info << "getCurrent: " << std::to_string(_uitemp) << std::endl;
    *current = _uitemp;//ma
    return true;
}
bool ti5Motor::getTargetCurrent(int32_t* current)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getTargetCurrentCode) == false)
    {
        tlog_error << "gettargetCurrent failed" << std::endl;
        return false;
    }
    tlog_info << "gettargetCurrent: " << std::to_string(_uitemp) << std::endl;
    *current = _uitemp;//ma
    return true;

}
bool ti5Motor::getVelocity(int32_t* velocity)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getVelocityCode) == false)
    {
        tlog_error << "getVelocity failed" << std::endl;
        return false;
    }
    tlog_info << "getVelocity: " << std::to_string(_uitemp) << std::endl;
    *velocity = _uitemp;//ma
    return true;
}
bool ti5Motor::getTargetVelocity(int32_t*   targetVelocity)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getTargetVelocityCode) == false)
    {
        tlog_error << "getTargetVelocity failed" << std::endl;
        return false;
    }
    tlog_info << "getTargetVelocity: " << std::to_string(_uitemp) << std::endl;
    *targetVelocity = _uitemp;//ma
    return true;

}
bool ti5Motor::getPosition(int32_t* position)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getPositionCode) == false)
    {
        tlog_error << "getPosition failed" << std::endl;
        return false;
    }
    tlog_info << "getPosition: " << std::to_string(_uitemp) << std::endl;
    *position = _uitemp;//ma
    return true;
}
bool ti5Motor::getTargetPosition(int32_t* targetPosition)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getTargetPositionCode) == false)
    {
        tlog_error << "getTargetPosition failed" << std::endl;
        return false;
    }
    tlog_info << "getTargetPosition: " << std::to_string(_uitemp) << std::endl;
    *targetPosition = _uitemp;//ma
    return true;
}
bool ti5Motor::getErrorStatus(errorStatus* errorStatus)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getErrorStatusCode) == false)
    {
        tlog_error << "getErrorStatus failed" << std::endl;
        return false;
    }
    tlog_info << "getErrorStatus: " << std::to_string(_uitemp) << std::endl;
    *errorStatus = static_cast<errorStatus>(_uitemp);
    return true;
}
bool ti5Motor::getMotorTemperature(int32_t* motorTemperature)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getMotorTemperatureCode) == false)
    {
        tlog_error << "getMotorTemperature failed" << std::endl;
        return false;
    }
    tlog_info << "getMotorTemperature: " << std::to_string(_uitemp) << std::endl;
    *motorTemperature = _uitemp;
    return true;
}
bool ti5Motor::getDriverTemperature(int32_t* driverTemperature)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getDriverTemperatureCode) == false)
    {
        tlog_error << "getDriverTemperature failed" << std::endl;
        return false;
    }
    tlog_info << "getDriverTemperature: " << std::to_string(_uitemp) << std::endl;
    *driverTemperature = _uitemp;
    return true;
}//建议使用autoMonitor()
bool ti5Motor::getCyclicSynchronousPosition(int32_t* cyclicSynchronousPosition){}
#warning 

bool ti5Motor::setTargetCurrent(int32_t targetCurrent)
{
    
}
bool ti5Motor::setTargetVelocity(int32_t targetVelocity){}
bool ti5Motor::setTargetPosition(int32_t targetPosition){}
bool ti5Motor::setCleanError(){}
bool ti5Motor::quickReset(){}
bool ti5Motor::quickHome(){}
bool ti5Motor::quickHalt(){}
bool ti5Motor::quickMoveAbsolute(int32_t position){}
bool ti5Motor::quickMoveAbsolute(int16_t positionInDegree){}
bool ti5Motor::quickMoveRelative(int32_t position){}
bool ti5Motor::quickMoveRelative(int16_t positionInDegree){}
bool ti5Motor::quickMoveVelocity(int32_t velocity){}
bool ti5Motor::quickMoveVelocity(int16_t perSecondDegree){}
bool ti5Motor::quickMoveJog(){}
bool ti5Motor::autoMonitor(){}
bool ti5Motor::autoCyclicSynchronousPosition(){}
///////////////////////////////////////////////////////////////////////

bool ti5Motor::writeRegister(FunctionCodeTabSend1Receive0 code)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 1;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(_frameSend.data[0]) << std::endl;
    return true;
}
bool ti5Motor::readRegister(FunctionCodeTabSend1Receive4 code)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 1;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(_frameSend.data[0]) << std::endl;
    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _uitemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24;
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[3]) << std::endl;
    tlog_debug << "read:" << std::to_string(_uitemp) << std::endl;
    return true;
}

bool ti5Motor::readRegister(FunctionCodeTabSend1Receive8 code)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 1;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(_frameSend.data[0]) << std::endl;
    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24
              | _frameReceive.data[4] << 32 | _frameReceive.data[5] << 40 | _frameReceive.data[6] << 48 | _frameReceive.data[7] << 56;
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[7]) << std::endl;
    tlog_debug << "read:" << std::to_string(_ultemp) << std::endl;
    return true;
} // csp

bool ti5Motor::writeRegister(FunctionCodeTabSend5Receive0 code, int32_t value)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 5;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    _frameSend.data[1] = static_cast<uint8_t>(static_cast<uint32_t>(value));
    _frameSend.data[2] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 8);
    _frameSend.data[3] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 16);
    _frameSend.data[4] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 24);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(_frameSend.data[0]) << "..." << std::to_string(_frameSend.data[4]) << std::endl;

    return true;
}
bool ti5Motor::writeReadRegister(FunctionCodeTabSend5Receive4 code, int32_t value)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 5;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    _frameSend.data[1] = static_cast<uint8_t>(static_cast<uint32_t>(value));
    _frameSend.data[2] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 8);
    _frameSend.data[3] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 16);
    _frameSend.data[4] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 24);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(_frameSend.data[0]) << "..." << std::to_string(_frameSend.data[4]) << std::endl;
    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _uitemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24;
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[3]) << std::endl;
    tlog_debug << "read:" << std::to_string(_uitemp) << std::endl;
    return true;
}
bool ti5Motor::writeReadRegister(FunctionCodeTabSend5Receive8 code, int32_t value)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 5;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    _frameSend.data[1] = static_cast<uint8_t>(static_cast<uint32_t>(value));
    _frameSend.data[2] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 8);
    _frameSend.data[3] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 16);
    _frameSend.data[4] = static_cast<uint8_t>(static_cast<uint32_t>(value) >> 24);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(_frameSend.data[0]) << "..." << std::to_string(_frameSend.data[4]) << std::endl;
    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24
              | _frameReceive.data[4] << 32 | _frameReceive.data[5] << 40 | _frameReceive.data[6] << 48 | _frameReceive.data[7] << 56;
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[7]) << std::endl;
    tlog_debug << "read:" << std::to_string(_ultemp) << std::endl;
    return true;
}

bool ti5Motor::writeReadRegister(FunctionCodeTabSend6Receive4 code, int64_t value)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 6;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    _frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    _frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    _frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    _frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    _frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(_frameSend.data[0]) << "..." << std::to_string(_frameSend.data[5]) << std::endl;

    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _uitemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24;
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[3]) << std::endl;
    tlog_debug << "read:" << std::to_string(_uitemp) << std::endl;
    return true;
}
bool ti5Motor::writeReadRegister(FunctionCodeTabSend6Receive7 code, int64_t value)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 6;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    _frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    _frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    _frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    _frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    _frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send:" << std::to_string(_frameSend.data[0]) << "..." << std::to_string(_frameSend.data[5]) << std::endl;
    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24
              | _frameReceive.data[4] << 32 | _frameReceive.data[5] << 40 | _frameReceive.data[6] << 48;
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[6]) << std::endl;
    tlog_debug << "read:" << std::to_string(_ultemp) << std::endl;
    return true;
}

bool ti5Motor::writeReadRegister(FunctionCodeTabSend7Receive5 code, int64_t value)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 7;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    _frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    _frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    _frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    _frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    _frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    _frameSend.data[6] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 40);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send" << std::to_string(_frameSend.data[0]) << "..." << std::to_string(_frameSend.data[6]) << std::endl;
    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24
              | _frameReceive.data[4] << 32;
    tlog_debug << "receive" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[4]) << std::endl;
    tlog_debug << "read" << std::to_string(_ultemp) << std::endl;
    return true;
}
bool ti5Motor::writeReadRegister(FunctionCodeTabSend7Receive6 code, int64_t value)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 7;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    _frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    _frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    _frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    _frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    _frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    _frameSend.data[6] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 40);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send" << std::to_string(_frameSend.data[0]) << "..." << std::to_string(_frameSend.data[6]) << std::endl;
    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24
              | _frameReceive.data[4] << 32 | _frameReceive.data[5] << 40;
    tlog_debug << "receive" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[5]) << std::endl;
    tlog_debug << "read" << std::to_string(_ultemp) << std::endl;
    return true;
}

bool ti5Motor::writeReadRegister(FunctionCodeTabSend8Receive8 code, int64_t value)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 6;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    _frameSend.data[1] = static_cast<uint8_t>(static_cast<uint64_t>(value));
    _frameSend.data[2] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 8);
    _frameSend.data[3] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 16);
    _frameSend.data[4] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 24);
    _frameSend.data[5] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 32);
    _frameSend.data[6] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 40);
    _frameSend.data[7] = static_cast<uint8_t>(static_cast<uint64_t>(value) >> 48);
    if(motorCan.sendFrame(_frameSend) == false)
    {
        tlog_error << "send failed" << std::endl;
        return false;
    }
    tlog_debug << "send" << std::to_string(_frameSend.data[0]) << "..." << std::to_string(_frameSend.data[7]) << std::endl;
    if (motorCan.receiveFrame(_frameReceive) == false)
    {
        tlog_error << "receive failed" << std::endl;
        return false;
    }
    _ultemp = _frameReceive.data[0] | _frameReceive.data[1] << 8 | _frameReceive.data[2] << 16 | _frameReceive.data[3] << 24
              | _frameReceive.data[4] << 32 | _frameReceive.data[5] << 40 | _frameReceive.data[6] << 48 | _frameReceive.data[7] << 56;
    tlog_debug << "receive" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[7]) << std::endl;
    tlog_debug << "read" << std::to_string(_ultemp) << std::endl;
    return true;

}
