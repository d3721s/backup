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

static void logInit(void)
{
    static std::once_flag logInitializedFlag;
    std::call_once(logInitializedFlag, []
    {
        if (tlog_init("ti5motor.log", 1048576, 8, 0, TLOG_SCREEN | TLOG_SCREEN_COLOR) != 0)
        {
            std::cerr << "log system init error" << std::endl;
        }
        tlog_setlevel(TI5MCLLOGLEVEL);
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
    logInit();
    canInit();
    tlog_warn << "void ti5Motor created" << std::endl;
}

ti5Motor::ti5Motor(uint8_t canId, reductionRatio reductionRatioValue)
{
    _deviceData->setCanId(canId);
    _canId = canId;
    _deviceData->setReductionRatio(static_cast<uint8_t>(reductionRatioValue));
    _reductionRatio = reductionRatioValue;

    logInit();
    canInit();
    writeRegister(FunctionCodeTabSend1Receive0::setRestoreFromFlashCode);
    _deviceData->setName(std::string("ti5Motor") + std::to_string(canId));
    int32_t maxPositiveCurrent, maxNegativeCurrent, maxPositiveAcceleration, maxNegativeAcceleration, maxPositivePositon, maxNegativePositon, maxPositiveVelocity, maxNegativeVelocity, positionOffset;
    readRegister(FunctionCodeTabSend1Receive4::getMaxPositiveCurrentCode, &maxPositiveCurrent);
    _deviceData->setsetMaxPositiveCurrent(maxPositiveCurrent);
    readRegister(FunctionCodeTabSend1Receive4::getMaxNegativeCurrentCode, &maxNegativeCurrent);
    _deviceData->setMaxNegativeCurrent(maxNegativeCurrent);
    readRegister(FunctionCodeTabSend1Receive4::getMaxPositiveAccelerationCode, &maxPositiveAcceleration);
    _deviceData->setMaxPositiveAcceleration(maxPositiveAcceleration);
    readRegister(FunctionCodeTabSend1Receive4::getMaxNegativeAccelerationCode, &maxNegativeAcceleration);
    _deviceData->setMaxNegativeAcceleration(maxNegativeAcceleration);
    readRegister(FunctionCodeTabSend1Receive4::getMaxPositivePositonCode, &maxPositivePositon);
    _deviceData->setMaxPositivePositon(maxPositivePositon);
    readRegister(FunctionCodeTabSend1Receive4::getMaxNegativePositonCode, &maxNegativePositon);
    _deviceData->setMaxNegativePositon(maxNegativePositon);
    readRegister(FunctionCodeTabSend1Receive4::getMaxPositiveVelocityCode, &maxPositiveVelocity);
    _deviceData->setMaxPositiveVelocity(maxPositiveVelocity);
    readRegister(FunctionCodeTabSend1Receive4::getMaxNegativeVelocityCode, &maxNegativeVelocity);
    _deviceData->setMaxNegativeVelocity(maxNegativeVelocity);
    readRegister(FunctionCodeTabSend1Receive4::getPositionOffsetCode, &positionOffset)
    _deviceData->setPositionOffset(positionOffset);
    tlog_info << "ti5Motor(canid:" << std::to_string(canId) << ") created using hardware settings" << std::endl;
}

ti5Motor::ti5Motor(uint8_t canId, ti5MotorSetupData *deviceData)
{
    _deviceData = deviceData;
    logInit();
    canInit();
    //    setCanId(_deviceData->getCanId());

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
bool ti5Motor::getMotorMode(MotorMode *mode)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getModeCode) == false)
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
    if(getMotorMode(&oldMode) == false)
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
            if(writeRegister(FunctionCodeTabSend5Receive0::setVelocityModeCode, 0) == false)
            {
                tlog_error << "setMotorMode failed" << std::endl;
                return false;
            }
        case MotorMode::modePosition:
            if(writeRegister(FunctionCodeTabSend5Receive0::setPositionModeCode, 0) == false)
            {
                tlog_error << "setMotorMode failed" << std::endl;
                return false;
            }
        default:
            tlog_error << "unsupported mode:" << std::to_string(static_cast<uint8_t> (mode)) << std::endl;
            return false;
        }
        MotorMode newMode;
        if(getMotorMode(&newMode) == false)
        {
            tlog_error << "setMotorMode failed" << std::endl;
            return false;
        }
        if(newMode != mode)
        {
            tlog_error << "setMotorMode failedfailed" << std::endl;
            return false;
        }
    }
    tlog_info << "setMotorMode: " << std::to_string(_sitemp) << std::endl;
    return true;
}

bool ti5Motor::getCurrent(int32_t *current)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getCurrentCode) == false)
    {
        tlog_error << "getCurrent failed" << std::endl;
        return false;
    }
    tlog_info << "getCurrent: " << std::to_string(_sitemp) << std::endl;
    *current = _sitemp;//ma
    return true;
}
bool ti5Motor::getTargetCurrent(int32_t *current)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getTargetCurrentCode) == false)
    {
        tlog_error << "gettargetCurrent failed" << std::endl;
        return false;
    }
    tlog_info << "gettargetCurrent: " << std::to_string(_sitemp) << std::endl;
    *current = _sitemp;//ma
    return true;

}
bool ti5Motor::getVelocity(int32_t *velocity)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getVelocityCode) == false)
    {
        tlog_error << "getVelocity failed" << std::endl;
        return false;
    }
    tlog_info << "getVelocity: " << std::to_string(_sitemp) << std::endl;
    *velocity = _sitemp;//ma
    return true;
}
bool ti5Motor::getTargetVelocity(int32_t   *targetVelocity)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getTargetVelocityCode) == false)
    {
        tlog_error << "getTargetVelocity failed" << std::endl;
        return false;
    }
    tlog_info << "getTargetVelocity: " << std::to_string(_sitemp) << std::endl;
    *targetVelocity = _sitemp;//ma
    return true;

}
bool ti5Motor::getPosition(int32_t *position)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getPositionCode) == false)
    {
        tlog_error << "getPosition failed" << std::endl;
        return false;
    }
    tlog_info << "getPosition: " << std::to_string(_sitemp) << std::endl;
    *position = _sitemp;//ma
    return true;
}
bool ti5Motor::getTargetPosition(int32_t *targetPosition)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getTargetPositionCode) == false)
    {
        tlog_error << "getTargetPosition failed" << std::endl;
        return false;
    }
    tlog_info << "getTargetPosition: " << std::to_string(_sitemp) << std::endl;
    *targetPosition = _sitemp;//ma
    return true;
}
bool ti5Motor::getErrorStatus(errorStatus *errorStatus)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getErrorCode) == false)
    {
        tlog_error << "getErrorStatus failed" << std::endl;
        return false;
    }
    tlog_info << "getErrorStatus: " << std::to_string(_sitemp) << std::endl;
    memcpy(errorStatus, &_sitemp, sizeof(*errorStatus));
    return true;
}
bool ti5Motor::getMotorTemperature(int32_t *motorTemperature)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getMotorTemperatureCode) == false)
    {
        tlog_error << "getMotorTemperature failed" << std::endl;
        return false;
    }
    tlog_info << "getMotorTemperature: " << std::to_string(_sitemp) << std::endl;
    *motorTemperature = _sitemp;
    return true;
}
bool ti5Motor::getDriverTemperature(int32_t *driverTemperature)
{
    if(readRegister(FunctionCodeTabSend1Receive4::getDriverTemperatureCode) == false)
    {
        tlog_error << "getDriverTemperature failed" << std::endl;
        return false;
    }
    tlog_info << "getDriverTemperature: " << std::to_string(_sitemp) << std::endl;
    *driverTemperature = _sitemp;
    return true;
}//建议使用autoMonitor()

bool ti5Motor::getCurrentSpeedPosition(currentSpeedPosition *currentSpeedPosition)
{


}
#warning "int32->int16+"

bool ti5Motor::setTargetCurrent(int32_t targetCurrent)
{
    targetCurrent = std::max(targetCurrent, _deviceData->getMaxNegativeCurrent());
    targetCurrent = std::min(targetCurrent, _deviceData->getMaxPositiveCurrent());
    if(writeRegister(FunctionCodeTabSend5Receive0::setCurrentModeCode, targetCurrent) == false)
    {
        tlog_error << "setTargetCurrent failed" << std::endl;
        return false;
    }

    tlog_info << "setTargetCurrent: " << std::to_string(targetCurrent) << std::endl;
    //processCurrentSpeedPosition(_ultemp);
    return true;
}
bool ti5Motor::setTargetVelocity(int32_t targetVelocity)
{
    targetVelocity = std::max(targetVelocity, _deviceData->getMaxNegativeVelocity());
    targetVelocity = std::min(targetVelocity, _deviceData->getMaxPositiveVelocity());
    if(writeRegister(FunctionCodeTabSend5Receive0::setVelocityModeCode, targetVelocity) == false)
    {
        tlog_error << "setTargetVelocity failed" << std::endl;
        return false;
    }
    tlog_info << "setTargetVelocity: " << std::to_string(targetVelocity) << std::endl;
    return true;
}
bool ti5Motor::setTargetPosition(int32_t targetPosition)
{
    targetPosition = std::max(targetPosition, _deviceData->getMaxNegativePositon());
    targetPosition = std::min(targetPosition, _deviceData->getMaxPositivePositon());
    if(writeRegister(FunctionCodeTabSend5Receive0::setPositionModeCode, targetPosition) == false)
    {
        tlog_error << "setTargetPosition failed" << std::endl;
        return false;
    }
    tlog_info << "setTargetPosition: " << std::to_string(targetPosition) << std::endl;
    return true;
}
bool ti5Motor::setCleanError()
{
    if(writeRegister(FunctionCodeTabSend1Receive0::setCleanErrorCode) == false)
    {
        tlog_error << "setCleanError failed" << std::endl;
        return false;
    }
    tlog_info << "setCleanError" << std::endl;
    return true;
}

// bool ti5Motor::quickReset(){}
bool ti5Motor::quickHome()
{
    return setTargetPosition(0);
}
bool ti5Motor::quickHalt()
{
    return setTargetVelocity(0);
}
bool ti5Motor::quickMoveAbsolute(int32_t position)
{
    return setTargetPosition(position);
}
bool ti5Motor::quickMoveAbsoluteInDegree(int16_t positionInDegree)
{
    return quickMoveAbsolute(positionInDegree * (static_cast<uint8_t>(_reductionRatio) << 13) / 45);
}
bool ti5Motor::quickMoveRelative(int32_t position)
{
    int32_t nowPosition = 0;
    getPosition(&nowPosition);
    return setTargetPosition(nowPosition + position);
}
bool ti5Motor::quickMoveRelativeInDegree(int16_t positionInDegree)
{
    return quickMoveRelative(positionInDegree * (static_cast<uint8_t>(_reductionRatio) << 13) / 45);
}
bool ti5Motor::quickMoveVelocity(int32_t velocity)
{
    return setTargetVelocity(velocity);
}
bool ti5Motor::quickMoveVelocityInDegree(int16_t perSecondInDegree)
{
#warning "TODO:wrong!"
    return quickMoveVelocity(perSecondInDegree * (static_cast<uint8_t>(_reductionRatio) << 13) / 45);
}
bool ti5Motor::quickMoveJog()
{
    return quickMoveRelative(5);
}
bool ti5Motor::autoMonitor()
{
    //pthread_create(&_monitorThread, nullptr, monitorThread, this);
    tlog_info << "autoMonitorThreadStart" << std::endl;
}
bool ti5Motor::autoCurrentSpeedPosition()
{
    //pthread_create(&_currentSpeedPositionThread, nullptr, currentSpeedPositionThread, this);
    tlog_info << "autoCurrentSpeedPositionThreadStart" << std::endl;
}

bool ti5Motor::writeRegister(FunctionCodeTabSend1Receive0 code)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 1;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    std::lock_guard<std::mutex> lock(canMutex);
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    _sitemp = (_frameReceive.data[1])  | (_frameReceive.data[2] << 8) | ( _frameReceive.data[3] << 16 ) | (_frameReceive.data[4] << 24) ;
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << std::to_string(_frameReceive.data[1]) << std::to_string(_frameReceive.data[2]) << std::to_string(_frameReceive.data[3]) << std::endl;
    tlog_debug << "read:" << std::to_string(_sitemp) << std::endl;
    return true;
}

bool ti5Motor::readRegister(FunctionCodeTabSend1Receive8 code)
{
    _frameSend.can_id = this->getCanId();
    _frameSend.can_dlc = 1;
    _frameSend.data[0] = static_cast<uint8_t>(code);
    std::lock_guard<std::mutex> lock(canMutex);
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
    _ultemp = (_frameReceive.data[0]) | (_frameReceive.data[1] << 8) | (_frameReceive.data[2] << 16) | (_frameReceive.data[3] << 24)
              | (_frameReceive.data[4] << 32) | (_frameReceive.data[5] << 40) | (_frameReceive.data[6] << 48) | (_frameReceive.data[7] << 56);
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << std::to_string(_frameReceive.data[1]) << std::to_string(_frameReceive.data[2]) << std::to_string(_frameReceive.data[3])
               << std::to_string(_frameReceive.data[4]) << std::to_string(_frameReceive.data[5]) << std::to_string(_frameReceive.data[6]) << std::to_string(_frameReceive.data[7]) << std::endl;
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    _sitemp = (_frameReceive.data[1]) | (_frameReceive.data[2] << 8) | (_frameReceive.data[3] << 16) | (_frameReceive.data[4] << 24);
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << std::to_string(_frameReceive.data[1]) << std::to_string(_frameReceive.data[2]) << std::to_string(_frameReceive.data[3]) << std::endl;
    tlog_debug << "read:" << std::to_string(_sitemp) << std::endl;
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    _ultemp = (_frameReceive.data[0]) | (_frameReceive.data[1] << 8) | (_frameReceive.data[2] << 16) | (_frameReceive.data[3] << 24)
              | (_frameReceive.data[4] << 32) | (_frameReceive.data[5] << 40) | (_frameReceive.data[6] << 48) | (_frameReceive.data[7] << 56);
    tlog_debug << "receive:" << std::to_string(_frameReceive.data[0]) << std::to_string(_frameReceive.data[1]) << std::to_string(_frameReceive.data[2]) << std::to_string(_frameReceive.data[3])
               << std::to_string(_frameReceive.data[4]) << std::to_string(_frameReceive.data[5]) << std::to_string(_frameReceive.data[6]) << std::to_string(_frameReceive.data[7]) << std::endl;
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    _sitemp =  (_frameReceive.data[1]) | (_frameReceive.data[2] << 8) | (_frameReceive.data[3] << 16) | (_frameReceive.data[4] << 24);
    tlog_debug << "receive:"
               << std::to_string(_frameReceive.data[0])
               << std::to_string(_frameReceive.data[1])
               << std::to_string(_frameReceive.data[2])
               << std::to_string(_frameReceive.data[3])
               << std::to_string(_frameReceive.data[4])
               << std::endl;
    tlog_debug << "read:" << std::to_string(_sitemp) << std::endl;
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    _ultemp = (_frameReceive.data[1]) | (_frameReceive.data[2] << 8) | (_frameReceive.data[3] << 16)
              | (_frameReceive.data[4] << 24) | (_frameReceive.data[5] << 32) | ( _frameReceive.data[6] << 40);
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    _ultemp = (_frameReceive.data[1])  | (_frameReceive.data[2] << 8) | (_frameReceive.data[3] << 16)
              | (_frameReceive.data[4] << 24) | (_frameReceive.data[5] << 32);
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    _ultemp = (_frameReceive.data[1])  | (_frameReceive.data[2] << 8) | (_frameReceive.data[3] << 16)
              | (_frameReceive.data[4] << 24) | (_frameReceive.data[5] << 32);
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
    std::lock_guard<std::mutex> lock(canMutex);
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
    _ultemp = (_frameReceive.data[0]) | _frameReceive.data[1] << 8 | (_frameReceive.data[2] << 16) | (_frameReceive.data[3] << 24)
              | (_frameReceive.data[4] << 32) | (_frameReceive.data[5] << 40) | (_frameReceive.data[6] << 48) | (_frameReceive.data[7] << 56);
    tlog_debug << "receive" << std::to_string(_frameReceive.data[0]) << "..." << std::to_string(_frameReceive.data[7]) << std::endl;
    tlog_debug << "read" << std::to_string(_ultemp) << std::endl;
    return true;
}
