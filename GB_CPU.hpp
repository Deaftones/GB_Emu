#pragma once
#include "GB_Memory_v2.hpp"
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
	uint8_t* m_A; //A - accumulator; where arhythmetic occurs
	uint8_t* m_F; // Flag register
	uint8_t* m_B;
	uint8_t* m_C;
	uint8_t* m_D; // !!! Some sources say these are 16-bit each
	uint8_t* m_E; // !!! Some sources say these are 16-bit each
	uint8_t* m_H; //	  can be used as two separate 8-bit regs or one 16-bit reg. Used to point to memory.
	uint8_t* m_L; //     can be used as two separate 8-bit regs or one 16-bit reg. Used to point to memory.
	uint8_t* m_S; // Stack Pointer;
	uint8_t* m_P; // Program counter === points to memory containing next instruction to be executed.
	// Boot instruction located at 0x0100 in Memory.
	uint8_t* m_CPU_total_memory;

	// === PRIVATE FUNCTIONS ===
	uint8_t** m_char_to_reg_ptr_single(char input)
	{
		uint8_t** ptr_to_reg_ptr;
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
		case 'F': ptr_to_reg_ptr = &m_F;
			break;
		case 'S': ptr_to_reg_ptr = &m_S;
			break;
		case 'P': ptr_to_reg_ptr = &m_P;
			break;
		default: std::cerr << "ERROR: m_char_to_reg_ptr_single input not recognized." << std::endl; exit(6);
		};
		return ptr_to_reg_ptr;
	};
	
	std::vector<uint8_t**> m_char_to_reg_ptr_paired(char input)
	{
		uint8_t** ptr_to_reg_ptr;
		uint8_t** ptr_to_reg_ptr_second;
		

		switch (input)
		{
		case 'A': ptr_to_reg_ptr = &m_A; ptr_to_reg_ptr_second = &m_F;  // A pairs with F
			break;
		case 'B': ptr_to_reg_ptr = &m_B; ptr_to_reg_ptr_second = &m_C;  // B pairs with C
			break;
		case 'D': ptr_to_reg_ptr = &m_D; ptr_to_reg_ptr_second = &m_E;  // D pairs with E
			break;
		case 'H': ptr_to_reg_ptr = &m_H; ptr_to_reg_ptr_second = &m_L;  // H pairs with L
			break;

		default: std::cerr << "ERROR: m_char_to_reg_ptr_paired input not recognized." << std::endl; exit(6);
		};

		std::vector<uint8_t**> vec_intptrptr;
		vec_intptrptr.push_back(ptr_to_reg_ptr);
		vec_intptrptr.push_back(ptr_to_reg_ptr_second);
		
		return vec_intptrptr;
	};

public:
	CPU()
	{
		m_CPU_total_memory = new uint8_t[12];
		m_A = &m_CPU_total_memory[0];
		m_B = &m_CPU_total_memory[1];
		m_C = &m_CPU_total_memory[2];
		m_D = &m_CPU_total_memory[3];
		m_E = &m_CPU_total_memory[4];
		m_F = &m_CPU_total_memory[5];
		m_H = &m_CPU_total_memory[6];
		m_L = &m_CPU_total_memory[7];
		m_S = &m_CPU_total_memory[8];
		m_P = &m_CPU_total_memory[10];
	};

	
	//======== C P U === COMMANDS ============================================================
	/*void COPY_REGISTRY_TO_REGISTRY(char copy_from, char copy_to)
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
	};*/
	//  - - - L O A D S - - - - - 
	void LD_r8_r8(char reg_copy_to, char reg_copy_from)  // Copy reg_right to reg_left. Cycles: 1   Bytes: 1    Flags: None
	{
		**m_char_to_reg_ptr_single(reg_copy_to) = **m_char_to_reg_ptr_single(reg_copy_from);
	};

	void LD_r8_n8(char reg_copy_to, uint8_t hex_or_bin_num_8bit)  //  Load value n8 into reg r8.   Cycles: 2    Bytes: 2    Flags: None
	{
		**m_char_to_reg_ptr_single(reg_copy_to) = hex_or_bin_num_8bit;
	};

	void LD_r16_n16(char reg_copy_to, uint16_t hex_or_bin_num_16bit) // Load n16 into r16.  Cycles: 3   Bytes: 3    Flags: None
	{
		std::vector<uint8_t**> vecptrs = m_char_to_reg_ptr_paired(reg_copy_to);
		**vecptrs[0] = ((uint16_t)hex_or_bin_num_16bit >> 0 & 0xFF);
		**vecptrs[1] = ((uint16_t)hex_or_bin_num_16bit >> 8 & 0xFF);
	};




	~CPU()
	{
		m_A, m_B, m_C, m_D, m_E, m_F0, m_F1, m_F2, m_F3, m_F4, m_F5, m_F6, m_F7, m_H, m_L, m_SP, m_PC = nullptr;
		delete[] m_CPU_total_memory;
		m_CPU_total_memory = nullptr;
	};
};