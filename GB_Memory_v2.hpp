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
		ptr_to_total_memory = new uint8_t[8192];
	};

	void Set_Memory(uint16_t reg_address_0to8191, uint8_t value)
	{
		uint8_t* ptrptr = &ptr_to_total_memory[reg_address_0to8191];
		*ptrptr = value;
	};

	uint8_t Get_Memory(uint16_t reg_address_0to8191)
	{
		uint8_t mem = ptr_to_total_memory[reg_address_0to8191];
		return mem;
	};

	~GB_Memory()
	{
		delete[] ptr_to_total_memory;
		ptr_to_total_memory = nullptr;
	};
};