#include "ti5mcl.h"
#include "interface.h"
int main(int argc,char* argv[])
{
    ti5Motor ti5Motor(1);
    usleep(5000);
    interfaceGetParameter1(&ti5Motor,ti5Motor::FunctionCodeTabSend1Receive4::currentCode);
    return 0;
}










