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
    /**
     * @brief Class defining a MIDI Input channel.  
     * 
     *        A MIDI input channel is an object that receives MIDI channel
     *        voice messages from a MIDI Input device.  The InputChannel class
     *        can be assigned an RTMIDI::Channel.  If set to a single MIDI 
     *        channel, it will forward messages to an attached InputChannelListener
     *        if the voice message belongs to the correct channel.  It may 
     *        also be set to ChOmni to receive messages from all channels, 
     *        or ChNone to receive no messages.
     * 
     */
    class InputChannel
    {
        public:
            /**
             * @brief Default constructor creates an InputChannel with no 
             *        listener attached and the channel set to ChNone.
             */
            InputChannel(): listener(nullptr), midiCh(ChNone){};

            /**
             * @brief Constructs an input channel with the supplied 
             *        Channel and (optionally) the supplied listener.
             * 
             * @param ch The MIDI Channel to assign the InputChannel to.
             * 
             * @param initialListener (optional) The initial InputChannelListener 
             *                                   to attatch to the InputChannel.
             */
            InputChannel(Channel ch, 
                         InputChannelListener* initialListener = nullptr): 
                listener(initialListener), midiCh(ch){};

            /**
             * @brief Sends a message to the input channel.  If this channel
             *        is assigned to a Channel that should receive the message,
             *        it will forward it appropriately to the InputChannel's
             *        listener (if attached)
             * 
             * @param msg The message to send to the InputChannel
             */
            void sendMessage(Message msg);
            
            /**
             * @brief Attaches the provided InputChannelListener object 
             *        the this InputChannel.
             * 
             * @param newListener A pointer to an InputChannelListener object 
             *                    that should handle events from this channel.
             */
            void attachListener(InputChannelListener* newListener)
            {
                listener = newListener;
            }

            /**
             * @brief Dettaches the currentlty assigned InputChannelListener
             *        from this InputChannel
             * 
             */
            void dettachListener()
            {   
                listener = nullptr;
            }

            /**
             * @brief Get this InputChannel's currently assigned MIDI Channel 
             * 
             * @return The Channel this InputChannel is assigned to
             */
            Channel midiChannel() const { return midiCh; };

            /**
             * @brief Set this InputChannel's currently assigned MIDI Channel
             * 
             * @param ch The Channel to assign this InputChannel to
             */
            void setMidiChannel(Channel ch){ midiCh = ch; };
        protected:
            /**
             * @brief The currently attached InputChannelListener object,
             *        or nullptr if none is attached.
             */
            InputChannelListener* listener;

            /**
             * @brief The currentlty assigned MIDI Channel.
             */
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
                        list[i].sendMessage(msg);
                    }
                }
            }
        protected:
            InputChannel* list;
            unsigned int length;
    };
}
#endif
