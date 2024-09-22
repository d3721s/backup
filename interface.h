#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED
#include <cstdint>
#include "ti5mcl.h"
#include "canglue.h"

void interfaceInit(void);
int32_t interfaceGetParameter1(ti5Motor* motorHandle, ti5Motor::FunctionCodeTabSend1Receive4 code);
int32_t interfaceGetParameter5(ti5Motor* motorHandle, ti5Motor::FunctionCodeTabSend5Receive4 code);
void interfaceSetParameter1(ti5Motor* motorHandle,ti5Motor::FunctionCodeTabSend1Receive4 code);
void interfaceSetParameter5(ti5Motor* motorHandle,ti5Motor::FunctionCodeTabSend5Receive4 code,uint32_t value);
#endif // INTERFACE_H_INCLUDED
