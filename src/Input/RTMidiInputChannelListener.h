//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiInputChannelListener.h 
//!  @brief RTMIDI InputChannelListener class definition
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

#ifndef _RT_MIDI_INPUT_INPUT_CHANEL_LISTENER_H_
#define _RT_MIDI_INPUT_INPUT_CHANEL_LISTENER_H_

#include "../Core/RTMidiCore.h"

namespace RTMIDI 
{
    /**
     * @brief Interface class for a Midi Input Channel listener.  Classes 
     *        that process input channel events should inherit from this 
     *        class.
     * 
     */
    class InputChannelListener 
    {
        public:
            /**
             * @brief Event handler called when a control change message 
             *        is received.
             * 
             * @param number The control change number (0-127)
             * @param value  The control change value (0-127)
             */
            virtual void controlChangeReceived(Byte number, Byte value) = 0;

            /**
             * @brief Event handler called when a program change message is 
             *        received.
             * 
             * @param number The program number (0-127)
             */
            virtual void programChangeReceived(Byte number) = 0;

            /**
             * @brief Event handler called when a note event is received.
             * 
             * @param note The note (key) number (0-127)
             * @param velocity The velocity value (0-127) 
             * @param noteOn True is this is a Note On message, 
             *               false if it is a Note Off message.
             */
            virtual void noteEventReceived(Byte note, Byte velocity, bool noteOn) = 0;

            /**
             * @brief Event handler called when an aftertouch event 
             *        is received.
             * 
             *        If this is a polyphonic key pressure event,
             *        the key value will be less than 0x80 and contain 
             *        the key number.  If this is a channel pressure event,
             *        the key value will be 0x80 or greater.
             * 
             * @param pressure The key pressure value (0-127)
             * @param key The key (note) number (0-127) if this is a polyphonic 
             *            key pressure event, or DataByte::Invalid is this is 
             *            a channel pressure event.
             */
            virtual void aftertouchReceived(Byte pressure, Byte key = DataByte::Invalid) = 0;

            /**
             * @brief Event handler called when a pitch bend change event 
             *        is received.
             * 
             * @param lsb The least significant seven data bits
             * @param msb The most significant seven data bits
             */
            virtual void pitchBendChangeReceived(Byte lsb, Byte msb) = 0;
    };
}
#endif