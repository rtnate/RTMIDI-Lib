//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiOutputChannel.h 
//!  @brief RTMIDI OutputChannel class definition
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

#ifndef _RT_MIDI_OUTPUT_OUTPUT_CHANNEL_H_
#define _RT_MIDI_OUTPUT_OUTPUT_CHANNEL_H_

#include "../Core/RTMidiCore.h"
#include "./RTMidiTransmitter.h"

namespace RTMIDI 
{
    class OutputChannel: public Transmitter
    {
        public:
            OutputChannel(Channel ch = ChNone, 
                          Transmitter* attachedTransmitter = nullptr): 
                transmitter(attachedTransmitter), channel(ch){};

            void attachTransmitter(Transmitter* newTransmitter)
            {
                //Prevent an accidental circular reference where this becomes
                //it's own transmitter, that could be very bad
                if (newTransmitter == static_cast<Transmitter*>(this))
                {
                    return;
                }
                transmitter = newTransmitter;
            };

            void dettachTransmitter()
            {
                transmitter = nullptr;
            };

            void sendProgramChangeMessage(Byte programNumber)
            {
                transmitMessage(Message::createProgramChange(channel, 
                                                             programNumber));
            };

            void sendControlChangeMessage(Byte ccNumber, Byte ccValue)
            {
                transmitMessage(Message::createControlChange(channel, 
                                                             ccNumber,
                                                             ccValue));
            };

            void sendMessage(Message msg) override
            {
                if (!msg.getStatus().isChannelVoice()) return;
                msg.setChannel(channel);
                transmitMessage(msg);
            }

        protected:
            Transmitter* transmitter;
            Channel channel;
            void transmitMessage(Message msg)
            {
                if (transmitter) transmitter->sendMessage(msg);
            }
    };

    class OutputChannelList 
    {
        public:

            OutputChannelList(OutputChannel* channels, 
                             unsigned int listLength = 1):
                                list(channels), length(listLength){};

            OutputChannelList(const OutputChannelList& other):
                list(other.list), length(other.length){};

            void attachTransmitter(Transmitter* newTransmitter)
            {
                for (unsigned int i = 0; i < length; i++)
                {
                    list[i].attachTransmitter(newTransmitter);
                }
            }
        protected:
            OutputChannel* list;
            unsigned int length;
    };
}
#endif