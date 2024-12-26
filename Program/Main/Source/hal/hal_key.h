#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

#define  HAL_KEY_NONE   0x00000000UL
#define  HAL_KEY_1      0x00000001UL
#define  HAL_KEY_2      0x00000002UL
#define  HAL_KEY_3      0x00000004UL
#define  HAL_KEY_4      0x00000008UL
#define  HAL_KEY_19     0x00000010UL

#define  HAL_KEY_ALL    0x0000001FUL

void HAL_InitKey(void);

void HAL_SetKeyVal(U32 mu32Val);
U32 HAL_GetKeyVal(void);

void HAL_SetLeverWater(U8 mu8Val);
U8 HAL_GetLeverWater(void);

void HAL_ScanKey(void);

#endif /* __HAL_KEY_H__ */
