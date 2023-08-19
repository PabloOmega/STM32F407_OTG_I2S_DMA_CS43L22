/*
 * CS43L22.h
 * 
 *  Created on: May 5, 2020
 *      Author: Steve
 *  El conocimiento es poder
 */

#ifndef INC_CS43L22_H_
#define INC_CS43L22_H_

#include "stdint.h"

#define BUFFER_SIZE  16001
#define SWAP16(x) (((x & 0xFF00) >> 8) | (x << 8))

void audioInit(void);
void audioPlay(const char * aPath);
void audioVolume(int8_t aVolI, int8_t aVolO);
void swapBuffer(void);

#endif /* INC_CS43L22_H_ */
