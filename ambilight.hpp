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
#include <xpcc/driver/pwm/pca9685.hpp>
#include <xpcc/processing/protothread/protothread.hpp>

#include "rgbled.hpp"

template<class I2cMaster>
class Ambilight : public xpcc::pt::Protothread
{
  public:

    static constexpr uint8_t controllerBaseAddress = 80;
    static constexpr uint8_t numControllers = 6;
    static constexpr uint8_t numControllerRgbLeds = 5;
    static constexpr uint8_t numRgbLeds = numControllers * numControllerRgbLeds;

  private:

    xpcc::Pca9685<I2cMaster> controller[numControllers] =
        { {controllerBaseAddress + 0},
          {controllerBaseAddress + 1},
          {controllerBaseAddress + 2},
          {controllerBaseAddress + 3},
          {controllerBaseAddress + 4},
          {controllerBaseAddress + 5} };

    RgbLed* leds;

  public:

    Ambilight(RgbLed* _leds) : leds(_leds) {}

    uint8_t getController(uint8_t led) { return led / numControllerRgbLeds; }

    uint8_t getRedChannel(uint8_t led)
    {
        uint8_t map[numControllers] = { 2, 3, 9, 12, 13 };

        return map[led % numControllerRgbLeds];
    }

    uint8_t getGreenChannel(uint8_t led)
    {
        uint8_t map[numControllers] = { 1, 4, 7, 11, 14 };

        return map[led % numControllerRgbLeds];
    }

    uint8_t getBlueChannel(uint8_t led)
    {
        uint8_t map[numControllers] = { 0, 5, 6, 10, 15 };

        return map[led % numControllerRgbLeds];
    }

    bool run()
    {
        static uint8_t c = 0;
        static uint8_t led = 0;
        static uint8_t channel = 0;

        PT_BEGIN();

        for (c = 0; c < numControllers; c++)
        {
            PT_CALL(controller[c].initialize(0, xpcc::pca9685::MODE2_INVRT));
            PT_CALL(controller[c].setAllChannels(0x0));
        }

        while (true)
        {
            for (c = 0; c < numControllers; c++)
                PT_CALL(controller[c].initialize(0, xpcc::pca9685::MODE2_INVRT));

            for (led = 0; led < numRgbLeds; led++)
            {
                c = getController(led);

                channel = getRedChannel(led);
                PT_CALL(controller[c].setChannel(channel, leds[led].getRed()));

                channel = getGreenChannel(led);
                PT_CALL(controller[c].setChannel(channel, leds[led].getGreen()));

                channel = getBlueChannel(led);
                PT_CALL(controller[c].setChannel(channel, leds[led].getBlue()));
            }
        }

        PT_END();
    }
};
