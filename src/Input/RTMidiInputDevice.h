//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiInputDevice.h 
//!  @brief RTMIDI MidiInputDevice class
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

#ifndef _RT_MIDI_INPUT_INPUT_DEVICE_H_
#define _RT_MIDI_INPUT_INPUT_DEVICE_H_

#include "../Core/RTMidiCore.h"
#include "./RTMidiRXHandler.h"
#include "./RTMidiMessageReceiver.h"
#include "./RTMidiRealtimeControllers.h"
#include "./RTMidiInputChannel.h"

namespace RTMIDI 
{
    class GenericInputDevice: public RxHandler 
    {
        public:
            GenericInputDevice(InputChannelList devChannels,
                               RealtimeController* realtimeController = nullptr):
                realtimeCtrl(realtimeController),
                channels(devChannels){};

            GenericInputDevice(InputChannel* inputChannel,
                               RealtimeController* realtimeController = nullptr):
                realtimeCtrl(realtimeController),
                channels(inputChannel, 1){};

            GenericInputDevice(InputChannel* inputChannels,
                               unsigned int noInputChannels,
                               RealtimeController* realtimeController = nullptr):
                realtimeCtrl(realtimeController),
                channels(inputChannels, noInputChannels){};

            void realtimeMessageReceived(Message msg, Word timestamp) override;
            void sysExStatusChanged(bool terminated, bool startedOrValid) override {};
            void sysExByteReceived(Byte byte) override {};   
        protected:
            RealtimeController *const realtimeCtrl;
            InputChannelList channels;
    };

    template<unsigned int BUFFER_LENGTH, typename BUFFER_INDEX = uint8_t>
    class InputDevice: public GenericInputDevice, 
                       public MessageReceiver<BUFFER_LENGTH, BUFFER_INDEX>
    {
        public:
            InputDevice(InputChannelList devChannels,
                        RealtimeController* realtimeController = nullptr):
                GenericInputDevice(devChannels, realtimeController){};

            InputDevice(InputChannel* inputChannel,
                        RealtimeController* realtimeController = nullptr):
                GenericInputDevice(inputChannel, realtimeController){};

            InputDevice(InputChannel* inputChannels,
                        unsigned int noInputChannels,
                        RealtimeController* realtimeController = nullptr):
                GenericInputDevice(inputChannels, noInputChannels, 
                                   realtimeController){};
            
            void standardMessageReceived(Message msg) override
            {
                this->messageBuffer.push(msg);
            }
        protected:
            void processChannelVoiceMessage(Message msg) override
            {
                channels.dispatchMessage(msg);
            }
            void processSystemCommonMessage(Message msg) override {};
    };
}
#endif