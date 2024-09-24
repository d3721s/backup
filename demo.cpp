#include "ti5mcl.h"

int main(int argc, char *argv[])
{
    ti5Motor ti5Motor(1, ti5Motor::reductionRatio::reductionRatio51);
    usleep(5000);
    ti5Motor.readRegister(ti5Motor::FunctionCodeTabSend1Receive8::getCurrentSpeedPositionCode);
    usleep(5000);

    return 0;
}










