/* The MIT License (MIT)
 *
 * Copyright (c) 2015 Christian Menard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../../xpcc/examples/stm32f4_discovery/stm32f4_discovery.hpp"

#include <xpcc/architecture/platform.hpp>
#include <xpcc/io/iostream.hpp>

#include "ambilight.hpp"
#include "rgbled.hpp"
#include "mono.hpp"

MAIN_FUNCTION
{
	Board::initialize();

	Board::LedRed::set();
	Board::LedRed::reset();

	// Enable USART 1
	GpioOutputA9::connect(Usart1::Tx);
	GpioInputA10::connect(Usart1::Rx, Gpio::InputType::PullUp);
	Usart1::initialize<Board::systemClock, 921600>(12);

	// Enable USART 2
	GpioOutputA2::connect(Usart2::Tx);
	GpioInputA3::connect(Usart2::Rx, Gpio::InputType::PullUp);
	Usart2::initialize<Board::systemClock, 9600>(12);

	GpioB11::connect(I2cMaster2::Sda);
	GpioB10::connect(I2cMaster2::Scl);
	I2cMaster2::initialize<Board::systemClock, 560000>();

	// Create a IOStream for complex formatting tasks
	xpcc::IODeviceWrapper< Usart2, xpcc::IOBuffer::BlockIfFull > device;
	xpcc::IOStream stream(device);

	stream << "Hello World!\r\n";

	RgbLed leds[30] = {};
    auto ambilight = Ambilight<I2cMaster2>(leds);
	auto mono = Mono<Usart1>(leds);

	while (1)
	{
		Board::LedGreen::set();
		Board::LedRed::set();

		ambilight.run();
		Board::LedGreen::reset();
		mono.run();
		Board::LedRed::reset();
	}

	return 0;
}
