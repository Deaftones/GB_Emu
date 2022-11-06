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
	uint8_t* m_X; // my personal flags
	// Boot instruction located at 0x0100 in Memory.
	uint8_t* m_CPU_total_memory;
	GB_Memory* m_ptr_to_total_memory;
	uint16_t m_HL() { return (*m_L << 8 | *m_H); };    
	uint16_t m_BC() { return (*m_C << 8 | *m_B); };
	uint16_t m_DE() { return (*m_E << 8 | *m_D); };

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

	uint16_t m_HL_pointer_value()
	{
		uint16_t HLptr;
		uint8_t H = *m_H;
		uint8_t L = *m_L;
		HLptr = (L << 8) | H;
		return HLptr;
	};

	uint8_t m_reg_ptr_number(char mem)
	{
		uint8_t binnum;
		switch (mem)
		{
		case 'A': binnum = 0b00000000;
			break;
		case 'B': binnum = 0b00000001;
			break;
		case 'C': binnum = 0b00000010;
			break;
		case 'D': binnum = 0b00000011;
			break;
		case 'E': binnum = 0b00000100;
			break;
		case 'F': binnum = 0b00000101;
			break;
		case 'S': binnum = 0b00001000;
			break;
		case 'P': binnum = 0b00001010;
			break;
		case 'X': binnum = 0b00001100;
			break;
		default: std::cerr << "ERROR: invalid registry address passed to m_reg_ptr_number" << std::endl; exit(9);
		};
		return binnum;
	};

public:
	CPU(GB_Memory& total_memory)
	{
		m_CPU_total_memory = new uint8_t[13];
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
		m_X = &m_CPU_total_memory[12];
		m_ptr_to_total_memory = &total_memory;
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

	void LD_r8_HLptr(char reg_copy_to)  // Load 8bit data pointed to by the 16-bit pointer register (HL)
	{
		uint16_t HL_pointer = m_HL_pointer_value();
		**m_char_to_reg_ptr_single(reg_copy_to) = m_ptr_to_total_memory->Get_Memory(HL_pointer);
	};

	void LD_HLderef_r8(char reg)
	{
		m_ptr_to_total_memory->Set_Memory(m_HL(), **m_char_to_reg_ptr_single(reg));
	};

	void LD_HLderef_n8(uint8_t n8)
	{
		m_ptr_to_total_memory->Set_Memory(m_HL(), n8);
	};

	void LD_HLptr_n16(uint16_t address_of_8bit_memory)
	{
		uint8_t h, l;
		h = ((uint16_t)address_of_8bit_memory >> 0 & 0xFF);
		l = ((uint16_t)address_of_8bit_memory >> 8 & 0xFF);
		*m_H = h;
		*m_L = l;
		std::cout << "h = " << std::bitset<8>(h) << "\nl = " << std::bitset<8>(l) << std::endl;
	};

	void LD_A_BCptr()
	{
		*m_A = m_ptr_to_total_memory->Get_Memory(m_BC());
	};

	void LD_A_DEptr()
	{
		*m_A = m_ptr_to_total_memory->Get_Memory(m_DE());
	};

	void LD_BCderef_A()
	{
		m_ptr_to_total_memory->Set_Memory(m_BC(), *m_A);
	};

	void LD_DEderef_A()
	{
		m_ptr_to_total_memory->Set_Memory(m_DE(), *m_A);
	};

	void LD_A_nnptr(uint16_t nn)
	{
		*m_A = m_ptr_to_total_memory->Get_Memory(nn);
	};

	void LD_nnderef_A(uint16_t nn)
	{
		m_ptr_to_total_memory->Set_Memory(nn, *m_A);
	};

	void LDH_A_Cptr()  // C = 8bit reg. Full 16-bit address = set most  significant byte to 0xFF and least sign to C.
	{
		uint16_t signif = (0xFF00 | *m_C);
		*m_A = m_ptr_to_total_memory->Get_Memory(signif);
	};

	void LDH_Cderef_A() // here, C is 8bit register, no 16-bit stuff like above.
	{
		m_CPU_total_memory[*m_C] = *m_A;
	};

	void LDH_A_nptr(uint8_t n)
	{
		*m_A = m_CPU_total_memory[n];
	};

	void LDH_nderef_A(uint8_t n) // n = 8bit uint. Full 16-bit address = set most signig byte to 0xFF and least sign to n.
	{
		uint16_t signif = (0xFF00 | n);
		m_ptr_to_total_memory->Set_Memory(signif, *m_A);
	};

	void LD_A_HLptr_decrement()    // load memory from location pointed to by HL into register A, then decrement HL by 1 AFTER read.
	{
		*m_A = m_ptr_to_total_memory->Get_Memory(m_HL());
		uint16_t HL = (*m_H << 8 | *m_L);
		HL = HL - 1;
		*m_H = ((uint16_t)HL >> 8 & 0xFF);
		*m_L = ((uint16_t)HL >> 0 & 0xFF);
	};

	void LD_HLrefer_A_decrement() // load value of reg A into memory pointed to by HL, then decrement HL after the write.
	{
		m_ptr_to_total_memory->Set_Memory(m_HL(), *m_A);
		uint16_t HL = (*m_H << 8 | *m_L);
		HL = HL - 1;
		*m_H = ((uint16_t)HL >> 8 & 0xFF);
		*m_L = ((uint16_t)HL >> 0 & 0xFF);
	};

	void LD_A_HLptr_increment() // load mem from HLptr to A, then increment HL by 1
	{
		*m_A = m_ptr_to_total_memory->Get_Memory(m_HL());
		uint16_t HL = (*m_H << 8 | *m_L);
		HL = HL + 1;
		*m_H = ((uint16_t)HL >> 8 & 0xFF);
		*m_L = ((uint16_t)HL >> 0 & 0xFF);
	};

	void LD_HLderef_A_increment() // load value A into memory poitned to be HL, then increment HL by 1.
	{
		m_ptr_to_total_memory->Set_Memory(m_HL(), *m_A);
		uint16_t HL = (*m_H << 8 | *m_L);
		HL = HL + 1;
		*m_H = ((uint16_t)HL >> 8 & 0xFF);
		*m_L = ((uint16_t)HL >> 0 & 0xFF);
	};


	// - - - - 1 6 - - b i t - - l o a d s - - - - 
	void LD_r16_n16(char reg_copy_to, uint16_t hex_or_bin_num_16bit) // Load n16 into r16.  Cycles: 3   Bytes: 3    Flags: None
	{
		std::vector<uint8_t**> vecptrs = m_char_to_reg_ptr_paired(reg_copy_to);
		**vecptrs[0] = ((uint16_t)hex_or_bin_num_16bit >> 0 & 0xFF);
		**vecptrs[1] = ((uint16_t)hex_or_bin_num_16bit >> 8 & 0xFF);
	};




	~CPU()
	{
		m_A, m_B, m_C, m_D, m_E, m_F, m_H, m_L, m_S, m_P, m_X = nullptr;
		delete[] m_CPU_total_memory;
		m_CPU_total_memory = nullptr;
		m_ptr_to_total_memory = nullptr;
	};
};