#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <ios>
#include <bit>

class Mem_Byte
{
public:
	bool bit[8];
};


class Memory_Load
{
private:
	Mem_Byte* m_ROM_Bank_00_16KB;
	Mem_Byte* m_ROM_Bank_01_to_NN_16KB;
	Mem_Byte* m_vRAM_8KB;
	Mem_Byte* m_external_RAM_8KB;
	Mem_Byte* m_work_RAM_0_4KB;
	Mem_Byte* m_work_RAM_1_4KB;
	Mem_Byte* m_echo_of_m_work_RAM_0_4KB;
	Mem_Byte* m_sprite_attribute_table;
	Mem_Byte* m_NOT_USABLE_MEMORY;
	Mem_Byte* m_IO_ports;
	Mem_Byte* m_high_RAM;
	Mem_Byte* m_interrupt_enable_register;
	Mem_Byte* m_total_memory;
	

public:
	Memory_Load()
	{
		// M A I N   M E M O R Y  ===  and Partitions

		m_total_memory = new Mem_Byte[0xFFFF];		
		m_ROM_Bank_00_16KB = &m_total_memory[0x0000];
		m_ROM_Bank_01_to_NN_16KB = &m_total_memory[0x4000];
		m_vRAM_8KB = &m_total_memory[0x8000];
		m_external_RAM_8KB = &m_total_memory[0xA000];
		m_work_RAM_0_4KB = &m_total_memory[0xC000];
		m_work_RAM_1_4KB = &m_total_memory[0xD000];
		m_echo_of_m_work_RAM_0_4KB = &m_total_memory[0xE000];
		m_sprite_attribute_table = &m_total_memory[0xFE00];
		m_NOT_USABLE_MEMORY = &m_total_memory[0xFEA0];
		m_IO_ports = &m_total_memory[0xFF00];
		m_high_RAM = &m_total_memory[0xFF80];
		m_interrupt_enable_register = &m_total_memory[0xFFFF];

		// C P U   M E M O R Y   &   EXTERNAL MEM

		// RST Commands typically used @ addresses 0000, 0008, 0010, 0018, 0020, 0028, 0030, and 0038.
		// Interrupts used @ addresses 0040, 0048, 0050, 0058, and 0060.
		// Cartridge header stored in memory @ 0100 to 014F.
		// External memory and hardware: can use 0000 to 7FFF and A000 to BFFF.
		//       External memory is often battery buffered, and may hold saved game positions and high scrore tables
		//       (etc.) even when the gameboy is turned of, or when the cartridge is removed.

		// === L C D   CONTROL REGISTER  === //
		Mem_Byte* LCD_control_reg_FF40 = &m_total_memory[0xFF40];
	};

	bool READ_bit_at_address(uint16_t hex_mem_address, uint16_t bit_position)
	{
		if (hex_mem_address < 0x0000 || hex_mem_address > 0xFFFF)
		{
			std::cerr << "ERROR: Memory address does not exist. Range 0x0000 to 0xFFFF" << std::endl; exit(5);
		};
		if (bit_position < 0 || bit_position > 7)
		{
			std::cerr << "ERROR: Bit position must be 0 to 7" << std::endl; exit(5);
		};
		return m_total_memory[hex_mem_address].bit[bit_position];
	}

	Mem_Byte READ_byte_at_address_ascending(uint16_t hex_mem_address)
	{
		if (hex_mem_address < 0x0000 || hex_mem_address > 0xFFFF)
		{
			std::cerr << "ERROR: Memory address does not exist. Range 0x0000 to 0xFFFF" << std::endl; exit(5);
		};
		Mem_Byte* ptr_mem_addr = &m_total_memory[hex_mem_address];
		Mem_Byte ascend;
		for (uint16_t i = 0; i < 8; ++i)
		{
			ascend.bit[i] = ptr_mem_addr->bit[i];
		}
		ptr_mem_addr = nullptr;
		return ascend;
	};

	Mem_Byte READ_byte_at_address_descending(uint16_t hex_mem_address)
	{
		if (hex_mem_address < 0x0000 || hex_mem_address > 0xFFFF)
		{
			std::cerr << "ERROR: Memory address does not exist. Range 0x0000 to 0xFFFF" << std::endl; exit(5);
		};
		Mem_Byte* ptr_mem_addr = &m_total_memory[hex_mem_address];
		Mem_Byte descend;
		for (uint16_t i = 0; i < 8; ++i)
		{
			descend.bit[i] = ptr_mem_addr->bit[7-i];
		}
		ptr_mem_addr = nullptr;
		return descend;
	};

	void WRITE_bit(uint16_t hex_mem_address, uint16_t bit_position, bool value)
	{
		if (hex_mem_address < 0x0000 || hex_mem_address > 0xFFFF)
		{
			std::cerr << "ERROR: Memory address does not exist. Range 0x0000 to 0xFFFF" << std::endl; exit(5);
		};
		if (bit_position < 0 || bit_position > 7)
		{
			std::cerr << "ERROR: Bit position must be 0 to 7" << std::endl; exit(5);
		};

		m_total_memory[hex_mem_address].bit[bit_position] = value;
	};

	void WRITE_byte_ascending(uint16_t hex_mem_address, bool mem_array[])
	{
		if (hex_mem_address < 0x0000 || hex_mem_address > 0xFFFF)
		{
			std::cerr << "ERROR: Memory address does not exist. Range 0x0000 to 0xFFFF" << std::endl; exit(5);
		};

		// REMINDER: have not implemented mem_array error checking yet. To do.

		Mem_Byte* ptr_mem_addr = &m_total_memory[hex_mem_address];
		for (uint16_t i = 0; i < 8; ++i)
		{
			ptr_mem_addr->bit[i] = mem_array[i];
		};

		ptr_mem_addr = nullptr;
	};

	void WRITE_byte_descending(uint16_t hex_mem_address, bool mem_array[])
	{
		if (hex_mem_address < 0x0000 || hex_mem_address > 0xFFFF)
		{
			std::cerr << "ERROR: Memory address does not exist. Range 0x0000 to 0xFFFF" << std::endl; exit(5);
		};

		// REMINDER: have not implemented mem_array error checking yet. To do.

		Mem_Byte* ptr_mem_addr = &m_total_memory[hex_mem_address];
		for (uint16_t i = 0; i < 8; ++i)
		{
			ptr_mem_addr->bit[i] = mem_array[7 - i];
		};

		ptr_mem_addr = nullptr;
	};

	~Memory_Load()
	{
		delete[] m_total_memory;
		m_ROM_Bank_00_16KB = nullptr;
		m_ROM_Bank_01_to_NN_16KB = nullptr;
		m_vRAM_8KB = nullptr;
		m_external_RAM_8KB = nullptr;
		m_work_RAM_0_4KB = nullptr;
		m_work_RAM_1_4KB = nullptr;
		m_echo_of_m_work_RAM_0_4KB = nullptr;
		m_sprite_attribute_table = nullptr;
		m_NOT_USABLE_MEMORY = nullptr;
		m_IO_ports = nullptr;
		m_high_RAM = nullptr;
		m_interrupt_enable_register = nullptr;
		m_total_memory = nullptr;
	};
};