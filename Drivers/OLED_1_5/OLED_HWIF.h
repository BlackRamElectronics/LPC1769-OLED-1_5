#ifndef _OLED_HARDWARE_INTERFACE_H_
#define _OLED_HARDWARE_INTERFACE_H_

void OLED_InitIF(void);
void OLED_MsDelay(uint32_t ms_delay);
void OLED_SendByte(uint8_t data);
void OLED_ResetAssert(void);
void OLED_ResetDeassert(void);
void OLED_SetCommand(void);
void OLED_SetData(void);

#endif // _OLED_HARDWARE_INTERFACE_H_
