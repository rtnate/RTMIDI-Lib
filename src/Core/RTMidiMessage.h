//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiMessage.h 
//!  @brief RTMIDI Message class definition
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

#ifndef _RT_MIDI_CORE_MESSAGE_H_
#define _RT_MIDI_CORE_MESSAGE_H_

#include "./RTMidiCoreTypes.h"
#include "./RTMidiDataByte.h"
#include "./RTMidiStatusByte.h"

namespace RTMIDI 
{
    /**
     * @brief  Standard MIDI Message wrapper class.
     * 
     *          This class wraps a StandardMessageData structure 
     *          providing easy functionality for accessing and manipulating
     *          midi message components.  The StandardMessageData struture 
     *          contains a MIDI message packed into a 32 bit word.
     * 
     *          Since the data is packed efficiently into 32 bits, this class 
     *          is designed to be copied and passsed by value rather than by 
     *          pointer or reference.
     * 
     */
    class Message 
    {
        public:

            /************************************
             *          Constructors            *
             ************************************/

            Message(): Message(DataByte::Invalid, 
                               DataByte::Invalid, 
                               DataByte::Invalid){};

            Message(SystemCommonCode code, 
                    DataByte data0 = DataByte::Invalid, 
                    DataByte data1 = DataByte::Invalid):
                        Message(StatusByte(code), data0, data1){};

            Message(StatusByte status):
                Message(static_cast<Byte>(status), 
                        DataByte::Invalid, 
                        DataByte::Invalid){};

            Message(StatusByte status, DataByte data):
                Message(static_cast<Byte>(status), 
                        static_cast<Byte>(data), 
                        DataByte::Invalid){};

            Message(StatusByte status, DataByte data0, DataByte data1):
                Message(static_cast<Byte>(status), 
                        static_cast<Byte>(data0), 
                        static_cast<Byte>(data1)){};

            Message(Byte status, Byte data0, Byte data1 = DataByte::Invalid): 
                msg{ .byte={0, status, data0, data1}}{};

            Message(Word dataWord):
                msg{ .word=dataWord }{};

            Message(StandardMessageData message): msg(message){};

            Message(StandardMessageItems message): msg{ .items={message}}{};

            /************************************
             *          Public Methods          *
             ************************************/

            /**
             * @brief Get the MIDI Message's Status Byte
             * 
             * @return This messages Status Byte 
             */
            StatusByte getStatus() const 
            {
                return StatusByte(msg.items.status);
            }

            /**
             * @brief Get the MIDI Message's Data Byte
             * 
             *        If the supplied index is greater than 1, 
             *        this function will retrieve data byte 0 for 
             *        even numbers or data byte 1 for odd numbers.
             * 
             * @param index The byte index to retrieve (0 or 1)
             * @return The requested data byte 
             */
            DataByte getDataByte(Byte index) const 
            {
                return DataByte(msg.items.data[index & 0x1]);
            }

            /**
             * @brief Get the MIDI Message's first data byte
             * 
             * @return This message's first data byte 
             */
            DataByte getFirstDataByte() const 
            {
                return DataByte(msg.items.data[0]);
            }

            /**
             * @brief Get the MIDI Message's second data byte
             * 
             * @return This message's second data byte 
             */
            DataByte getSecondDataByte() const 
            {
                return DataByte(msg.items.data[1]);
            }

            /**
             * @brief Get the MIDI Message's data bytes
             * 
             *        This function will return a pointer to this 
             *        messages data bytes.  This is a const pointer 
             *        to an array of length 2.
             * 
             * @return Byte* A pointer to the messages data bytes.
             */
            const Byte* getDataBytes()
            {
                return msg.items.data;
            }

            /**
             * @brief Get the MIDI Message's content bytes
             * 
             *        This function will return a pointer to this 
             *        messages status byte and data bytes as an array.  
             *        This is a const pointer to an array of length 3.
             * 
             * @return Byte* A pointer to the messages data bytes.
             */
            const Byte* getBytes() 
            {
                return &(msg.byte[1]);
            }

            /**
             * @brief Get the MIDI Message's underlying data structure.
             * 
             * @return The StandardMessageData structure containing the message 
             *         data.
             */
            StandardMessageData data()
            {
                return msg;
            }

            /**
             * @brief Check if this is a valid MIDI Message.
             * 
             *        A valid MIDI messag is any message that has a status 
             *        that is not invalid.
             * 
             * @return True is valid, false if not.
             */
            bool isValid() const 
            {
                return getStatus().isValid();
            }

            /**
             * @brief  Operator overload allowing this class to be implicitly
             *         cast to its underlying data structure.
             * 
             * @return The message as a StandardMessageData structure
             */
            operator StandardMessageData()
            {
                return msg;
            }

            /**
             * @brief  Operator overload allowing this class to be implicitly 
             *         cast to a data word.
             * 
             * @return Word 
             */
            operator Word()
            {
                return msg.word;
            }


        protected:
            StandardMessageData msg;
    };
}
#endif