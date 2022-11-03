#pragma once
#include "GB_Memory.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <bit>
#include <ios>
#include <cmath>
#include <cstdint>


class CPU
{
private:
	bool* m_A; //A - accumulator; where arhythmetic occurs
	bool* m_F0; bool* m_F1; bool* m_F2; bool* m_F3; bool* m_F4; bool* m_F5; bool* m_F6; bool* m_F7; // Flag register
	bool* m_B;
	bool* m_C;
	bool* m_D; // !!! Some sources say these are 16-bit each
	bool* m_E; // !!! Some sources say these are 16-bit each
	bool* m_H; //	  can be used as two separate 8-bit regs or one 16-bit reg. Used to point to memory.
	bool* m_L; //     can be used as two separate 8-bit regs or one 16-bit reg. Used to point to memory.
	bool* m_SP; // Stack Pointer;
	bool* m_PC; // Program counter === points to memory containing next instruction to be executed.
	// Boot instruction located at 0x0100 in Memory.
	bool* m_CPU_total_memory;

	// === PRIVATE FUNCTIONS ===
	bool** m_char_to_reg_ptr_single(char input)
	{
		bool** ptr_to_reg_ptr;
		switch (input)
		{
		case 'A': ptr_to_reg_ptr = &m_A;
			break;
		case 'B': ptr_to_reg_ptr = &m_B;
			break;
		case 'C': ptr_to_reg_ptr = &m_C;
			break;
		case 'D': ptr_to_reg_ptr = &m_D;
			break;
		case 'E': ptr_to_reg_ptr = &m_E;
			break;
		case 'H': ptr_to_reg_ptr = &m_H;
			break;
		case 'L': ptr_to_reg_ptr = &m_L;
			break;
		case '0': ptr_to_reg_ptr = &m_F0;
			break;
		case '1': ptr_to_reg_ptr = &m_F1;
			break;
		case '2': ptr_to_reg_ptr = &m_F2;
			break;
		case '3': ptr_to_reg_ptr = &m_F3;
			break;
		case '4': ptr_to_reg_ptr = &m_F4;
			break;
		case '5': ptr_to_reg_ptr = &m_F5;
			break;
		case '6': ptr_to_reg_ptr = &m_F6;
			break;
		case '7': ptr_to_reg_ptr = &m_F7;
			break;
		case 'S': ptr_to_reg_ptr = &m_SP;
			break;
		case 'P': ptr_to_reg_ptr = &m_PC;
			break;
		default: std::cerr << "ERROR: m_char_to_reg_ptr_single input not recognized." << std::endl; exit(6);
		};
		return ptr_to_reg_ptr;
	};
	
	std::vector<bool**> m_char_to_reg_ptr_paired(char input)
	{
		bool** ptr_to_reg_ptr;
		bool** ptr_to_reg_ptr_second;
		std::vector<bool**> array_of_bool_ptrs;

		switch (input)
		{
		case 'A': ptr_to_reg_ptr = &m_A; ptr_to_reg_ptr_second = &m_F0;  // A pairs with F
			break;
		case 'B': ptr_to_reg_ptr = &m_B; ptr_to_reg_ptr_second = &m_C;  // B pairs with C
			break;
		case 'D': ptr_to_reg_ptr = &m_D; ptr_to_reg_ptr_second = &m_E;  // D pairs with E
			break;
		case 'H': ptr_to_reg_ptr = &m_H; ptr_to_reg_ptr_second = &m_L;  // H pairs with L
			break;

		default: std::cerr << "ERROR: m_char_to_reg_ptr_paired input not recognized." << std::endl; exit(6);
		};

		for (uint16_t i = 0; i < 8; ++i)
		{
			array_of_bool_ptrs.push_back(ptr_to_reg_ptr + i);
		};
		for (uint16_t j = 0; j < 8; ++j)
		{
			array_of_bool_ptrs.push_back(ptr_to_reg_ptr_second + j);
		};
		return array_of_bool_ptrs;
	};

public:
	CPU()
	{
		m_CPU_total_memory = new bool[96];
		m_A = &m_CPU_total_memory[0];
		m_B = &m_CPU_total_memory[8];
		m_C = &m_CPU_total_memory[16];
		m_D = &m_CPU_total_memory[24];
		m_E = &m_CPU_total_memory[32];
		m_F0 = &m_CPU_total_memory[40]; m_F1 = &m_CPU_total_memory[41]; m_F2 = &m_CPU_total_memory[42]; m_F3 = &m_CPU_total_memory[43];
		m_F4 = &m_CPU_total_memory[44]; m_F5 = &m_CPU_total_memory[45]; m_F6 = &m_CPU_total_memory[46]; m_F7 = &m_CPU_total_memory[47];
		m_H = &m_CPU_total_memory[48];
		m_L = &m_CPU_total_memory[56];
		m_SP = &m_CPU_total_memory[64];
		m_PC = &m_CPU_total_memory[80];
	};

	
	//======== C P U === COMMANDS ============================================================
	void COPY_REGISTRY_TO_REGISTRY(char copy_from, char copy_to)
	{
		bool** ptr_to_registry_ptr_FROM = m_char_to_reg_ptr_single(copy_from);
		bool** ptr_to_registry_ptr_TO = m_char_to_reg_ptr_single(copy_to);
		
		for (uint16_t i = 0; i < 8; ++i)
		{
			*(*ptr_to_registry_ptr_TO + i) = *(*ptr_to_registry_ptr_FROM + i);
		};
		ptr_to_registry_ptr_FROM = nullptr;
		ptr_to_registry_ptr_TO = nullptr;
	};

	void LOAD_MEMORY_TO_REGISTRY_8BIT(Memory_Load& ref_to_main_memory, uint16_t hex_memory_address, char load_to)
	{
		Mem_Byte mem_to_be_loaded = ref_to_main_memory.READ_byte_at_address_ascending(hex_memory_address);
		bool** ptr_to_registry_ptr_to = m_char_to_reg_ptr_single(load_to);
		for (uint16_t i = 0; i < 8; ++i)
		{
			*(*ptr_to_registry_ptr_to + i) = mem_to_be_loaded.bit[i];
		};
		ptr_to_registry_ptr_to = nullptr;
	};

	void LOAD_MEMORY_TO_REGISTRY_16BIT(Memory_Load& ref_to_main_memory, uint16_t hex_memory_address_1, uint16_t hex_memory_address_2, char load_to)
	{
		Mem_Byte mem_to_be_loaded_1 = ref_to_main_memory.READ_byte_at_address_ascending(hex_memory_address_1);
		Mem_Byte mem_to_be_loaded_2 = ref_to_main_memory.READ_byte_at_address_ascending(hex_memory_address_2);
		std::vector<bool**> ptr_to_registry_ptr_to = m_char_to_reg_ptr_paired(load_to);
		for (uint16_t i = 0; i < 8; ++i)
		{
			**ptr_to_registry_ptr_to[i] = mem_to_be_loaded_1.bit[i];
		};
		for (uint16_t j = 0; j < 8; ++j)
		{
			**ptr_to_registry_ptr_to[j + 8] = mem_to_be_loaded_2.bit[j];
		};
	};

	void WRITE_TO_FLAG(char flag_name, bool value)
	{
		bool** ptr_to_ptr_to_flag = m_char_to_reg_ptr_single(flag_name);
		**ptr_to_ptr_to_flag = value;
		ptr_to_ptr_to_flag = nullptr;
	};

	bool READ_FLAG(char flag_name)
	{
		bool flag_value = **(m_char_to_reg_ptr_single(flag_name));
		return flag_value;
	};

	void LDrs(char reg_8bit, uint8_t any_hex_or_8bit_num)
	{
		bool** ptr_to_ptr_to_registry = m_char_to_reg_ptr_single(reg_8bit);
		**ptr_to_ptr_to_registry = any_hex_or_8bit_num;
		ptr_to_ptr_to_registry = nullptr;
	};


	~CPU()
	{
		m_A, m_B, m_C, m_D, m_E, m_F0, m_F1, m_F2, m_F3, m_F4, m_F5, m_F6, m_F7, m_H, m_L, m_SP, m_PC = nullptr;
		delete[] m_CPU_total_memory;
		m_CPU_total_memory = nullptr;
	};
};