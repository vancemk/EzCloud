#include "databuffer.h"
#include <stdio.h>
#include <stdint.h>
#include <iostream>

using namespace std;


int main(){
	DataBuffer dbuf;
	printf("get data len: %d\n", dbuf.getDataLen());
	
	// test write uint8_t
	uint8_t uiNum1 = 10;
	uint8_t uiNum2 = 11;
	dbuf.writeInt8(uiNum1);
	dbuf.writeInt8(uiNum2);
	printf("get data len: %d\n", dbuf.getDataLen());

	// test write uint16_t
	uint16_t ui16Num1 = 9;
	uint16_t ui16Num2 = 10;
	dbuf.writeInt16(ui16Num1);
	dbuf.writeInt16(ui16Num2);
	printf("get data len: %d\n", dbuf.getDataLen());

	// test write uinti32_t
	uint32_t ui32Num1 = 9;
	uint32_t ui32Num2 = 10;
	dbuf.writeInt32(ui32Num1);
	dbuf.writeInt32(ui32Num2);
	printf("get data len: %d\n", dbuf.getDataLen());

	// test write uinti32_t
	uint64_t ui64Num1 = 9;
	uint64_t ui64Num2 = 10;
	dbuf.writeInt64(ui64Num1);
	dbuf.writeInt64(ui64Num2);
	printf("get data len: %d\n", dbuf.getDataLen());

	// test read uint8_t
	uint8_t ui8Read1 = dbuf.readInt8();
	uint8_t ui8Read2 = dbuf.readInt8();
	printf("read success: %d\n", ui8Read1);
	printf("read success: %d\n", ui8Read2);


	return 0;

}








