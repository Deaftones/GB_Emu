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
	GB_Bus() 
	{
		m_immediate_value_16 = 0;
		m_immediate_value_8 = 0;
	};
	uint8_t GetIMM_8()
	{
		return m_immediate_value_8;
	};
	uint16_t GetIMM_16()
	{
		return m_immediate_value_16;
	};
	void SetIMM_8(uint8_t set)
	{
		m_immediate_value_8 = set;
	};
	void setIMM_16(uint16_t set)
	{
		m_immediate_value_16 = set;
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

	bool m_GET_FLAGS()
	{
		bool flags[4] = { m_F_Z, m_F_N, m_F_H, m_F_Cy };
		return flags;
	};

	void m_SET_FLAGS(short Z, short N, short H, short C)
	{
		switch (Z)
		{
		case 0: m_F_Z = 0;
			break;
		case 1: m_F_Z = 1;
			break;
		case 2: m_F_Z = m_F_Z;
			break;
		default: std::cerr << "ERROR: m_SET_FLAGS error" << std::endl; exit(20);
		};
		switch (N)
		{
		case 0: m_F_N = 0;
			break;
		case 1: m_F_N = 1;
			break;
		case 2: m_F_N = m_F_N;
			break;
		default: std::cerr << "ERROR: m_SET_FLAGS error" << std::endl; exit(20);
		};
		switch (H)
		{
		case 0: m_F_H = 0;
			break;
		case 1: m_F_H = 1;
			break;
		case 2: m_F_H = m_F_H;
			break;
		default: std::cerr << "ERROR: m_SET_FLAGS error" << std::endl; exit(20);
		};
		switch (C)
		{
		case 0: m_F_Cy = 0;
			break;
		case 1: m_F_Cy = 1;
			break;
		case 2: m_F_Cy = m_F_Cy;
			break;
		default: std::cerr << "ERROR: m_SET_FLAGS error" << std::endl; exit(20);
		};
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

	void LD_BC(uint16_t right, uint16_t cycles)
	{
		m_SET_BC(right);
		m_clock_cycles = cycles;
	};

	void LD_8(uint8_t& reg, uint8_t right_side, uint16_t cycles)
	{
		if ((reg + right_side) > 0xFF) { m_SET_FLAG('C', 1); };
		reg = reg + right_side;
	};

	void LD_16(std::string reg_16, uint16_t right_side, uint16_t cycles) 
	{
		
	};

	void LD_16(uint16_t address, uint16_t right_side, uint16_t cycles) 
	{
		m_ptr_total_mem->Set_Memory_16(address, right_side);
	};

	void LD_ptr_reg(char ptr_first_letter, uint8_t& registry, uint16_t cycles)
	{
		m_ptr_total_mem->Set_Memory(m_GET_PAIR_VALUE(ptr_first_letter), registry);
	};

	void INC_8(uint8_t& reg, uint16_t cycles)
	{
		if (reg == 0xFF) { m_SET_FLAG('C', 1); };
		reg++;
	};

	void INC_16(std::string reg_16, uint16_t cycles)
	{
		if (reg_16 == "BC") 
		{ 
			if (m_GET_BC() == 0xFFFF) { m_SET_FLAG('C', 1); };
			m_SET_BC(m_GET_BC() + 1); 
		}
		else if (reg_16 == "DE") 
		{ 
			if (m_GET_DE() == 0xFFFF) { m_SET_FLAG('C', 1); };
			m_SET_DE(m_GET_DE() + 1); 
		}
		else if (reg_16 == "HL") 
		{ 
			if (m_GET_HL() == 0xFFFF) { m_SET_FLAG('C', 1); };
			m_SET_HL(m_GET_HL() + 1); 
		}
		else { std::cerr << "ERROR - INC_16 - invalid arguments" << std::endl; exit(69); };
	};

	void DEC_8(uint8_t& reg, uint16_t cycles)
	{
		if (reg == 0x00)
		{
			m_SET_FLAG('C', 0);
			reg = 0xFF;
		}
		else { reg = reg - 1; };
	};

	void DEC_16(std::string reg_16, uint16_t cycles)
	{
		if (reg_16 == "BC")
		{
			uint16_t bctemp = m_GET_BC();
			if (bctemp == 0x0000)
			{
				m_SET_FLAG('C', 0);
				m_SET_BC(0xFFFF);
			}
			else { m_SET_BC(bctemp - 1); };
		}
		else if (reg_16 == "DE")
		{
			uint16_t detemp = m_GET_DE();
			if (detemp == 0x0000)
			{
				m_SET_FLAG('C', 0);
				m_SET_DE(0xFFFF);
			}
			else { m_SET_DE(detemp - 1); };
		}
		else if (reg_16 == "HL")
		{
			uint16_t hltemp = m_GET_HL();
			if (hltemp == 0x0000)
			{
				m_SET_FLAG('C', 0);
				m_SET_HL(0xFFFF);
			}
			else { m_SET_HL(hltemp - 1); };
		}
		else { std::cerr << "ERROR - DEC_16 - invalid argument" << std::endl; exit(69); };
	};

	void RLCA()                   // DONT CONFUSE WITH RLC_A -- it does the same but faster and c/ different flag use
	{                             // S,Z, and P/V are preserved, H and N flags are reset
		uint8_t atemp = m_A;
		m_A << 1;
		if (m_F_Cy == 1)
		{
			m_A += 1;
		};
		if (atemp >= 0b10000000)
		{
			m_F_Cy = 1;
		}
		else { m_F_Cy = 0; };
		m_F_H = 0;
		m_F_N = 0;
	};
	
	void OPCODE_2(uint16_t code)
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
		case 0x01: LD_BC(IMM_16(), 12); m_SET_FLAGS(2, 2, 2, 2);
			break;
		case 0x02: LD_ptr_reg(B, m_A, 8); m_SET_FLAGS(2, 2, 2, 2);
			break;
		case 0x03: INC_16("BC", 8); m_SET_FLAGS(2, 2, 2, 2);
			break;
		case 0x04: INC_8(m_B, 4); m_SET_FLAGS(2, 0, 2, 2);
			break;
		case 0x05: DEC_8(m_B, 4); m_SET_FLAGS(2, 1, 2, 2);
			break;
		case 0x06: LD_8(m_B, IMM_8(), 8); m_SET_FLAGS(2, 2, 2, 2);
			break;
		case 0x07: RLCA();
			break;
		case 0x08: LD_16()
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

enum class opcode_functions
{
	ADC,
	ADD,
	AND,
	BIT,
	CALL,
	CCF,
	CP,
	CPL,
	DAA,
	DEC,
	DI,
	EI,
	HALT,
	INC,
	JP,
	JR,
	LD,
	LDH,
	NOP,
	OR,
	POP,
	PUSH,
	RAA,
	RLA,
	RES,
	RET,
	RLCA,
	RR,
	RRC,
	RRCA,
	RST,
	SBC,
	SCF,
	SET,
	SRA,
	SRL,
	STOP,
	SUB,
	XOR
};

//template <typename T, typename U>
//class OPCODE
//{
//private:
//	uint8_t* m_ptr_tot_mem;
//
//public:
//	OPCODE(GB_Memory& mem, opcode_functions func, T& left_value, bool left_is_ptr, U& right_value, bool right_is_ptr)
//	{
//		m_ptr_tot_mem = mem.Get_Mem_Total_Ptr();
//		T* ptr_left;
//		U* ptr_right;
//
//		if (left_is_ptr == true) { ptr_left = mem.Get_Memory(left_value); }
//		else
//		{
//			ptr_left = &left_value;
//		};
//
//		if (right_is_ptr == true) { ptr_right = mem.Get_Memory(right_value); }
//		else
//		{
//			prt_right = &right_value;
//		};
//
//		switch (func)
//		{
//		case ADC:
//			break;
//		case ADD:
//			break;
//		case AND:
//			break;
//		case BIT:
//			break;
//			CALL,
//			CCF,
//			CP,
//			CPL,
//			DAA,
//			DEC,
//			DI,
//			EI,
//			HALT,
//			INC,
//			JP,
//			JR,
//			LD,
//			LDH,
//			NOP,
//			OR,
//			POP,
//			PUSH,
//			RAA,
//			RLA,
//			RES,
//			RET,
//			RLCA,
//			RR,
//			RRC,
//			RRCA,
//			RST,
//			SBC,
//			SCF,
//			SET,
//			SRA,
//			SRL,
//			STOP,
//			SUB,
//			XOR
//		}
//
//	}
//};