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
#include "../Rx/RTMidiRXHandler.h"
#include "../Rx/RTMidiMessageHandler.h"
#include "./RealtimeControllers.h"
#include "./RTMidiInputChannel.h"

namespace RTMIDI 
{
    class InputDevice: public RxHandler, public MessageHandler<64, uint8_t>
    {
        public:
            InputDevice(InputChannelList& devChannels,
                        RealtimeClockController* devClockController = nullptr,
                        RealtimeController* devRealtimeController = nullptr):
                clockController(devClockController),
                realtimeController(devRealtimeController),
                channels(devChannels){};

            InputDevice(InputChannel* devChannel,
                        RealtimeClockController* devClockController = nullptr,
                        RealtimeController* devRealtimeController = nullptr):
                clockController(devClockController),
                realtimeController(devRealtimeController),
                channels(devChannel, 1){};

            InputDevice(InputChannel* devChannels,
                        unsigned int noChannels,
                        RealtimeClockController* devClockController = nullptr,
                        RealtimeController* devRealtimeController = nullptr):
                clockController(devClockController),
                realtimeController(devRealtimeController),
                channels(devChannels, noChannels){};
            
            void standardMessageReceived(Message msg) override;
            void realtimeMessageReceived(Message msg, Word timestamp) override;
            void sysExStatusChanged(bool terminated, bool startedOrValid) override;
            void sysExByteReceived(Byte byte) override;
        protected:
            RealtimeClockController *const clockController;
            RealtimeController *const realtimeController;
            InputChannelList channels;

            void processChannelVoiceMessage(Message msg) override;
            void processSystemCommonMessage(Message msg) override;
    };
}
#endif