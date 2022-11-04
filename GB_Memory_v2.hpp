#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <ios>
#include <bit>

class GB_Memory
{
private:
	uint8_t* ptr_to_total_memory;


public:
	GB_Memory()
	{
		ptr_to_total_memory = new uint8_t[256];
	};

	uint8_t* Get_Ptr_to_Memory(uint16_t hex_or_bin_address)
	{
		uint8_t* ptr = ptr_to_total_memory + (hex_or_bin_address / 256);
		return ptr;
	};

	~GB_Memory()
	{
		delete[] ptr_to_total_memory;
		ptr_to_total_memory = nullptr;
	};
};