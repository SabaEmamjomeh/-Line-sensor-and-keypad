#include <main.h>
#include <utils.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> //for atoi function
#include <ctype.h> //is digit
#include "SX1509_Registers.h"

extern I2C_HandleTypeDef hi2c1;
extern int LED_PRESC;
int16_t data_key;

uint32_t previousMillis = 0;
uint32_t currentMillis = 0;

int row, col;

char keyBuffer[16] = {0};
int bufferIndex = 0;

char key = {0};
char previous_key = {0};

const char keypadLayout[4][4] = {
		{'*', '0', '#', 'D'},
		{'7', '8', '9', 'C'},
		{'4', '5', '6', 'B'},
		{'1', '2', '3', 'A'}};



// get the row and column from 2byte data (first byte is the row, second byte the column)
void getIndex(int data_key, int *row, int *col) {
	*row = -1;
	*col = -1;

	for (int i = 0; i < 8; ++i) {
		if (((data_key >> i) & 1) == 0) {
			*col = i;
			break;
		}
	}
	for (int i = 8; i < 16; ++i) {
		if (((data_key >> i) & 1) == 0) {
			*row = i - 8;
			break;
		}
	}
}

void clear_key_buffer(){
	for (int i = 0; i < 16; i++) {
		keyBuffer[i] = 0;
	}
	bufferIndex = 0;
}

// Check for the valid string to be assigned as blink frequency (Needs to be a number)
bool check_valid(const char *buffer) {
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (!isdigit((unsigned char)buffer[i])) //isdigit expects a non-negative value
			return false;
	}
	return true;
}

void keyPad_read() {

	HAL_I2C_Mem_Read(&hi2c1, SX1509_I2C_ADDR2 << 1, REG_KEY_DATA_1, 1, (uint8_t*)&data_key, 2, I2C_TIMEOUT);
	getIndex(data_key, &row, &col);
	key = keypadLayout[row][col];

	currentMillis = HAL_GetTick();

	// Print the buffer
	if(key == '#'){
		printf("Buffer: %s \n", keyBuffer);

		if (check_valid(keyBuffer)){
			printf("Value: %d \n", atoi(keyBuffer)); //
			LED_PRESC = atoi(keyBuffer);
		}
		else{
			printf("NO SE PUEDEEEE, MA PERCHEEEE \n");
		}

		clear_key_buffer();
	}
	else if (key == 'D'){
		clear_key_buffer();
		printf("Buffered cleared \n");
	}

	// Some type of debouncer. If you hold a button we write the value into the buffer with a freq of 200ms
	else if (key==previous_key && (currentMillis-previousMillis>1000)){
		keyBuffer[bufferIndex++] = key;
		previousMillis = currentMillis;
		printf("%c\n",key);
	}

	// if the values differ then just write it to the buffer
	else if(key!=previous_key){
		keyBuffer[bufferIndex++]=key;
		//Debug prints
		printf("%c\n",key);
	}
	previous_key = key;
}


