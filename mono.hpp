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

#pragma once

#include <xpcc/architecture/platform.hpp>
#include <xpcc/ui/color.hpp>

template<typename Uart>
class Mono : public xpcc::pt::Protothread
{
  private:

      xpcc::color::RgbT<uint16_t>* leds;

  public:

    Mono(xpcc::color::RgbT<uint16_t>* _leds) : leds(_leds) { }

    bool run()
    {
        static uint8_t led = 0;

        static uint8_t data;

        static uint16_t value = 0;

        PT_BEGIN();

        while (true)
        {

            PT_WAIT_UNTIL(Uart::read(data));

            if (data != 0xff)
                continue;

            PT_WAIT_UNTIL(Uart::read(data));

            if (data != 0xaa)
                continue;

            PT_WAIT_UNTIL(Uart::read(data));

            if (data != 0x55)
                continue;

            PT_WAIT_UNTIL(Uart::read(data));

            if (data != 0x00)
                continue;

            // if we reach this point we read all four prefix bytes and can now
            // read led values
            for (led = 0; led < 30; led++)
            {
                value = 0;
                PT_WAIT_UNTIL(Uart::read(data));
                value = data << 8;
                PT_WAIT_UNTIL(Uart::read(data));
                value |= data;
                leds[led].red = value;

                value = 0;
                PT_WAIT_UNTIL(Uart::read(data));
                value = data << 8;
                PT_WAIT_UNTIL(Uart::read(data));
                value |= data;
                leds[led].green = value;

                value = 0;
                PT_WAIT_UNTIL(Uart::read(data));
                value = data << 8;
                PT_WAIT_UNTIL(Uart::read(data));
                value |= data;
                leds[led].blue = value;
            }
        }

        PT_END();
    }

};
