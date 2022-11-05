#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <ios>
#include <bit>
#include <bitset>

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

	void Set_Memory_8bit(uint8_t hex_or_bin_address, uint16_t value)
	{
		uint8_t* ptr = ptr_to_total_memory + (hex_or_bin_address - 1);
		//uint8_t bitt = std::bitset<8>(value);
		*ptr = value;
		ptr = nullptr;
	};

	uint8_t Get_Memory_8bit(uint8_t hex_or_bin_address)
	{
		std::cout << hex_or_bin_address << std::endl;
		uint8_t mem = *(ptr_to_total_memory + (hex_or_bin_address - 1));
		return mem;
	}

	~GB_Memory()
	{
		delete[] ptr_to_total_memory;
		ptr_to_total_memory = nullptr;
	};
};