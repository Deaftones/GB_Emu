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
	bool m_A[8]; //A - accumulator; where arhythmetic occurs
	bool m_F0, m_F1, m_F2, m_F3, m_F4, m_F5, m_F6, m_F6; // Flag register
	bool m_B[8], m_C[8];
	bool m_D[16], m_E[16];
	bool m_H[8], m_L[8]; //	can be used as two separate 8-bit regs or one 16-bit reg. Used to point to memory.
	bool m_SP; // Stack Pointer;
	bool m_PC; // Program counter === points to memory containing next instruction to be executed.

	// Boot instruction located at 0x0100 in Memory.

public:
	//8-bit Load Commands:
	void COPY_REGISTRY_TO_REGISTRY(bool reg_to_be_copied[], bool reg_to_copy_to[])
	{
		
	}


};