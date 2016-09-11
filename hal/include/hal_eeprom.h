/*
 * hal_eeprom.h
 *
 *  Created on: 10.03.2016
 *      Author: lars
 */

#ifndef HAL_INCLUDE_HAL_EEPROM_H_
#define HAL_INCLUDE_HAL_EEPROM_H_

void hal_eeprom_init(void);
void hal_eeprom_getString(int which, char* buf, int length);
void hal_eeprom_setString(int which, char* buf, int length);
int hal_eeprom_getIntValue(int which, int failSafeValue);
void hal_eeprom_setIntValue(int which, int value);
float hal_eeprom_getfloat(int which, float failSafeValue);
void hal_eeprom_getfloat(int which, float value);


#endif /* HAL_INCLUDE_HAL_EEPROM_H_ */
