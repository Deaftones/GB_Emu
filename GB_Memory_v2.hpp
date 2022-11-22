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
		ptr_to_total_memory = new uint8_t[65536];
	};

	void Set_Memory(uint16_t reg_address_0to65536, uint8_t value)
	{
		uint8_t* ptrptr = &ptr_to_total_memory[reg_address_0to65536];
		*ptrptr = value;
	};

	uint8_t* Get_Memory(uint16_t reg_address_0to65536)
	{
		uint8_t* mem = &ptr_to_total_memory[reg_address_0to65536];
		return mem;
	};

	void Set_Memory_16(uint16_t reg_address, uint16_t value16)
	{
		ptr_to_total_memory[reg_address] = ((uint16_t)value16 >> 8 & 0xFF);
		ptr_to_total_memory[reg_address + 1] = ((uint16_t)value16 >> 0 & 0xFF);
	};

	uint8_t* Get_Mem_Total_Ptr()
	{
		return ptr_to_total_memory;
	};

	~GB_Memory()
	{
		delete[] ptr_to_total_memory;
		ptr_to_total_memory = nullptr;
	};
};