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
//基础方法
ti5Motor(void);//空电机,预留
ti5Motor(uint8_t canId);//使用硬件设置初始化软件对象
ti5Motor(uint8_t canId,ti5MotorSetupData* deviceData);   //使用软件设置初始化软件对象//canId为当前canId，deviceData->_canId为将要设置的canId！
~ti5Motor(void) = default;
//常用方法
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

//全部方法
enum class FunctionCodeTabSend1Receive0 //set
{
    setStopMotorCode = 2, // 停止电机
    setRestoreFromFlashCode = 13,     // 从Flash恢复参数
    setSaveToFlashCode = 14,          // 保存参数到Flash
    setRestorFactoryCode = 15,            // 恢复出厂设置
//  setStoreToFactoryCode = 31,         // 储存参数到出厂
};
enum class FunctionCodeTabSend1Receive4 // get
{
    getModeCode = 3,                  // 模式
    getCurrentCode = 4,               // 当前电流
    getTargetCurrentCode = 5,         // 目标电流
    getVelocityCode = 6,              // 速度
    getTargetVelocityCode = 7,        // 目标速度
    getPositionCode = 8,              // 位置
    getTargetPositionCode = 9,        // 目标位置
    getErrorCode = 10,                // 错误
    getSpeedLoopKpCode = 16,          // 速度环P增益
    getSpeedLoopKiCode = 17,          // 速度环I增益
    getPositionLoopKpCode = 18,       // 位置环P增益
    getPositionLoopKdCode = 19,       // 位置环D增益
    getBusVoltageCode = 20,           // 总线电压
    getMaxPositiveAccelerationCode = 22,  // 最大正向加速度
    getMaxNegativeAccelerationCode = 23,  // 最大负向加速度
    getMaxPositiveVelocityCode = 24,      // 最大正向速度
    getMaxNegativeVelocityCode = 25,      // 最大负向速度
    getMaxPositivePositionCode = 26,      // 最大正向位置
    getMaxNegativePositionCode = 27,      // 最大负向位置
    getMotorTemperatureCode = 49,         // 电机温度
    getDriverTemperatureCode = 50,        // 驱动器温度
    getSpeedLoopKdCode = 51,          // 速度环D增益
    getPositionLoopKiCode = 52,       // 位置环I增益
    getMaxPositiveCurrentCode = 53,       // 最大正向电流
    getMaxNegativeCurrentCode = 54,       // 最大负向电流
    getPositionOffsetCode = 84,       // 位置偏移
    getCurrentLoopKpCode = 97,        // 电流环P增益
    getCurrentLoopKiCode = 98,        // 电流环I增益
    getCurrentLoopKdCode = 99,        // 电流环D增益
    getMotorModelCode = 100,              // 电机型号
    getMotorVersionNumberCode = 101,      // 电机版本号
    getMotorSoftwareVersionNumberCode = 102, // 电机软件版本号
    getEncoderVoltageCode = 120,          // 编码器电压
    getEncoderStatusCode = 121,           // 编码器状态
    getOverVoltageThresholdCode = 138,    // 过压阈值
    getLowVoltageThresholdCode = 140,     // 低压阈值
    getMotorOverTemperatureThresholdCode = 143, // 电机过温阈值
    getDriverOverTemperatureThresholdCode = 147  // 驱动器过温阈值
};
enum class FunctionCodeTabSend1Receive8 //get
{
    getCyclicSynchronousPositionCode = 65, // 循环同步位置
};
enum class FunctionCodeTabSend5Receive0
{ 
    setCurrentModeCode = 28, // 电流模式
};
enum class FunctionCodeTabSend5Receive4 // set
{
    setStopMotorCode = 2, // 停止电机
    setVelocityModeCode = 29, // 速度模式
    setPositionModeCode = 30, // 位置模式
    setMaxPositiveCurrentCode = 32, // 最大正向电流
    setMaxNegativeCurrentCode = 33, // 最大负向电流
    setMaxPositiveAccelerationCode = 34, // 最大正向加速度
    setMaxNegativeAccelerationCode = 35, // 最大负向加速度
    setMaxPositiveVelocityCode = 36, // 最大正向速度
    setMaxNegativeVelocityCode = 37, // 最大负向速度
    setMaxPositivePositionCode = 38, // 最大正向位置
    setMaxNegativePositionCode = 39, // 最大负向位置
    setSpeedLoopKpCode = 41, // 速度环比例增益
    setSpeedLoopKiCode = 42, // 速度环积分增益
    setPositionLoopKpCode = 43, // 位置环比例增益
    setPositionLoopKiCode = 44, // 位置环积分增益
    setPositionLoopKdCode = 45, // 位置环微分增益
    setCanIdCode = 46, // CAN ID
    setBaudRateCode = 63, // 波特率
    // setEncoderZeroCode = 80, // 设置编码器零点
    setPositionOffsetCode = 83, // 位置偏移
    // setDualEncoderCode = 85, // 双编码器
    setOverVoltageThresholdCode = 135,  // 过压阈值
    setLowVoltageThresholdCode = 137, // 低压阈值
};
enum class FunctionCodeTabSend5Receive8 // set
{
    setCurrentCode = 66, // 实际电流
    setVelocityCode = 67, // 实际速度
    setPositionCode = 68, // 实际位置
};

enum class FunctionCodeTabSend6Receive4 // set
{

};
enum class FunctionCodeTabSend6Receive7 // set
{

};


enum class FunctionCodeTabSend7Receive55 // set
{

};
enum class FunctionCodeTabSend7Receive6 // set
{

};
enum class FunctionCodeTabSend8Receive8 // set
{

};



void getParameter(FunctionCodeTabSend1Receive4 code);
void setParameter(FunctionCodeTabSend1Receive0 code);
void getParameter(FunctionCodeTabSend1Receive8 code);//csp

void setParameter(FunctionCodeTabSend5Receive0 code,int32_t value);
void setParameter(FunctionCodeTabSend5Receive4 code, int32_t value);
void setParameter(FunctionCodeTabSend5Receive8 code, int32_t value);

void setParameter(FunctionCodeTabSend6Receive4 code, int64_t value);
void setParameter(FunctionCodeTabSend6Receive7 code, int64_t value);

void setParameter(FunctionCodeTabSend7Receive5 code, int64_t value);
void setParameter(FunctionCodeTabSend7Receive6 code, int64_t value);


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

uint8_t _uctemp8;
uint16_t _ustemp16;
uint32_t _uitemp32;
uint64_t _ultemp64;
int8_t _sctemp8;
int16_t _sstemp16;
int32_t _sitemp32;
int64_t _sltemp64;
};
#endif // TI5MCL_H_INCLUDED
