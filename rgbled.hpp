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

class RgbLed
{
  private:

    uint16_t red;
    uint16_t green;
    uint16_t blue;

  public:

    RgbLed() : red(0), green(0), blue(0) {}

    RgbLed(uint16_t _red, uint16_t _green, uint16_t _blue)
        : red(_red), green(_green), blue(_blue)
    { }

    void setRed(uint16_t _red) { red = _red; }

    void setGreen(uint16_t _green) { green = _green; }
    void setBlue(uint16_t _blue) { blue = _blue; }

    void setRedHigh(uint8_t _red)
    {
        red &= 0x00ff;
        red |= static_cast<uint16_t>(_red) << 8;
    }

    void setGreenHigh(uint8_t _green)
    {
        green &= 0x00ff;
        green |= static_cast<uint16_t>(_green) << 8;
    }

    void setBlueHigh(uint8_t _blue)
    {
        blue &= 0x00ff;
        blue |= static_cast<uint16_t>(_blue) << 8;
    }

    void setRedLow(uint8_t _red)
    {
        red &= 0xff00;
        red |= static_cast<uint16_t>(_red);
    }

    void setGreenLow(uint8_t _green)
    {
        green &= 0xff00;
        green |= static_cast<uint16_t>(_green);
    }

    void setBlueLow(uint8_t _blue)
    {
        blue &= 0xff00;
        blue |= static_cast<uint16_t>(_blue);
    }

    uint16_t getRed()   const { return red; }
    uint16_t getGreen() const { return green; }
    uint16_t getBlue()  const { return blue; }

};
