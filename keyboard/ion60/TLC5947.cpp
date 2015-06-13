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
uint8_t TLC5947::s_nNumChips = 0;
uint16_t** TLC5947::s_pnValues;
uint16_t** TLC5947::s_pnValuesTemp;
bool TLC5947::s_bModified = true;

TLC5947::TLC5947(uint16_t nInitialValue) {
  // Set latch and blank (SS) to outputs
  XLATDDR |= (1<<XLATPIN);
  BLANKDDR |= (1<<BLANKPIN);

  disable();

  XLATPORT &= ~(1<<XLATPIN); // Ensure that the Latch Pin is off

  if (s_nNumChips == 0) {
    // Set MOSI and SCK as outputs
    SCKDDR |= (1<<SCKPIN);
    SINDDR |= (1<<SINPIN);

    // Enable SPI, master
    SPCR = (1<<SPE) | (1<<MSTR);

    // Set clock rate fck/2
    SPSR |= (1<<SPI2X);

    // Send a blank byte so that the SPIF bit is set
    SPDR = 0;
  }

  m_nChip = s_nNumChips; // Set current ID based on number of total chips
  embiggen(); // Embiggen the data array

  for (uint8_t i = 0; i < 24; i++) {
    s_pnValues[m_nChip][i] = nInitialValue;
  }

  update();
}

TLC5947::~TLC5947() {
  disable();

  if (s_nNumChips == 1) {
    // Disable SPI
    SPCR = 0;
  }

  s_nNumChips--;
}

void TLC5947::embiggen(void) {
  if (s_nNumChips) {
    // Allocate a dynamic multidimensional array
    s_pnValuesTemp = new uint16_t*[s_nNumChips + 1];
    for (uint8_t i = 0; i < s_nNumChips + 1; i++) {
      s_pnValuesTemp[i] = new uint16_t[24];
    }

    // Copy the array to the new temporary array
    for (uint8_t i = 0; i < s_nNumChips; i++) {
      for (uint8_t ii = 0; ii < 24; ii++) {
        s_pnValuesTemp[i][ii] = s_pnValues[i][ii];
      }
    }

    // Delete the dynamic multidimensional array
    for (uint8_t i = 0; i < s_nNumChips; i++) {
      delete[] s_pnValues[i];
    }
    delete[] s_pnValues;

    s_pnValues = s_pnValuesTemp; // Point the array at the new array
    s_pnValuesTemp = 0; // Nullify the temporary pointer
  } else {
    // Allocate a dynamic multidimensional array
    s_pnValues = new uint16_t*[s_nNumChips + 1];
    for (uint8_t i = 0; i < s_nNumChips + 1; i++) {
      s_pnValues[i] = new uint16_t[24];
    }
  }

  s_nNumChips++;
}

uint8_t TLC5947::chipID(void) {
  return m_nChip;
}

uint8_t TLC5947::numChips(void) {
  return s_nNumChips;
}

uint16_t TLC5947::read(uint8_t nChannel) {
  // Return the given channel
  if (nChannel < 24) {
    return s_pnValues[m_nChip][nChannel];
  } else {
    return s_pnValues[(nChannel - (nChannel % 24)) / 24][nChannel % 24];
  }
}

void TLC5947::enable(void) {
  // Enable all outputs (BLANK low)
  BLANKPORT &= ~(1<<BLANKPIN);
}

void TLC5947::disable(void) {
  // Disable all outputs (BLANK high)
  BLANKPORT |= (1<<BLANKPIN);
}

void TLC5947::set(uint8_t nChannel, uint16_t nValue) {
  nValue &= 0x0FFF;

  uint8_t i;
  if (nChannel < 24) {
    i = m_nChip;
  } else {
    i = (nChannel - (nChannel % 24)) / 24;
  }

  if (s_pnValues[i][nChannel % 24] != nValue) {
    s_pnValues[i][nChannel % 24] = nValue;
    s_bModified = true;
  }
}

void TLC5947::set(uint16_t nValue) {
  nValue &= 0x0FFF;

  // Set all values to nValue
  for (uint8_t i = 0; i < 24; i++) {
    if (s_pnValues[m_nChip][i] != nValue) {
      s_pnValues[m_nChip][i] = nValue;
      s_bModified = true;
    }
  }
}

void TLC5947::setAll(uint16_t nValue) {
  nValue &= 0x0FFF;

  for (uint8_t i = 0; i < s_nNumChips; i++) {
    for (uint8_t ii = 0; ii < 24; ii++) {
      if (s_pnValues[i][ii] != nValue) {
        s_pnValues[i][ii] = nValue;
        s_bModified = true;
      }
    }
  }
}

void TLC5947::write(uint16_t anValues[24]) {
  for (uint8_t i = 0; i < 24; i++) {
    anValues[i] &= 0x0FFF;

    if (s_pnValues[m_nChip][i] != anValues[i]) {
      s_pnValues[m_nChip][i] = anValues[i];
      s_bModified = true;
    }
  }
}

void TLC5947::clear(void) {
  // Set all values to zero
  for (uint8_t i = 0; i < 24; i++) {
    if (s_pnValues[m_nChip][i]) {
      s_pnValues[m_nChip][i] = 0;
      s_bModified = true;
    }
  }
}

void TLC5947::clearAll(void) {
  // Set all chips to zero
  for (uint8_t i = 0; i < s_nNumChips; i++) {
    for (uint8_t ii = 0; ii < 24; ii++) {
      if (s_pnValues[i][ii]) {
        s_pnValues[i][ii] = 0;
        s_bModified = true;
      }
    }
  }
}

void TLC5947::shift(uint16_t nShift, uint16_t nValue) {
  if (nShift >= (s_nNumChips * 24)) {
    nShift %= (s_nNumChips * 24);
  }

  // Initialize an array to store the overflow data
  uint16_t *p_nValues = new uint16_t[nShift];

  // Save the overflow data to the array
  for (int16_t i = (s_nNumChips * 24) - 1; i >= (s_nNumChips * 24) - nShift; i--) {
    if (nValue == 0xFFFF) {
      // If nValue is unchanged from the default, we are doing a circular
      // rotation. This means that no data should be lost.
      p_nValues[i + nShift - (s_nNumChips * 24)] = s_pnValues[(i - (i % 24)) / 24][i % 24];
    } else {
      // If not, set all the data to the same given value
      p_nValues[i + nShift - (s_nNumChips * 24)] = nValue;
    }
  }

  // Rotate the data by the required number of channels and update the array
  for (int16_t i = (s_nNumChips * 24) - 1; i >= 0; i--) {
    if (i >= nShift) {
      s_pnValues[(i - (i % 24)) / 24][i % 24] = s_pnValues[
        (i - nShift - ((i - nShift) % 24)) / 24][
        (i - nShift) % 24];
    } else {
      // Restore the overflow data
      s_pnValues[(i - (i % 24)) / 24][i % 24] = p_nValues[i];
    }
  }

  // Disable the outputs
  disable();

  // Actually shift the data out to the chips
  for (int16_t i = nShift - 1; i >= 0; i -= 2) {
    if (i == 0) {
      // If we are shifting an odd number of channels, we no longer have a
      // nice whole number of bytes. For the last channel, we have to send
      // a byte and a nibble.
      while(!(SPSR & (1<<SPIF)));
      SPDR = (uint8_t)((p_nValues[i] >> 4) & 0x00FF);

      // TODO: figure out how to properly transfer the last nibble (4 bits)
      while(!(SPSR & (1<<SPIF)));
      for (uint8_t ii = 0; ii < 4; ii++) {
        // Send the current bit
        if (((p_nValues[i] & 0x000F)<<ii) & (1<<3)) {
          SINPORT |= (1<<SINPIN);
        } else {
          SINPORT &= ~(1<<SINPIN);
        }

        // Clock in the current bit (serial clock) [rising edge]
        SCKPORT |= (1<<SCKPIN);
        SCKPORT &= ~(1<<SCKPIN);
      }
    } else {
      // Break every two channels into 3 bytes and send them
      while(!(SPSR & (1<<SPIF)));
      SPDR = (uint8_t)((p_nValues[i] >> 4) & 0x00FF);
      while(!(SPSR & (1<<SPIF)));
      SPDR = (uint8_t)((p_nValues[i] << 4) & 0x00F0) | (uint8_t)((p_nValues[i - 1] >> 8) & 0x000F);
      while(!(SPSR & (1<<SPIF)));
      SPDR = (uint8_t)(p_nValues[i - 1] & 0x00FF);
    }
  }

  // Latch the data (send it to the outputs)
  latch();
  // Enable the outputs
  enable();
  // Clear the modified flag
  s_bModified = false;

  // No memory leaks here!
  delete[] p_nValues;
}

void TLC5947::send(void) {
  // Shift the data out to the chips
  for (int16_t i = (s_nNumChips * 24) - 1; i >= 0; i -= 2) {
    // Break every two channels into 3 bytes and send them
    while(!(SPSR & (1<<SPIF)));
    SPDR = (uint8_t)((s_pnValues[(i - (i % 24)) / 24][i % 24] >> 4) & 0x00FF);
    while(!(SPSR & (1<<SPIF)));
    SPDR = (uint8_t)((s_pnValues[(i - (i % 24)) / 24][i % 24] << 4) & 0x00F0) | (uint8_t)((s_pnValues[((i - 1) - ((i - 1) % 24)) / 24][(i - 1) % 24] >> 8) & 0x000F);
    while(!(SPSR & (1<<SPIF)));
    SPDR = (uint8_t)(s_pnValues[((i - 1) - ((i - 1) % 24)) / 24][(i - 1) % 24] & 0x00FF);
  }
}

void TLC5947::latch(void) {
  // Latch the data to the outputs (rising edge of XLAT)
  XLATPORT |= (1<<XLATPIN);
  XLATPORT &= ~(1<<XLATPIN);
}

void TLC5947::update(void) {
  if (s_bModified) {
    // Disable the outputs
    disable();
    // Shift the data out to the chips
    send();
    // Latch the data to the outputs
    latch();
    // Enable the outputs
    enable();

    // Clear the modified flag
    s_bModified = false;
  }
}
