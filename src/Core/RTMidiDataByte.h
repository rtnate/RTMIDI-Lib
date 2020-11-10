//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiDataByte.h 
//!  @brief RTMIDI DataByte class definition
//!
//!  @author Nate Taylor 

//!  Contact: nate@rtelectronix.com
//!  @copyright (C) 2020  Nate Taylor - All Rights Reserved.
//
//      |------------------------------------------------------------------------------------|
//      |                                                                                    |
//      |               MMMMMMMMMMMMMMMMMMMMMM   NNNNNNNNNNNNNNNNNN                          |
//      |               MMMMMMMMMMMMMMMMMMMMMM   NNNNNNNNNNNNNNNNNN                          |
//      |              MMMMMMMMM    MMMMMMMMMM       NNNNNMNNN                               |
//      |              MMMMMMMM:    MMMMMMMMMM       NNNNNNNN                                |
//      |             MMMMMMMMMMMMMMMMMMMMMMM       NNNNNNNNN                                |
//      |            MMMMMMMMMMMMMMMMMMMMMM         NNNNNNNN                                 |
//      |            MMMMMMMM     MMMMMMM          NNNNNNNN                                  |
//      |           MMMMMMMMM    MMMMMMMM         NNNNNNNNN                                  |
//      |           MMMMMMMM     MMMMMMM          NNNNNNNN                                   |
//      |          MMMMMMMM     MMMMMMM          NNNNNNNNN                                   |
//      |                      MMMMMMMM        NNNNNNNNNN                                    |
//      |                     MMMMMMMMM       NNNNNNNNNNN                                    |
//      |                     MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                |
//      |                   MMMMMMM      E L E C T R O N I X         MMMMMM                  |
//      |                    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                    |
//      |                                                                                    |
//      |------------------------------------------------------------------------------------|
//
//      |------------------------------------------------------------------------------------|
//      |                                                                                    |
//      |      [MIT License]                                                                 |
//      |                                                                                    |
//      |      Copyright (c) 2020 Nathaniel Taylor                                           |
//      |                                                                                    |
//      |      Permission is hereby granted, free of charge, to any person                   |
//      |      obtaining a copy of this software and associated documentation                |
//      |      files (the "Software"), to deal in the Software without                     |
//      |      restriction, including without limitation the rights to use,                  |
//      |      copy, modify, merge, publish, distribute, sublicense, and/or sell             |
//      |      copies of the Software, and to permit persons to whom the Software            |
//      |      is furnished to do so, subject to the following conditions:                   |
//      |                                                                                    |
//      |      The above copyright notice and this permission notice shall be                |
//      |      included in all copies or substantial portions of the Software.               |
//      |                                                                                    |
//      |      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,             |
//      |      EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES               |
//      |      OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                      |
//      |      NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS           |
//      |      BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN               |
//      |      AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF                |
//      |      OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS               |
//      |      IN THESOFTWARE.                                                               |
//      |                                                                                    |
//      |------------------------------------------------------------------------------------|
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef _RT_MIDI_CORE_DATA_BYTE_H_ 
#define _RT_MIDI_CORE_DATA_BYTE_H_

#include "./RTMidiCoreTypes.h"

namespace RTMIDI 
{
    class DataByte 
    {
        public:
            static constexpr Byte Min = 0;

            static constexpr Byte Max = 0x7F;

            static constexpr Byte Invalid = 0xFD;

            static constexpr bool isDataByte(Byte byte)
            {
                return (byte <= Max);
            }

            static constexpr bool isStatusByte(Byte byte)
            {
                return (byte > Max);
            }

            static constexpr bool isNotDataByte(Byte byte)
            {
                return isStatusByte(byte);
            }

            static constexpr bool isNotStatusByte(Byte byte)
            {
                return isDataByte(byte);
            }

            static constexpr bool isValid(Byte byte)
            {
                return (byte != Invalid);
            }

            static constexpr bool isInvalid(Byte byte)
            {
                return (byte == Invalid);
            }

            static constexpr Byte lowNibble(Byte byte)
            {
                return byte & 0xF;
            }

            static constexpr Byte highNibble(Byte byte)
            {
                return byte >> 4;
            }

            static constexpr bool lowNibbleMatches(Byte byte, Byte test)
            {
                return ((byte && 0xF) == (test && 0xF));
            }

            static constexpr bool highNibbleMatches(Byte byte, Byte test)
            {
                return ((byte && 0xF0) == (test << 4));
            }

            static constexpr Byte setLowNibbleInByte(Byte original, Byte newNibble)
            {
                return (original & 0xF0) | (newNibble & 0x0F);
            }

            static constexpr Byte setHighNibbleInByte(Byte original, Byte newNibble)
            {
                return (original & 0x0F) | (newNibble << 4);
            }

            /**
             * @brief Concenates two MIDI Data bytes to get a 14-bit value
             * 
             * @param lsb The byte containing the least significant 7 bits
             * @param msb The byte containing the most significant 7 bits
             * @return constexpr Word The 14 bit unsigned value
             */
            static constexpr Word concatenate(Byte lsb, Byte msb)
            {
                return ( (static_cast<Word>(msb) & 0x7F) << 7 ) |
                       (static_cast<Word>(lsb) & 0x7F);
            }

            DataByte(Byte byte): c(byte){};

            DataByte(): c(Invalid){};

            bool isDataByte() const { return isDataByte(c); };

            bool isStatusByte() const { return isStatusByte(c); };

            bool isNotDataByte() const { return isNotDataByte(c); };

            bool isNotStatusByte() const { return isNotStatusByte(c); };

            bool isValid() const { return isValid(c); };

            bool isInvalid() const { return isInvalid(c); };

            Byte lowNibble() const { return lowNibble(c); };

            Byte highNibble() const { return highNibble(c); };

            bool lowNibbleMatches(Byte test) const 
            { 
                return lowNibbleMatches(c, test);
            };

            bool highNibbleMatches(Byte test) const 
            {
                return highNibbleMatches(c, test);
            };

            bool matches(Byte test) const { return (c == test); };

            operator Byte() const { return c; };

            explicit operator bool() const { return isDataByte(); };

            DataByte operator=(Byte b)
            {
                return DataByte(b);
            }

            friend DataByte operator%(DataByte a, Byte b)
            {
                return DataByte(a.c % b);
            };

            friend DataByte operator^(DataByte a, Byte b)
            {
                return DataByte(a.c ^ b);
            };

            friend DataByte operator&(DataByte a, Byte b)
            {
                return DataByte(a.c & b);
            };

            friend DataByte operator|(DataByte a, Byte b)
            {
                return DataByte(a.c & b);
            };

        protected:
            Byte c;
    };
}
#endif