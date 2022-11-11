#pragma once
#include "GB_Memory_v2.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <bit>
#include <ios>
#include <cmath>
#include <cstdint>
#include <bitset>


class GB_Bus
{
private:
	uint8_t m_immediate_value_8;
	uint16_t m_immediate_value_16;

public:
	GB_Bus() {};
	uint8_t GetIMM_8()
	{
		return m_immediate_value_8;
	};
	uint16_t GetIMM_16()
	{
		return m_immediate_value_16;
	};
	~GB_Bus() {};
};

class GB_CPU
{
private:
	uint8_t m_A, m_B, m_C, m_D, m_E, m_H, m_L;  // 8-bit CPU registers

	// 16-bit paired registers, get and set
	uint16_t m_GET_BC() { return (m_C >> 8 | m_B); };
	uint16_t m_GET_DE() { return (m_E >> 8 | m_D); };
	uint16_t m_GET_HL() { return (m_L >> 8 | m_H); };
	void m_SET_BC(uint16_t val)
	{
		m_B = ((uint16_t)val >> 8 & 0xFF);
		m_C = ((uint16_t)val >> 0 & 0xFF);
	};

	void m_SET_DE(uint16_t val)
	{
		m_D = ((uint16_t)val >> 8 & 0xFF);
		m_E = ((uint16_t)val >> 0 & 0xFF);
	};

	void m_SET_HL(uint16_t val)
	{
		m_H = ((uint16_t)val >> 8 & 0xFF);
		m_L = ((uint16_t)val >> 0 & 0xFF);
	};

	uint8_t* m_RAM_pointer;

	GB_Memory* m_ptr_total_mem;

	bool m_F_Z;  // Zero flag
	bool m_F_N;  // Negative flag
	bool m_F_H;  // Half-carry flag
	bool m_F_Cy; // Carry flag

	//Interrupt Flag registers (IF)
	bool m_IF7, m_IF6, m_IF5, m_IF4, m_IF3, m_IF2, m_IF1, m_IF0;

	//interrupt Enable registers (IE)
	bool m_IE7, m_IE6, m_IE5, m_IE4, m_IE3, m_IE2, m_IE1, m_IE0;

	uint16_t m_clock_cycles;

	GB_Bus* m_bus_ptr;

	void m_SET_FLAG(char flagname, char val)
	{
		if (flagname == 'Z')
		{
			switch (val)
			{
			case '0': m_F_Z = 0;
				break;
			case '1': m_F_Z = 1;
				break;
			case '-':
				break;
			default: std::cerr << "ERROR: m_SET_FLAG invalid value" << std::endl; exit(14);
			};
		}
		else if (flagname == 'N')
		{
			switch (val)
			{
			case '0': m_F_N = 0;
				break;
			case '1': m_F_N = 1;
				break;
			case '-':
				break;
			default: std::cerr << "ERROR: m_SET_FLAG invalid value" << std::endl; exit(14);
			};
		}
		else if (flagname == 'H')
		{
			switch (val)
			{
			case '0': m_F_H = 0;
				break;
			case '1': m_F_H = 1;
				break;
			case '-':
				break;
			default: std::cerr << "ERROR: m_SET_FLAG invalid value" << std::endl; exit(14);
			};
		}
		else if (flagname == 'C')
		{
			switch (val)
			{
			case '0': m_F_Cy = 0;
				break;
			case '1': m_F_Cy = 1;
				break;
			case '-':
				break;
			default: std::cerr << "ERROR: m_SET_FLAG invalid value" << std::endl; exit(14);
			};
		}
		else { std::cerr << "ERROR m_SET_FLAG invalid flag input" << std::endl; exit(14); };
	};

	uint16_t m_GET_PAIR_VALUE(char first_letter)
	{
		uint16_t memory_address;
		switch (first_letter)
		{
		case 'B': memory_address = m_GET_BC();
			break;
		case 'D': memory_address = m_GET_DE();
			break;
		case 'H': memory_address = m_GET_HL();
			break;
		default: std::cerr << "ERROR: unable to set LD_ptr_reg memory_address" << std::endl; exit(22);
		};
		return memory_address;
	};

	/*uint16_t* m_GET_PAIR_POINTER(char first_letter)
	{
		uint16_t* ptr;
		switch (first_letter)
		{
		case 'B': ptr = m_;
			break;
		case 'D': memory_address = m_GET_DE();
			break;
		case 'H': memory_address = m_GET_HL();
			break;
		default: std::cerr << "ERROR: unable to set LD_ptr_reg memory_address" << std::endl; exit(22);
		};
		return ptr;
	}*/

public:
	GB_CPU(GB_Memory& mem)
	{
		m_RAM_pointer = new uint8_t[512];
		m_A, m_B, m_C, m_D, m_E, m_H, m_L = 0;
		m_IF7, m_IF6, m_IF5, m_IF4, m_IF3, m_IF2, m_IF1, m_IF0 = false;
		m_IE7, m_IE6, m_IE5, m_IE4, m_IE3, m_IE2, m_IE1, m_IE0 = false;
		m_F_Z, m_F_N, m_F_H, m_F_Cy = false;
		m_bus_ptr = new GB_Bus[1];
		m_ptr_total_mem = &mem;
	};

	uint8_t IMM_8()
	{
		return m_bus_ptr->GetIMM_8();
	};

	uint16_t IMM_16()
	{
		return m_bus_ptr->GetIMM_16();
	};

	void NOP(uint16_t cycles)
	{
		m_clock_cycles = cycles;
		while (m_clock_cycles > 0)
		{
			m_clock_cycles -= m_clock_cycles;
		};
	};

	void LD_BC(uint16_t right, uint16_t cycles, char ZFlag, char NFlag, char HFlag, char CyFlag)
	{
		m_SET_BC(right);
		m_clock_cycles = cycles;
		m_SET_FLAG('Z', ZFlag);
		m_SET_FLAG('N', NFlag);
		m_SET_FLAG('H', HFlag);
		m_SET_FLAG('C', CyFlag);
	};

	void LD_ptr_reg(char ptr_first_letter, uint8_t& registry, uint16_t cycles, char ZFlag, char NFlag, char HFlag, char CyFlag)
	{
		m_ptr_total_mem->Set_Memory(m_GET_PAIR_VALUE(ptr_first_letter), registry);
	};

	void INC_8(uint8_t& reg, uint16_t cycles, char ZFlag, char NFlag, char HFlag, char CyFlag)
	{
		reg++;
	};

	void INC_16(uint8_t& LSB, uint16_t cycles, char ZFlag, char NFlag, char HFlag, char CyFlag)
	{
		LSB++;
	};

	
	void OPCODE(uint16_t code)
	{
		char set = '1';
		char reset = '0';
		char Z = 'Z';
		char N = 'N';
		char H = 'H';
		char C = 'C';
		char X = '-';
		char B = 'B';
		char A = 'A';
		char D = 'D';
	
		switch (code)
		{
		case 0x00: NOP(4);
			break;
		case 0x01: LD_BC(IMM_16(), 12, X,X,X,X);
			break;
		case 0x02: LD_ptr_reg(B, m_A, 8, X, X, X, X);
			break;
		case 0x03: INC_16(m_C, 8, X, X, X, X);
			break;
		case 0x04:
			break;
		case 0x05:
			break;
		case 0x06:
			break;
		case 0x07:
			break;
		case 0x08:
			break;
		case 0x09:
			break;
		case 0x0A:
			break;
		case 0x0B:
			break;
		case 0x0C:
			break;
		case 0x0D:
			break;
		case 0x0E:
			break;
		case 0x0F:
			break;
		case 0x10:
			break;
		case 0x11:
			break;
		case 0x12:
			break;
		case 0x13:
			break;
		case 0x14:
			break;
		case 0x15:
			break;
		case 0x16:
			break;
		case 0x17:
			break;
		case 0x18:
			break;
		case 0x19:
			break;
		case 0x1A:
			break;
		case 0x1B:
			break;
		case 0x1C:
			break;
		case 0x1D:
			break;
		case 0x1E:
			break;
		case 0x1F:
			break;
		};
	};













	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	~GB_CPU()
	{
		delete[] m_RAM_pointer;
		m_RAM_pointer = nullptr;
	};
};