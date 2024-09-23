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
#include "canglue.h"

#define TI5MCLLOGLEVEL TLOG_DEBUG
#warning "temploglevel"
#ifndef TI5MCLLOGLEVEL
#define TI5MCLLOGLEVEL TLOG_WARN
#endif

#define CANDEVICE "vcan0"

class ti5MotorSetupData
{
public:
    ti5MotorSetupData(void) = default;
    ~ti5MotorSetupData(void) = default;
    ti5MotorSetupData(
        uint8_t canId, std::string name,
        uint8_t reductionRatio,
        int32_t maxPositiveCurrent, int32_t maxNegativeCurrent,
        int32_t maxPositiveAcceleration, int32_t maxNegativeAcceleration,
        int32_t maxPositiveVelocity, int32_t maxNegativeVelocity,
        int32_t maxPositivePositon, int32_t maxNegativePositon,
        int32_t positionOffset
    );

    canid_t getCanId(void) const;
    std::string getName(void) const;

    uint8_t getReductionRatio(void) const;

    int32_t getMaxPositiveCurrent(void) const;
    int32_t getMaxNegativeCurrent(void) const;

    int32_t getMaxPositiveAcceleration(void) const;
    int32_t getMaxNegativeAcceleration(void) const;

    int32_t getMaxPositivePositon(void) const;
    int32_t getMaxNegativePositon(void) const;

    int32_t getMaxPositiveVelocity(void) const;
    int32_t getMaxNegativeVelocity(void) const;

    int32_t getPositionOffset(void) const;

    void setCanId(canid_t canId); //仅空电机，其他情况不建议使用
    void setName(std::string name);
    void setReductionRatio(uint8_t reductionRatio); //仅空电机，其他情况不建议使用

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
    canid_t  _canId;
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
    enum class reductionRatio
    {
        reductionRatio51 = 51,
        reductionRatio81 = 81,
        reductionRatio101 = 101,
        reductionRatio121 = 121,
    };
    ti5Motor(void);//空电机,预留
    ti5Motor(uint8_t canId, reductionRatio reductionRatioValue);//使用硬件设置初始化软件对象
    ti5Motor(uint8_t canId, ti5MotorSetupData *deviceData);  //使用软件设置初始化软件对象//canId为当前canId，deviceData->_canId为将要设置的canId！
    ~ti5Motor(void) = default;
    //常用方法
    canid_t  getCanId(void);
    reductionRatio getReductionRatio(void);
    enum class MotorMode
    {
        modeUndefined = -1,
        modeStop = 0,
        modeCurrent = 1,
        modeVelocity = 2,
        modePosition = 3,
    };
    bool getMotorMode(void);
    bool setMotorMode(MotorMode mode);
    bool getCurrent();
    bool getTargetCurrent();
    bool getVelocity(int32_t* velocity)
    bool getTargetVelocity(int32_t* targetVelocity);
    bool getPosition(int32_t* position);
    bool getTargetPosition(int32_t* targetPosition);

    struct errorStatus
    {
        uint8_t softwareError: 1;
        uint8_t overVoltageError: 1;
        uint8_t lowVoltageError: 1;
        uint8_t : 1;
        uint8_t switchonError: 1;
        uint8_t speedFeedbackError: 1;
        uint8_t overCurrentError: 1;
        uint8_t operationError: 1;
        uint8_t : 8;
        uint8_t encoderTemperatureError: 1;
        uint8_t motorTemperatureError: 1;
        uint8_t driverTemperatureError: 1;
        uint8_t driverChipError: 1;
    };

    bool getErrorStatus();
    bool getMotorTemperature();
    bool getDriverTemperature();//建议使用autoMonitor()
    bool getCyclicSynchronousPosition();
    bool setTargetCurrent(int32_t targetCurrent);
    bool setTargetVelocity(int32_t targetVelocity);
    bool setTargetPosition(int32_t targetPosition);
    bool setCleanError();

    //快速设置
    bool quickReset();
    bool quickHome();
    bool quickHalt();
    bool quickMoveAbsolute(int32_t position);
    bool quickMoveAbsolute(int16_t positionInDegree);
    bool quickMoveRelative(int32_t position);
    bool quickMoveRelative(int16_t positionInDegree);
    bool quickMoveVelocity(int32_t velocity);
    bool quickMoveVelocity(int16_t perSecondDegree);
    bool quickMoveJog();
    //自定义
#warning "TODO:"
    //托管
    bool autoMonitor();
    bool autoCyclicSynchronousPosition();

    //全部方法
    enum class FunctionCodeTabSend1Receive0 //set
    {
        setStopModeCode = 2, // 停止电机
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
        interpolationPositionModeCode = 86,
    };


    enum class FunctionCodeTabSend7Receive5 // set
    {

    };
    enum class FunctionCodeTabSend7Receive6 // set
    {
        positionAndVelocityCode = 88,
    };
    enum class FunctionCodeTabSend8Receive8 // set
    {

    };



    bool readRegister(FunctionCodeTabSend1Receive4 code);
    bool writeRegister(FunctionCodeTabSend1Receive0 code);
    bool readRegister(FunctionCodeTabSend1Receive8 code);//csp

    bool writeRegister(FunctionCodeTabSend5Receive0 code, int32_t value);
    bool writeReadRegister(FunctionCodeTabSend5Receive4 code, int32_t value);
    bool writeReadRegister(FunctionCodeTabSend5Receive8 code, int32_t value);

    bool writeReadRegister(FunctionCodeTabSend6Receive4 code, int64_t value);
    bool writeReadRegister(FunctionCodeTabSend6Receive7 code, int64_t value);

    bool writeReadRegister(FunctionCodeTabSend7Receive5 code, int64_t value);
    bool writeReadRegister(FunctionCodeTabSend7Receive6 code, int64_t value);

    bool writeReadRegister(FunctionCodeTabSend8Receive8 code, int64_t value);

private:

    ti5MotorSetupData *_deviceData;
    canid_t  _canId;
    // uint32_t _baudRate;
    reductionRatio _reductionRatio;

    can_frame _frameSend;
    can_frame _frameReceive;

    uint8_t _uctemp;
    uint16_t _ustemp;
    uint32_t _uitemp;
    uint64_t _ultemp;
    int8_t _sctemp;
    int16_t _sstemp;
    int32_t _sitemp;
    int64_t _sltemp;
};
#endif // TI5MCL_H_INCLUDED
