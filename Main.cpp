#include <iostream>
#include <bitset>

void m_SET_BC(uint16_t val)
{
	uint8_t m_B, m_C;
	m_B = ((uint16_t)val >> 8 & 0xFF);
	m_C = ((uint16_t)val >> 0 & 0xFF);

	std::cout << "B = " << std::bitset<8>(m_B) << "\nC = " << std::bitset<8>(m_C);
};

int main()
{
	m_SET_BC(0b1010101000001111);

	return 0;
}