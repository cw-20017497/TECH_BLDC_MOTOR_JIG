#ifndef __KEY_NORMAL_HANDLER_H__
#define __KEY_NORMAL_HANDLER_H__

#include "prj_type.h"


U8 IsValidNormalKeyCondition(U32 mu32Key);
void* GetNormalKeyEventList(void);

U8 GetNormalKeyEventListSize(void);

void InitKeyHandler(void);
void ContyUpDownVolt(void);
#endif /* __KEY_NORMAL_HANDLER_H__ */
