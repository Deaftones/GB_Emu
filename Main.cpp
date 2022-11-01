#include <iostream>
#include "GB_Memory.hpp"

int main()
{
	Memory_Load mainmemory;
	bool memarray[8] { 0, 0, 1, 1, 0, 1, 0, 0 };
	mainmemory.WRITE_byte_descending(0x00FF, memarray);

	for (uint16_t i = 0; i < 8; ++i)
	{
		std::cout << "Bit " << i << ": \t" <<
		mainmemory.READ_byte_at_address_ascending(0x00FF).bit[i] << std::endl;
	};

	return 0;
}