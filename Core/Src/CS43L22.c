/*
 * CS43L22.c
 * 
 *  Created on: May 5, 2020
 *      Author: Steve
 *  El conocimiento es poder
 */

#include "CS43L22.h"
#include "main.h"
#include "fatfs.h"

extern I2C_HandleTypeDef hi2c1;
extern I2S_HandleTypeDef hi2s3;

int16_t aBuf[BUFFER_SIZE];
uint16_t bw = 0;

void audioInit(void){
	uint8_t pData;
	HAL_Delay(10);
	HAL_GPIO_WritePin(Audio_RST_GPIO_Port,Audio_RST_Pin,GPIO_PIN_SET);
	pData = 0x01;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x02, 1, &pData, 1, 100);
	pData = 0xAF;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x04, 1, &pData, 1, 100);
	//pData = 0x20;
	pData = 0xA0;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x05, 1, &pData, 1, 100);
	pData = 0x07;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x06, 1, &pData, 1, 100);
	pData = 0x99;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x00, 1, &pData, 1, 100);
	pData = 0x80;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x47, 1, &pData, 1, 100);
	HAL_I2C_Mem_Read(&hi2c1, 0x94, 0x32, 1, &pData, 1, 100);
	pData |= 0x80;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x32, 1, &pData, 1, 100);
	HAL_I2C_Mem_Read(&hi2c1, 0x94, 0x32, 1, &pData, 1, 100);
	pData &= ~0x80;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x32, 1, &pData, 1, 100);
	pData = 0;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x00, 1, &pData, 1, 100);
	pData = 0x9E;
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x02, 1, &pData, 1, 100);
}

/*Utilizar si es con interrupcion*/
//void audioPlay(const char * aPath){
//	uint32_t br;
//	if(f_open(&USBHFile, aPath, FA_READ)) return;
//	audioVolume(0x0, -5);
//	f_lseek(&USBHFile, 157);
//	HAL_I2S_Transmit_IT(&hi2s3, aBuf, BUFFER_SIZE);
//	while(1){
//		f_read(&USBHFile, (uint8_t *)aBuf, BUFFER_SIZE*2, &br);
//		//swapBuffer(); para little-endian
//		if(!br) break;
//		while(bw < 19002);
//		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
//	}
//	f_close(&USBHFile);
//	audioVolume(0,0);
//}
/*esto en la interrupcion*/
//if(++bw == BUFFER_SIZE){
//	  HAL_I2S_Transmit_IT(&hi2s3, aBuf, BUFFER_SIZE);
//	  bw = 0;
//}

void audioPlay(const char * aPath){
	uint32_t br;
	if(f_open(&USBHFile, aPath, FA_READ)) return;
	audioVolume(0x0, -40);
	f_lseek(&USBHFile, 157);
	f_read(&USBHFile, (uint8_t *)aBuf, BUFFER_SIZE*2, &br);
	HAL_I2S_Transmit_DMA(&hi2s3, aBuf, BUFFER_SIZE);
	while(1){
		while(bw < 2);
		bw = 0;
		f_read(&USBHFile, (uint8_t *)aBuf, BUFFER_SIZE*2, &br);
		//swapBuffer(); para little-endian
		if(!br) break;
		HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_13);
	}
	f_close(&USBHFile);
	audioVolume(0,0);
}

void audioVolume(int8_t aVolI, int8_t aVolO){
	int8_t pData[2] = {aVolI, aVolO};
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x1A, 1, pData, 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x1B, 1, pData, 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x20, 1, &pData[1], 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, 0x94, 0x21, 1, &pData[1], 1, 100);
}

void swapBuffer(void){
	for(uint16_t aCnt = 0;aCnt < 256;aCnt++) aBuf[aCnt] = SWAP16(aBuf[aCnt]);
}
