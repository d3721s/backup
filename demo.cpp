#include "ti5mcl.h"

int main(int argc, char *argv[])
{
int tempPosition;
    ti5Motor ti5Motor1(1, ti5Motor::reductionRatio::reductionRatio101);
    ti5Motor1.setCleanError();
    ti5Motor ti5Motor2(2, ti5Motor::reductionRatio::reductionRatio101);
    ti5Motor2.setCleanError();

    ti5Motor ti5Motor3(3, ti5Motor::reductionRatio::reductionRatio81);
    ti5Motor3.setCleanError();
    ti5Motor ti5Motor4(4, ti5Motor::reductionRatio::reductionRatio81);
    ti5Motor4.setCleanError();
    ti5Motor ti5Motor5(5, ti5Motor::reductionRatio::reductionRatio51);
    ti5Motor5.setCleanError();
    sleep(5);
//    while(1)
//    {
//     ti5Motor1.setTargetPosition(1654784);
//     sleep(5);
//     ti5Motor1.getPosition(&tempPosition);
//        printf("%d should = 1654784\n",tempPosition);
//        sleep(2);
//    ti5Motor1.readRegister(ti5Motor::FunctionCodeTabSend1Receive8::getCurrentSpeedPositionCode);
//    sleep(5);
//
//    ti5Motor::errorStatus es;
//    ti5Motor1.getErrorStatus(&es);
//    sleep(2);
//    }
    while(1)
    {
        ti5Motor1.quickHome();
        usleep(400000);
        ti5Motor2.quickHome();
        usleep(400000);
        ti5Motor3.quickHome();
        usleep(400000);
        ti5Motor4.quickHome();
        usleep(400000);
        ti5Motor5.quickHome();
        usleep(400000);

        sleep(5);

        ti5Motor1.quickMoveAbsoluteInDegree(45);
        usleep(400000);
        ti5Motor2.quickMoveRelativeInDegree(45);
        usleep(400000);
        ti5Motor3.quickMoveRelativeInDegree(45);
        usleep(400000);
        ti5Motor4.quickMoveRelativeInDegree(45);
        usleep(400000);
        ti5Motor5.quickMoveRelativeInDegree(45);
        usleep(400000);
        
        sleep(5);
    }

    return 0;
}










