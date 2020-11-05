//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiRingBuffer.h 
//!  @brief RTMIDI Ring Buffer template class definition
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

#ifndef _RT_MIDI_CORE_RING_BUFFER_H_
#define _RT_MIDI_CORE_RING_BUFFER_H_

#include <RingBuffer.h>

namespace RTMIDI 
{
    /**
     * @brief Template class for a RingBuffer.
     * 
     *        A ring buffer is a first-in, first-out buffer 
     *        that stores items of class T up to length LENGTH.
     * 
     *        Storing an item adds it to the buffer, reading it removes it.
     * 
     * @tparam T The class of items that the buffer stores
     * 
     * @tparam LENGTH The length of the ring buffer
     * 
     * @tparam INDEX_TYPE The index storage type. It should be an unsigned  
     *                    integer capable of holding LENGTH
     */
    template<class T, unsigned int LENGTH, typename INDEX_TYPE>
    class RingBuffer 
    {
        public:
            /**
             * @brief Get the ring buffer's length
             * 
             * @return The length of the buffer in items
             */
            static constexpr unsigned int length(){ return LENGTH; };

            /**
             * @brief Construct a new empty RingBuffer
             */
            RingBuffer(): head(0), tail(0), buffer{}{};

            /**
             * @brief  Construct a new RingBuffer with the supplied data.
             *
             *         This will store the number of items "length" into the 
             *         buffer.  If "length" is greater than the length of the 
             *         buffer, the buffer will be filled to capacity and 
             *         and data will be truncated.
             * 
             * @param initialData  A pointer to an array of data to fill the 
             *                     buffer with.
             * @param length The length of the initial data array.
             */
            RingBuffer(T* initialData, unsigned int length): head(0), tail(0)
            {
                fill(initialData, length);
            }

            /**
             * @brief  Fill the RingBuffer with the supplied data.
             *
             *         This will store the number of items "length" into the 
             *         buffer.  If "length" is greater than the length of the 
             *         buffer, the buffer will be filled to capacity and 
             *         and data will be truncated.
             * 
             * @param initialData  A pointer to an array of data to fill the 
             *                     buffer with.
             * @param length The length of the initial data array.
             * 
             * @return The number of items copied into the buffer
             */
            unsigned int fill(T* initialData, unsigned int length)
            {
                if (length > LENGTH) length = LENGTH;
                for(unsigned int i = 0; i < length; i++)
                {
                    push(initialData[i]);
                }
                return length;
            }

            /**
             * @brief  Clears the buffer.  This does *NOT* erase the buffers 
             *         memory;
             */
            void clear()
            {
                head = 0; tail = 0;
            }

            /**
             * @brief Retrieve an item from the tail of the buffer and 
             *        removes it.
             * 
             * @return The next item at the tail of the buffer.
             */
            T pop()
            {
                if(head == tail) return T();
                T item = buffer[tail];
                tail = nextIndex(tail);
                return item;
            }

            /**
             * @brief Reads an item from the tail of the buffer and
             *        removes it.
             * 
             * @return The next item at the tail of the buffer.
             */
            T read(){ return pop(); };

            /**
             * @brief Adds an item to the head of the buffer.
             * 
             * @param item The item to add.
             */
            void push(T item)
            {
                INDEX_TYPE nextHead = nextIndex(head);
                if ( nextHead != tail )
                {
                    buffer[head] = item;
                    head = nextHead;
                }
            }

             /**
             * @brief Stores an item to the head of the buffer.
             * 
             * @param item The item to add.
             */
            void store(T item){ push(item); };

            /**
             * @brief Retrieves the item at the tail of the buffer without 
             *        removing it.
             * 
             * @return The next item at the tail of the buffer.
             */
            T peek()
            {
                if (tail == head) return T();
                else return buffer[tail];
            }

            /**
             * @brief Retrieves the number of items currently stored in the 
             *        buffer.
             * 
             * @return The number of items in the buffer available for retrieval
             */
            int available()
            {
                int difference = head - tail;
                if (difference < 0) return difference + LENGTH;
                else return difference;
            }

            /**
             * @brief Get the amount of free space available in the buffer.
             * 
             * @return The number of items that can be stored in the buffer 
             *         before it will be full.
             */
            int freeSpace()
            {
                if (head >= tail)
                    return LENGTH - 1 - head + tail;
                else
                    return tail - head - 1;
            }

            /**
             * @brief Checks if the buffer is currently full.
             * 
             * @return True if there is no free space available.
             */
            bool isFull()
            {
                //If the index of the next head will match the tail,
                //then the next item would overwrite the tail of the 
                //buffer.  This means the buffer is full.
                return (nextIndex(head) == tail) ? true : false;
            }

            /**
             * @brief   Operator overload for array access.
             * 
             *          This allows for reading/setting buffer 
             *          values directly. The supplied index will 
             *          automatically "wrap around" in case it is 
             *          out of bounds.
             * 
             * @param index The index of the item to retrieve.
             * @return The item at the supplied index
             */
            T& operator[](unsigned int index)
            {
                return buffer[index % LENGTH];
            }

        protected:
            /**
             * @brief The buffer's head index value
             */
            volatile INDEX_TYPE head;

            /**
             * @brief The buffer's tail index value
             */
            volatile INDEX_TYPE tail;

            /**
             * @brief The buffer's storage array
             */
            T buffer[LENGTH];

            /**
             * @brief Gets an index's next value, automatically 
             *        wrapping around at the end of the buffer.
             * 
             * @param current The current index value
             * @return The next index value 
             */
            INDEX_TYPE nextIndex(INDEX_TYPE current)
            {
                return (current + 1) % LENGTH;
            }
    };
}
#endif