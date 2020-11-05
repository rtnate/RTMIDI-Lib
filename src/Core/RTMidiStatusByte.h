//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiStatusByte.h 
//!  @brief RTMIDI Status Byte class definition
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

#ifndef _RT_MIDI_STATUS_BYTE_H_
#define _RT_MIDI_STATUS_BYTE_H_

#include "./RTMidiDataByte.h"

namespace RTMIDI 
{
    class StatusByte: public DataByte 
    {
        public:
            static constexpr Byte Min = 0x80;
            static constexpr Byte Max = 0xFF;

            static constexpr Byte SystemCommonMin = 0xF0;
            static constexpr Byte SystemCommonMax = 0xFF;
            static constexpr Byte SystemRealtimeMin = 0xF8;
            static constexpr Byte SystemRealtimeMax = 0xFF;

            static constexpr bool isValid(Byte byte)
            {
                return (byte >= Min) && (byte != Invalid);
            }

            static constexpr bool isInvalid(Byte byte)
            {
                return (byte < Min) || (byte == Invalid);
            }

            static constexpr bool isSystemCommon(Byte byte)
            {
                return (byte >= SystemCommonMin);
            }

            static constexpr bool isChannelVoice(Byte byte)
            {
                return (byte >= Min)||(byte < SystemCommonMax);
            }

            static constexpr bool isSystemRealtime(Byte byte)
            {
                return (byte >= SystemRealtimeMin);
            }

            static constexpr Channel getChannel(Byte byte)
            {
                return isChannelVoice(byte) ? 
                            static_cast<Channel>(lowNibble(byte)) : 
                            ChNone;
            }

            static constexpr StatusCode getStatusCode(Byte byte)
            {
                return static_cast<StatusCode>(byte & 0xF0);
            }

            static constexpr SystemCommonCode getSystemCommonCode(Byte byte)
            {
                return (byte >= SystemCommonMin) ? 
                            static_cast<SystemCommonCode>(byte) : 
                            SystemCommonCode::Invalid;
            }

            static constexpr Byte populate(StatusCode code, Channel ch)
            {
                return ((ch <= Ch15) && (code != StatusCode::SystemCommon)) ? 
                            (static_cast<Byte>(code) | static_cast<Byte>(ch)) : 
                            Invalid;
            }

            static constexpr Byte populate(SystemCommonCode sysCommonCode)
            {
                return static_cast<Byte>(sysCommonCode);
            }

            static bool appliesToChannel(Byte status, Channel ch)
            {
                Channel statusChannel = getChannel(status);
                if (statusChannel == ChNone) return false;
                if (ch == ChOmni) return true;
                return (ch == getChannel(status));
            } 

            /************************************
             *          Constructors            *
             ************************************/

            StatusByte(Byte byte): DataByte(byte){};

            StatusByte(StatusCode code, Channel ch): DataByte(populate(code, ch)){};

            StatusByte(SystemCommonCode code): DataByte(populate(code)){};

            bool isValid() const
            {
                return isValid(c); 
            }

            bool isInvalid() const
            {
                return isInvalid(c);
            }

            bool isSystemCommon() const
            {
                return isSystemCommon(c);
            }

            bool isChannelVoice() const
            {
                return isChannelVoice(c);
            }

            bool isSystemRealtime() const
            {
                return isSystemRealtime(this->c);
            }

            Channel getChannel() const 
            {
                return getChannel(c);
            }

            StatusCode getStatusCode() const 
            {
                return getStatusCode(c);
            }

            SystemCommonCode getSystemCommonCode() const 
            {
                return getSystemCommonCode(c);
            }

            bool isSystemCommonCode(SystemCommonCode code) const 
            {
                return (c == static_cast<Byte>(code));
            }

            bool appliesToChannel(Channel ch) const 
            {
                return appliesToChannel(c, ch);
            }

            void setChannel(Channel ch)
            {
                if (isChannelVoice())
                {
                    c = setLowNibbleInByte(c, ch);
                }
            }

            void setStatusCode(StatusCode code)
            {
                c = static_cast<Byte>(code) | (c & 0x0F);
            }

            StatusByte operator=(SystemCommonCode code)
            {
                return StatusByte(code);
            }
        protected:
    };
}
#endif