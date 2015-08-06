/*
Copyright 2015 Jordi Pakey-Rodriguez <jordi.orlando@hexa.io>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TLC5947.h"

// Static variable definitions
const pin TLC5947::s_SCK = SPI_SCK;
const pin TLC5947::s_MOSI = SPI_MOSI;
pin* TLC5947::s_latch;
pin* TLC5947::s_blank;
uint8_t TLC5947::s_numChips = 0;
uint16_t** TLC5947::s_values;
bool TLC5947::s_modified = true;
bool TLC5947::s_SPIenabled = false;

TLC5947::TLC5947() : TLC5947(s_latch[0], s_blank[0]) {
  // TODO: warn the user if they don't initialize the first chip
  //#if (s_numChips == 0)
  //#  error "You must define pins for the first chip"
  //#endif
}

TLC5947::TLC5947(pin latch, pin blank) {
  // Set current ID based on number of total chips
  m_chip = s_numChips;
  // Embiggen the data array
  embiggen();

  s_latch[m_chip] = latch;
  s_blank[m_chip] = blank;

  // Set latch and blank (SS) to outputs
  *s_latch[m_chip].ddr |= _BV(s_latch[m_chip].pin);
  *s_blank[m_chip].ddr |= _BV(s_blank[m_chip].pin);

  // Set the BLANK pin high
  disable();
  // Ensure that the Latch Pin is off
  *s_latch[m_chip].port &= ~(_BV(s_latch[m_chip].pin));

  if (!m_chip) {
    // Enable the SPI interface
    enableSPI();

    // Send a blank byte so that the SPIF bit is set
    SPDR = 0;
  }

  // Set all channels to start at 0
  clear();
  update();
  enable();
}

TLC5947::~TLC5947() {
  // TODO: properly shrink the arrays
  if (!m_chip) {
    // Disable the SPI interface
    disableSPI();
  }
}

void TLC5947::embiggen(void) {
  if (s_numChips) {
    // TODO: add a buffer in here so that we don't waste any space in RAM
    // Allocate a dynamic multidimensional array
    uint16_t **s_valuesTemp = new uint16_t*[s_numChips + 1];
    for (uint8_t i = 0; i < s_numChips + 1; i++) {
      s_valuesTemp[i] = new uint16_t[24];
    }
    pin *s_latchTemp = new pin[s_numChips + 1];
    pin *s_blankTemp = new pin[s_numChips + 1];

    // Copy the array to the new temporary array
    for (uint8_t i = 0; i < s_numChips; i++) {
      for (uint8_t ii = 0; ii < 24; ii++) {
        s_valuesTemp[i][ii] = s_values[i][ii];
      }

      s_latchTemp[i] = s_latch[i];
      s_blankTemp[i] = s_blank[i];
    }

    // Delete the dynamic multidimensional array
    for (uint8_t i = 0; i < s_numChips; i++) {
      delete[] s_values[i];
    }
    delete[] s_values;
    delete[] s_latch;
    delete[] s_blank;

    // Point the array at the new array location
    s_values = s_valuesTemp;
    s_latch = s_latchTemp;
    s_blank = s_blankTemp;
    // Nullify the temporary pointer
    s_valuesTemp = 0;
    s_latchTemp = 0;
    s_blankTemp = 0;
  } else {
    // Allocate a dynamic multidimensional array
    s_values = new uint16_t*[1];
    for (uint8_t i = 0; i < s_numChips + 1; i++) {
      s_values[i] = new uint16_t[24];
    }

    s_latch = new pin;
    s_blank = new pin;
  }

  s_numChips++;
}

uint8_t TLC5947::chipID(void) {
  return m_chip;
}

uint8_t TLC5947::numChips(void) {
  return s_numChips;
}

uint16_t TLC5947::read(uint8_t channel) {
  // Return the given channel
  if (channel < 24) {
    return s_values[m_chip][channel];
  } else {
    return s_values[(channel - (channel % 24)) / 24][channel % 24];
  }
}

void TLC5947::set(uint16_t values[24]) {
  for (uint8_t i = 0; i < 24; i++) {
    values[i] &= 0x0FFF;

    if (s_values[m_chip][i] != values[i]) {
      s_values[m_chip][i] = values[i];
      s_modified = true;
    }
  }
}

void TLC5947::set(uint16_t value) {
  value &= 0x0FFF;

  // Set all values to value
  for (uint8_t i = 0; i < 24; i++) {
    if (s_values[m_chip][i] != value) {
      s_values[m_chip][i] = value;
      s_modified = true;
    }
  }
}

void TLC5947::set(uint8_t channel, uint16_t value) {
  value &= 0x0FFF;

  uint8_t i;
  if (channel < 24) {
    i = m_chip;
  } else {
    i = (channel - (channel % 24)) / 24;
  }

  if (s_values[i][channel % 24] != value) {
    s_values[i][channel % 24] = value;
    s_modified = true;
  }
}

void TLC5947::setAll(uint16_t value) {
  value &= 0x0FFF;

  for (uint8_t i = 0; i < s_numChips; i++) {
    for (uint8_t ii = 0; ii < 24; ii++) {
      if (s_values[i][ii] != value) {
        s_values[i][ii] = value;
        s_modified = true;
      }
    }
  }
}

void TLC5947::clear(void) {
  // Set all values to zero
  for (uint8_t i = 0; i < 24; i++) {
    if (s_values[m_chip][i]) {
      s_values[m_chip][i] = 0;
      s_modified = true;
    }
  }
}

void TLC5947::clearAll(void) {
  // Set all chips to zero
  for (uint8_t i = 0; i < s_numChips; i++) {
    for (uint8_t ii = 0; ii < 24; ii++) {
      if (s_values[i][ii]) {
        s_values[i][ii] = 0;
        s_modified = true;
      }
    }
  }
}

void TLC5947::enableSPI() {
  // Set MOSI and SCK as outputs
  *s_SCK.ddr |= _BV(s_SCK.pin);
  *s_MOSI.ddr |= _BV(s_MOSI.pin);

  // Enable SPI, master
  SPCR = (1<<SPE) | (1<<MSTR);

  // Set clock rate fck/2
  SPSR |= (1<<SPI2X);

  s_SPIenabled = true;
}

void TLC5947::disableSPI() {
  // Disable SPI
  SPCR = 0;

  // Reset clock rate
  SPSR &= ~(1<<SPI2X);

  s_SPIenabled = false;
}

void TLC5947::enable(void) {
  // Enable all outputs (BLANK low)
  *s_blank[m_chip].port &= ~(_BV(s_blank[m_chip].pin));
}

void TLC5947::enable(uint8_t chip) {
  // Enable all outputs (BLANK low)
  *s_blank[chip].port &= ~(_BV(s_blank[chip].pin));
}

void TLC5947::disable(void) {
  // Disable all outputs (BLANK high)
  *s_blank[m_chip].port |= _BV(s_blank[m_chip].pin);
}

void TLC5947::disable(uint8_t chip) {
  // Disable all outputs (BLANK high)
  *s_blank[chip].port |= _BV(s_blank[chip].pin);
}

void TLC5947::latch(void) {
  // Latch the data to the outputs (rising edge of XLAT)
  *s_latch[m_chip].port |= _BV(s_latch[m_chip].pin);
  *s_latch[m_chip].port &= ~(_BV(s_latch[m_chip].pin));
}

void TLC5947::latch(uint8_t chip) {
  // Latch the data to the outputs (rising edge of XLAT)
  *s_latch[chip].port |= _BV(s_latch[chip].pin);
  *s_latch[chip].port &= ~(_BV(s_latch[chip].pin));
}

void TLC5947::send(void) {
  // Shift the data out to the chips
  for (int16_t i = (s_numChips * 24) - 1; i >= 0; i -= 2) {
    // Break every two channels into 3 bytes and send them
    while(!(SPSR & (1<<SPIF)));
    SPDR = (uint8_t)((s_values[(i - (i % 24)) / 24][i % 24] >> 4) & 0x00FF);
    while(!(SPSR & (1<<SPIF)));
    SPDR = (uint8_t)((s_values[(i - (i % 24)) / 24][i % 24] << 4) & 0x00F0) | (uint8_t)((s_values[((i - 1) - ((i - 1) % 24)) / 24][(i - 1) % 24] >> 8) & 0x000F);
    while(!(SPSR & (1<<SPIF)));
    SPDR = (uint8_t)(s_values[((i - 1) - ((i - 1) % 24)) / 24][(i - 1) % 24] & 0x00FF);
  }
}

void TLC5947::update(void) {
  // TODO: weed out duplicate calls to disable(), enable(), and latch()
  if (s_modified) {
    // Enable SPI if it isn't already on
    if (!s_SPIenabled) {
      enableSPI();
    }
    // Shift the data out to the chips
    send();
    // Latch the data to the outputs
    for (uint8_t i = 0; i < s_numChips; i++) {
      latch(i);
    }

    // Clear the modified flag
    s_modified = false;
  }
}

// TODO: warn the user if they have modified anything before calling shift(). In
// this situation, the un-sent data will be lost.
void TLC5947::shift(uint16_t shift, uint16_t value) {
  if (shift >= (s_numChips * 24)) {
    shift %= (s_numChips * 24);
  }

  // Initialize an array to store the overflow data
  uint16_t *p_values = new uint16_t[shift];

  // Save the overflow data to the array
  for (int16_t i = (s_numChips * 24) - 1; i >= (s_numChips * 24) - shift; i--) {
    if (value == 0xFFFF) {
      // If value is unchanged from the default, we are doing a circular
      // rotation. This means that no data should be lost.
      p_values[i + shift - (s_numChips * 24)] = s_values[(i - (i % 24)) / 24][i % 24];
    } else {
      // If not, set all the data to the same given value
      p_values[i + shift - (s_numChips * 24)] = value;
    }
  }

  // Rotate the data by the required number of channels and update the array
  for (int16_t i = (s_numChips * 24) - 1; i >= 0; i--) {
    if (i >= shift) {
      s_values[(i - (i % 24)) / 24][i % 24] = s_values[
        (i - shift - ((i - shift) % 24)) / 24][
        (i - shift) % 24];
    } else {
      // Restore the overflow data
      s_values[(i - (i % 24)) / 24][i % 24] = p_values[i];
    }
  }

  // Enable SPI if it isn't already on
  if (!s_SPIenabled) {
    enableSPI();
  }

  // Disable the outputs
  for (uint8_t i = 0; i < s_numChips; i++) {
    disable(i);
  }

  // Actually shift the data out to the chips
  for (int16_t i = shift - 1; i >= 0; i -= 2) {
    if (i == 0) {
      // If we are shifting an odd number of channels, we no longer have a
      // nice whole number of bytes. For the last channel, we have to send
      // a byte and a nibble.
      while(!(SPSR & (1<<SPIF)));
      SPDR = (uint8_t)((p_values[i] >> 4) & 0x00FF);

      // TODO: figure out how to properly transfer the last nibble (4 bits)
      while(!(SPSR & (1<<SPIF)));
      for (uint8_t ii = 0; ii < 4; ii++) {
        // Send the current bit
        if (((p_values[i] & 0x000F)<<ii) & (1<<3)) {
          *s_MOSI.port |= _BV(s_MOSI.pin);
        } else {
          *s_MOSI.port &= ~(_BV(s_MOSI.pin));
        }

        // Clock in the current bit (serial clock) [rising edge]
        *s_SCK.port |= _BV(s_SCK.pin);
        *s_SCK.port &= ~(_BV(s_SCK.pin));
      }
    } else {
      // Break every two channels into 3 bytes and send them
      while(!(SPSR & (1<<SPIF)));
      SPDR = (uint8_t)((p_values[i] >> 4) & 0x00FF);
      while(!(SPSR & (1<<SPIF)));
      SPDR = (uint8_t)((p_values[i] << 4) & 0x00F0) | (uint8_t)((p_values[i - 1] >> 8) & 0x000F);
      while(!(SPSR & (1<<SPIF)));
      SPDR = (uint8_t)(p_values[i - 1] & 0x00FF);
    }
  }

  // Latch the data (send it to the outputs)
  for (uint8_t i = 0; i < s_numChips; i++) {
    latch(i);
  }
  // Enable the outputs
  for (uint8_t i = 0; i < s_numChips; i++) {
    enable(i);
  }
  // Clear the modified flag
  s_modified = false;

  // No memory leaks here!
  delete[] p_values;
}
