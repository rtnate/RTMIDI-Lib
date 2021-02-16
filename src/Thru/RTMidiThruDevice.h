//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiThruDevice.h 
//!  @brief RT GenericMidiThru Device and MidiThruDevice class definitions
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

#ifndef _RT_MIDI_THRU_DEVICE_H_
#define _RT_MIDI_THRU_DEVICE_H_

#include "../Core/RTMidiCore.h"
#include "../Input/RTMidiInputs.h"
#include "../Output/RTMidiOutputs.h"

namespace RTMIDI 
{
    class GenericThruDevice: public GenericInputDevice, public GenericOutputDevice
    {
        public:
            GenericThruDevice(InputChannelList devChannels,
                               RealtimeController* realtimeController = nullptr):
                GenericInputDevice(devChannels, realtimeController){};

            GenericThruDevice(InputChannel* inputChannel,
                               RealtimeController* realtimeController = nullptr):
                GenericInputDevice(inputChannel, realtimeController){};

            GenericThruDevice(InputChannel* inputChannels,
                               unsigned int noInputChannels,
                               RealtimeController* realtimeController = nullptr):
                GenericInputDevice(inputChannels, 
                                   noInputChannels, 
                                   realtimeController){};

            void realtimeMessageReceived(Message msg, Word timestamp) override;
        protected:
            bool thruEnabled;
            bool realtimeThruEnabled;
    };

    template<unsigned int BUFFER_LENGTH, typename BUFFER_INDEX = uint8_t>
    class ThruDevice: public GenericThruDevice, 
                       public MessageReceiver<BUFFER_LENGTH, BUFFER_INDEX>
    {
        public:
            ThruDevice(InputChannelList devChannels,
                        RealtimeController* realtimeController = nullptr):
                GenericThruDevice(devChannels, realtimeController){};

            ThruDevice(InputChannel* inputChannel,
                        RealtimeController* realtimeController = nullptr):
                GenericThruDevice(inputChannel, realtimeController){};

            ThruDevice(InputChannel* inputChannels,
                        unsigned int noInputChannels,
                        RealtimeController* realtimeController = nullptr):
                GenericThruDevice(inputChannels, noInputChannels, 
                                   realtimeController){};
            
            void standardMessageReceived(Message msg) override
            {
                this->messageBuffer.push(msg);
                if (this->thruEnabled) this->thruBuffer.push(msg);
            };

            void sendMessage(Message msg) override 
            {
                transmitBuffer.push(msg);
            }

        protected:
            MessageBuffer<BUFFER_LENGTH, BUFFER_INDEX> transmitBuffer;
            MessageBuffer<BUFFER_LENGTH, BUFFER_INDEX> thruBuffer;

            void processChannelVoiceMessage(Message msg) override
            {
                channels.dispatchMessage(msg);
            }

            void processSystemCommonMessage(Message msg) override {};

            Message getNextMessage() override 
            {
                if (thruBuffer.available())
                {
                    return thruBuffer.pop();
                }
                else if (transmitBuffer.available())
                {
                    return transmitBuffer.pop();
                }
                else return Message::invalid();
            }
    };
}


#endif