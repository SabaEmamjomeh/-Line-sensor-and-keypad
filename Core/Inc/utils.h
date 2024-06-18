#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stdbool.h>

#define HAL_TIMEOUT 100
#define I2C_TIMEOUT 200

#define SX1509_I2C_ADDR1 0x3E //	SX1509 Proxy Sensors I2C address
#define SX1509_I2C_ADDR2 0x3F //	SX1509 Keypad I2C address

int getIndexfromByte(int byte);
void getIndex(int data_key, int *row, int *col);
void clear_key_buffer();
bool check_valid(const char *buffer);
void keyPad_read();

#endif /* INC_UTILS_H_ */
