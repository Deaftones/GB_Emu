#include <iostream>
#include <bitset>
#include "GB_Memory_v2.hpp"
#include "GB_CPU.hpp"


int main()
{
	uint16_t test = 0b0000'0110'1111'1110;
	uint8_t leftside, rightside;
	leftside = ((uint16_t)test >> 8 & 0xFF);
	rightside = ((uint16_t)test >> 0 & 0xFF);

	std::cout << "16bit test = " << std::bitset<16>(test) << "\nleftside = " << std::bitset<8>(leftside) << "\nrightside = " <<
		std::bitset<8>(rightside) << std::endl;




	return 0;
}