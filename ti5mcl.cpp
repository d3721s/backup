#include "ti5mcl.h"
#include "tlog.h"
#include "interface.h"

ti5MotorSetupData::ti5MotorSetupData(uint8_t canId,std::string name,
                  //uint8_t reductionRatio,
                  int32_t maxPositiveCurrent,int32_t maxNegativeCurrent,
                  int32_t maxPositiveAcceleration,int32_t maxNegativeAcceleration,
                  int32_t maxPositiveVelocity,int32_t maxNegativeVelocity,
                  int32_t maxPositivePositon,int32_t maxNegativePositon,
                  int32_t positionOffset
                  )
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
uint8_t ti5MotorSetupData::getCanId(void) const
{
    return _canId;
}
std::string ti5MotorSetupData::getName(void) const
{
    return _name;
}
//uint8_t ti5MotorSetupData::getReductionRatio(void) const
//{
//    return _reductionRatio;
//}
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
void ti5MotorSetupData::setCanId(uint8_t canId)
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
    std::call_once(motorInitializedFlag,[]
    {
        if(tlog_init("ti5motor.log", 1048576, 8, 0, TLOG_SCREEN|TLOG_SCREEN_COLOR)!=0)
        {
           std::cerr << "log system init error" << std::endl;
        }
        tlog_setlevel(TI5MCLLOGLEVEL);
        int status = system("/usr/sh/ti5mclsetup.sh");
        if (status != 0)
        {
            tlog_warn << "shexec failed with status " << std::to_string(status) << std::endl ;
        }
        else
        {
            if (WIFEXITED(status)) {
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

    }
    );
}

ti5Motor::ti5Motor(void)
{
    preprocess();
    tlog_warn << "void ti5Motor created" << std::endl;
}

ti5Motor::ti5Motor(uint8_t canId)
{
    _canId=canId;
    preprocess();
    interfaceInit();
    interfaceSetParameter1(,);
    //restoreConfig();
    tlog_info << "ti5Motor(canid:"<< std::to_string(canId) <<") created using hardware settings"<<std::endl;
}

ti5Motor::ti5Motor(uint8_t canId,ti5MotorSetupData* deviceData)
{
    _deviceData = deviceData;
    preprocess();
    interfaceInit();
    tlog_info << "ti5Motor created canID:" << std::to_string(deviceData->getCanId()) << " name:" << deviceData->getName() <<"using software settings"<< std::endl;
}

int32_t ti5Motor::getParameter(FunctionCodeTabSend1Receive4 code)
{
    tlog_info << "getting parameter " << std::to_string(static_cast<uint8_t>(code)) << std::endl;
    return interfaceGetParameter1(this,code);
}
int32_t ti5Motor::getParameter(FunctionCodeTabSend5Receive4 code)
{
    tlog_info << "getting parameter " << std::to_string(static_cast<uint8_t>(code)) << std::endl;
    return interfaceGetParameter5(this,code);
}


void ti5Motor::setParameter(FunctionCodeTabSend1Receive4 code)
{
    interfaceSetParameter1(this,code);
}


uint8_t ti5Motor::getCanId(void)
{
    return _canId;
}







