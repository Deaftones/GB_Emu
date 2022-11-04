#include <iostream>
#include "GB_Memory_v2.hpp"


int main()
{
	GB_Memory memtest;
	uint8_t* ptr = memtest.Get_Ptr_to_Memory(0xFFFFF);
	std::cout << std::dec << *ptr;

	return 0;
}