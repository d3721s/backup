#ifndef TI5MCL_H_INCLUDED
#define TI5MCL_H_INCLUDED
#include <cstdint>
#include <iostream>
#include <cstdio>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <unistd.h>
#include <chrono>


#define TI5MCLLOGLEVEL TLOG_DEBUG
#warning "temploglevel"
#ifndef TI5MCLLOGLEVEL
#define TI5MCLLOGLEVEL TLOG_WARN
#endif


class ti5MotorSetupData
{
public:
ti5MotorSetupData(void) = default;
~ti5MotorSetupData(void) = default;
ti5MotorSetupData(uint8_t canId,std::string name,
                  //uint8_t reductionRatio,
                  int32_t maxPositiveCurrent,int32_t maxNegativeCurrent,
                  int32_t maxPositiveAcceleration,int32_t maxNegativeAcceleration,
                  int32_t maxPositiveVelocity,int32_t maxNegativeVelocity,
                  int32_t maxPositivePositon,int32_t maxNegativePositon,
                  int32_t positionOffset
                  );

uint8_t getCanId(void) const;
std::string getName(void) const;

// uint8_t getReductionRatio(void) const; //使用(ti5Motor*)->getReductionRatio()获取减速比

int32_t getMaxPositiveCurrent(void) const;
int32_t getMaxNegativeCurrent(void) const;

int32_t getMaxPositiveAcceleration(void) const;
int32_t getMaxNegativeAcceleration(void) const;

int32_t getMaxPositivePositon(void) const;
int32_t getMaxNegativePositon(void) const;

int32_t getMaxPositiveVelocity(void) const;
int32_t getMaxNegativeVelocity(void) const;

int32_t getPositionOffset(void) const;

void setCanId(uint8_t canId);
void setName(std::string name);
//void setReductionRatio(uint8_t reductionRatio); //减速比不能设置

void setMaxPositiveCurrent(int32_t maxPositiveCurrent);
void setMaxNegativeCurrent(int32_t maxNegativeCurrent);

void setMaxPositiveAcceleration(int32_t maxPositiveAcceleration);
void setMaxNegativeAcceleration(int32_t maxNegativeAcceleration);

void setMaxPositivePositon(int32_t maxPositivePositon);
void setMaxNegativePositon(int32_t maxNegativePositon);

void setMaxPositiveVelocity(int32_t maxPositiveVelocity);
void setMaxNegativeVelocity(int32_t maxNegativeVelocity);

void setPositionOffset(int32_t positionOffset);

private:
uint8_t _canId;
std::string _name;
uint8_t _reductionRatio;
int32_t _maxPositiveCurrent;
int32_t _maxNegativeCurrent;
int32_t _maxPositiveAcceleration;
int32_t _maxNegativeAcceleration;
int32_t _maxPositivePositon;
int32_t _maxNegativePositon;
int32_t _maxPositiveVelocity;
int32_t _maxNegativeVelocity;
int32_t _positionOffset;
};

class ti5Motor
{
public:
ti5Motor(void);//空电机,预留
ti5Motor(uint8_t canId);//使用硬件设置初始化软件对象
ti5Motor(uint8_t canId,ti5MotorSetupData* deviceData);   //使用软件设置初始化软件对象//canId为当前canId，deviceData->canId为将要设置的canId！
~ti5Motor(void) = default;

enum class MotorMode
{
    modeUndefined = -1,
    modeStop = 0,
    modeCurrent = 1,
    modeVelocity = 2,
    modePosition = 3,
};
MotorMode getMotorMode(void);
void setMotorMode(MotorMode mode);
enum class FunctionCodeTab1//get
{
    modeCode = 3,
    currentCode = 4,
    targetCurrentCode = 5,
    velocityCode = 6,
    targetVelocityCode = 7,
    positionCode = 8,
    targetPositionCode = 9,
    errorCode = 10,
    speedLoopKpCode = 16,
    speedLoopKiCode = 17,
    speedLoopKdCode = 51,
    positionLoopKpCode = 18,
    positionLoopKiCode = 52,
    positionLoopKdCode = 19,
    busVoltageCode = 20,
    maxPositiveCurrentCode = 53,
    maxNegativeCurrentCode = 54,
    maxPositiveVelocityCode = 24,
    maxNegativeVelocityCode = 25,
    maxPositivePositionCode = 26,
    maxNegativePositionCode = 27,
    maxPositiveAccelerationCode = 22,
    maxNegativeAccelerationCode = 23,
    motorTemperatureCode = 49,
    driverTemperatureCode = 50,
    currentLoopKpCode = 97,
    currentLoopKiCode = 98,
    currentLoopKdCode = 99,
    motorModelCode = 100,
    motorVersionNumberCode = 101,
    motorSoftwareVersionNumberCode = 102,
    positionOffsetCode = 84,
    cyclicSynchronousPositionCode = 65,
    encoderVoltageCode = 120,
    encoderStatusCode = 121,
    overVoltageThresholdCode = 138,
    lowVoltageThresholdCode = 140,
    motorOverTemperatureThresholdCode = 143,
    driverOverTemperatureThresholdCode = 147,
};
enum class FunctionCodeTab5//set
{
    currentModeCode = 28,
    velocityModeCode = 29,
    positionModeCode = 30,
    currentCode = 66,
    velocityCode = 67,
    positionCode = 68,
    maxPositiveCurrentCode = 32,
    maxNegativeCurrentCode = 33,

    maxPositiveAccelerationCode = 34,
    maxNegativeAccelerationCode = 35,
    maxPositiveVelocityCode = 36,
    maxNegativeVelocityCode = 37,
    maxPositivePositionCode = 38,
    maxNegativePositionCode = 39,
    speedLoopKpCode = 41,
    speedLoopKiCode = 42,
    positionLoopKpCode = 43,
    positionLoopKdCode = 45,
    canIdCode = 46,
    stopMotorCode = 2,
    positionOffsetCode = 83,
    baudRateCode = 63,

};
enum class FunctionCodeTab6//set
{

};
enum class FunctionCodeTab7//set
{

};
enum class FunctionCodeTab8//set
{

};

int32_t getParameter(FunctionCodeTab1 code);
void setParameter(FunctionCodeTab1 code);
int32_t getParameter(FunctionCodeTab5 code);
void setParameter(FunctionCodeTab5 code, int32_t value);


void setParameter(FunctionCodeTab6 code, int32_t value);
void setParameter(FunctionCodeTab7 code, int32_t value);

void setStopMode(void);

uint8_t getCanId(void);
private:

ti5MotorSetupData* _deviceData;
uint8_t _canId;
uint32_t _baudRate;
uint32_t _timeoutPeriod;
uint8_t _reductionRatio;

MotorMode _motorMode;
int32_t _current;
int32_t _targetCurrent;
int32_t _velocity;
int32_t _position;

};
#endif // TI5MCL_H_INCLUDED
