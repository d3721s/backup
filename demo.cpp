#include "ti5mcl.h"

int main(int argc, char *argv[])
{
int tempPosition;
    ti5Motor ti5Motor1(1, ti5Motor::reductionRatio::reductionRatio101);
    usleep(400000);
    ti5Motor1.setCleanError();
    usleep(400000);
    ti5Motor ti5Motor2(2, ti5Motor::reductionRatio::reductionRatio101);
    usleep(400000);
    ti5Motor2.setCleanError();
    usleep(400000);

    ti5Motor ti5Motor3(3, ti5Motor::reductionRatio::reductionRatio81);
    usleep(400000);
    ti5Motor3.setCleanError();
    usleep(400000);
    ti5Motor ti5Motor4(4, ti5Motor::reductionRatio::reductionRatio81);
    usleep(400000);
    ti5Motor4.setCleanError();
    usleep(400000);
    ti5Motor ti5Motor5(5, ti5Motor::reductionRatio::reductionRatio51);
    usleep(400000);
    ti5Motor5.setCleanError();
    usleep(400000);
    sleep(5);

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

        ti5Motor1.quickMoveAbsoluteInDegree(30);
        usleep(400000);
        ti5Motor2.quickMoveAbsoluteInDegree(30);
        usleep(400000);
        ti5Motor3.quickMoveAbsoluteInDegree(30);
        usleep(400000);
        ti5Motor4.quickMoveAbsoluteInDegree(30);
        usleep(400000);
        ti5Motor5.quickMoveAbsoluteInDegree(30);
        usleep(400000);
        ti5Motor1.quickMoveRelativeInDegree(45);
        usleep(400000);
        sleep(5);

    }

    return 0;
}










