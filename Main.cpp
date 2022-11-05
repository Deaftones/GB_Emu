#include <iostream>
#include <bitset>
#include "GB_Memory_v2.hpp"
#include "GB_CPU.hpp"


int main()
{
	GB_Memory memory;
	CPU cpu;
	
	memory.Set_Memory_8bit(24, 00000111);

	uint8_t getmem = memory.Get_Memory_8bit(0xC000);
	

	std::cout << "Bit mem @ 0xC000 = " << std::bitset<8u>(getmem) << std::endl;

	return 0;
}