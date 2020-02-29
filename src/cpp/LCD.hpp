#ifndef LCD_HPP
#define LCD_HPP

#include "../c/LCD.h"

namespace LCD
{
[[maybe_unused]] constexpr uint32_t volatile & set  {GPIOB->BSRR};
[[maybe_unused]] constexpr uint32_t volatile & reset{GPIOB->BRR };

[[maybe_unused]] constexpr auto RST  = int{1 << 11};
[[maybe_unused]] constexpr auto CS   = int{1 << 12};
[[maybe_unused]] constexpr auto SCLK = int{1 << 13};
[[maybe_unused]] constexpr auto SOD  = int{1 << 14};
[[maybe_unused]] constexpr auto SID  = int{1 << 15};



[[maybe_unused]] constexpr auto clear_display        = uint8_t{0x1 << 0}; // C++14: 0b00000001
	
[[maybe_unused]] constexpr auto return_home          = uint8_t{0x1 << 1}; // C++14: 0b00000010

[[maybe_unused]] constexpr auto view                 = uint8_t{0x1 << 2}; // C++14: 0b00000100
[[maybe_unused]] constexpr auto  bottom              = uint8_t{0x1 << 1}; // C++14: 0b00000010
[[maybe_unused]] constexpr auto  top                 = uint8_t{0x1 << 0}; // C++14: 0b00000001

[[maybe_unused]] constexpr auto display              = uint8_t{0x1 << 3}; // C++14: 0b00001000
[[maybe_unused]] constexpr auto  display_on          = uint8_t{0x1 << 2}; // C++14: 0b00000100
[[maybe_unused]] constexpr auto  cursor_on           = uint8_t{0x1 << 1}; // C++14: 0b00000010
[[maybe_unused]] constexpr auto  blink_on            = uint8_t{0x1 << 0}; // C++14: 0b00000001

[[maybe_unused]] constexpr auto follower_control     = uint8_t{0x3 << 5}; // C++14: 0b01100000
[[maybe_unused]] constexpr auto  follower_circuit_on = uint8_t{0x1 << 3}; // C++14: 0b00001000


template <typename Data = uint8_t>
class Display
{
	Data m_on{display};
	
public:
	auto init_ports() -> Display<Data> &;
	auto reset() -> Display<Data> &;

	auto init() -> Display<Data> &;

	template <typename... Args>
	auto operator()(Data start_byte, Args... data) -> Display<Data> &;

	auto clear() -> Display<Data> &;
	auto on(bool on = true) -> Display<Data> &;
	auto cursor(bool on = true) -> Display<Data> &;
	auto blink(bool on = true) -> Display<Data> &;

	auto operator<<(char c) -> Display<Data> &;
	auto operator<<(char str[]) -> Display<Data> &;
};

template <typename Data>
auto Display<Data>::init_ports() -> Display<Data> &
{
	LCD_init_ports();
	return *this;
}
template <typename Data>
auto Display<Data>::reset() -> Display<Data> &
{
	LCD_reset();
	return *this;
}

template <typename Data>
auto Display<Data>::init() -> Display<Data> &
{
	(*this)
		.init_ports()
		.reset();
	GPIOB->BSRR = CS;
	GPIOB->BSRR = SCLK; wait_ms(1);
	return *this;
}

template <typename Data>
template <typename... Args>
auto Display<Data>::operator()(Data start_byte, Args... data) -> Display<Data> &
{
	SPI_start(start_byte);
	Data data_array[sizeof...(data)] {(Data)data...};
	LCD_data_count(data_array, sizeof...(data));
	return *this;
}

template <typename Data>
auto Display<Data>::clear() -> Display<Data> &
{
	return (*this)(LCD_execute, clear_display);
}

template <typename Data>
auto Display<Data>::on(bool on) -> Display<Data> &
{
	if (on) m_on |=  display_on;
	else    m_on &= ~display_on;
	return (*this)(LCD_execute, m_on);
}
template <typename Data>
auto Display<Data>::cursor(bool on) -> Display<Data> &
{
	if (on) m_on |=  cursor_on;
	else    m_on &= ~cursor_on;
	return (*this)(LCD_execute, m_on);
}
template <typename Data>
auto Display<Data>::blink(bool on) -> Display<Data> &
{
	if (on) m_on |=  blink_on;
	else    m_on &= ~blink_on;
	return (*this)(LCD_execute, m_on);
}


template <typename Data>
auto Display<Data>::operator<<(char c) -> Display<Data> &
{
	SPI_start(LCD_output);
	LCD_data(c);
	return *this;
}
template <typename Data>
auto Display<Data>::operator<<(char str[]) -> Display<Data> &
{
	SPI_start(LCD_output);
	LCD_data_0term((uint8_t*)str);
	return *this;
}

}

#endif // LCD_HPP
