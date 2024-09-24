#include "ti5mcl.h"

int main(int argc, char *argv[])
{
    ti5Motor ti5Motor1(1, ti5Motor::reductionRatio::reductionRatio51);
    usleep(5000);
    ti5Motor1.readRegister(ti5Motor::FunctionCodeTabSend1Receive8::getCurrentSpeedPositionCode);
    ti5Motor1.setTargetPosition(1654784);
    usleep(5000);

    return 0;
}










