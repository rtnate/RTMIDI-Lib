//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file MidiInputChannel.h 
//!  @brief MidiInputChannel class definition
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

#ifndef _RT_MIDI_INPUT_INPUT_CHANNEL
#define _RT_MIDI_INPUT_INPUT_CHANNEL

#include "../Core/RTMidiCore.h"
#include "./RTMidiInputChannelListener.h"

namespace RTMIDI 
{
    class InputChannel
    {
        public:
            void processMessage(Message msg);
            
            void attachListener(InputChannelListener* newListener)
            {
                listener = newListener;
            }
            void dettachListener()
            {   
                listener = nullptr;
            }

            Channel midiChannel() const { return midiCh; };

            void setMidiChannel(Channel ch){ midiCh = ch; };
        protected:
            InputChannelListener* listener;
            Channel midiCh;
    };

    class InputChannelList 
    {
        public:
            InputChannelList(InputChannel* channels, 
                             unsigned int listLength = 1):
                                list(channels), length(listLength){};

            InputChannelList(const InputChannelList& other):
                list(other.list), length(other.length){};

            void dispatchMessage(Message msg)
            {
                if (length > 0)
                {
                    for(unsigned int i = 0; i < length; i++)
                    {
                        list[i].processMessage(msg);
                    }
                }
            }
        protected:
            InputChannel* list;
            unsigned int length;
    };
}
#endif
