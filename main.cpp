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
#include <xpcc/processing.hpp>
#include <xpcc/ui/button_group.hpp>

#include "ambilight.hpp"
#include "rgbled.hpp"
#include "mono.hpp"

enum class Mode
{
	MONO,
	ALL_ON,
	ALL_OFF,
};

void setLeds(Mode mode)
{
	switch (mode)
	{
	case Mode::MONO:
		Board::LedGreen::set();
		Board::LedRed::reset();
		Board::LedBlue::reset();
		Board::LedOrange::reset();
		break;
	case Mode::ALL_OFF:
		Board::LedGreen::reset();
		Board::LedRed::reset();
		Board::LedBlue::reset();
		Board::LedOrange::reset();
		break;
	case Mode::ALL_ON:
		Board::LedGreen::reset();
		Board::LedRed::reset();
		Board::LedBlue::set();
		Board::LedOrange::reset();
		break;
	default:
		Board::LedGreen::reset();
		Board::LedRed::reset();
		Board::LedBlue::reset();
		Board::LedOrange::reset();
		break;
	}
}

MAIN_FUNCTION
{
	Board::initialize();

	// Enable USART 1
	GpioOutputA9::connect(Usart1::Tx);
	GpioInputA10::connect(Usart1::Rx, Gpio::InputType::PullUp);
	Usart1::initialize<Board::systemClock, 921600>(12);

	GpioB11::connect(I2cMaster2::Sda);
	GpioB10::connect(I2cMaster2::Scl);
	I2cMaster2::initialize<Board::systemClock, 560000>();

	RgbLed leds[30] = {};
    auto ambilight = Ambilight<I2cMaster2>(leds);
	auto mono = Mono<Usart1>(leds);

	Mode mode = Mode::MONO;

	// use a ButtonGroup to debounce the key
	auto button = xpcc::ButtonGroup<uint8_t>(1);

	xpcc::ShortPeriodicTimer timer(5);

	setLeds(mode);

	while (1)
	{
		// update button periodically for debouncing
		if (timer.execute())
			button.update(Board::Button::read() ? 0 : 1);

		if (button.isPressed(1))
		{
			switch (mode)
			{
			case Mode::MONO:
				mode = Mode::ALL_ON;

				for (int i = 0; i < 30; i++)
				{
					leds[i].setRed(0xffffff);
					leds[i].setGreen(0xffffff);
					leds[i].setBlue(0xffffff);
				}

				break;
			case Mode::ALL_ON:
				mode = Mode::ALL_OFF;

				for (int i = 0; i < 30; i++)
				{
					leds[i].setRed(0x0);
					leds[i].setGreen(0x0);
					leds[i].setBlue(0x0);
				}

				break;
			case Mode::ALL_OFF:
				mode = Mode::MONO;

				break;
			default:
				break;
			}

			setLeds(mode);
		}

		switch (mode)
		{
		case Mode::MONO:
			ambilight.run();
			mono.run();
			break;
		case Mode::ALL_ON:
			ambilight.run();
			break;
		case Mode::ALL_OFF:
			ambilight.run();
			break;
		default:
			break;
		}
	}

	return 0;
}
