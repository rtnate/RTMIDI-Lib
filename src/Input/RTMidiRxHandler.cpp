//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiRxHandler.cpp 
//!  @brief RTMIDI RxHandler class implementations
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

#include "./RTMidiRXHandler.h"

using namespace RTMIDI;

void RxHandler::receiveByte(Byte ip, Word timestamp)
{
    if (DataByte::isStatusByte(ip))
    {
        processStatusByte(ip, timestamp);
    }
    else processDataByte(ip);
}

void RxHandler::processStatusByte(Byte ip, Word timestamp)
{
    StatusByte status(ip);
    if (status.isSystemRealtime())
    {
        this->realtimeMessageReceived(Message(status), timestamp);
    }
    else
    {
        if (status.isSystemCommonCode(SystemCommonCode::SysExStart))
        {
            sysExInProgress = true;
            this->sysExStatusChanged(false, true);
        }
        else 
        {
            sysExInProgress = false;
            runningStatusBuffer = status;
            thirdByteExpected = false;
            this->sysExStatusChanged(true, false);
            if (status.isSystemCommonCode(SystemCommonCode::TuneRequest))
            {
                this->standardMessageReceived(Message(ip));
            }
        }
    }
}

void RxHandler::processDataByte(Byte ip)
{
    if (sysExInProgress)
    {
        this->sysExByteReceived(ip);
        return;
    }
    if (thirdByteExpected)
    {
        thirdByteExpected = 0;
        if (runningStatusBuffer >= 0xF0) runningStatusBuffer = 0;
        Message msg(runningStatusBuffer, dataByteBuffer, ip);
        this->standardMessageReceived(msg);
    }
    else 
    {
        if (runningStatusBuffer == 0) return;
        if (runningStatusBuffer < 0xC0) 
        {
            thirdByteExpected = true;
            dataByteBuffer = ip;
            return;
        }
        else if (runningStatusBuffer < 0xE0)
        {
            this->standardMessageReceived(Message(runningStatusBuffer, ip));
            return;
        }
        else if (runningStatusBuffer < 0xF0)
        {
            thirdByteExpected = true;
            dataByteBuffer = ip;
            return;
        }
        else
        {
            if (runningStatusBuffer == 0xF2)
            {
                thirdByteExpected = true;
                dataByteBuffer = ip;
                return;
            }
            if (runningStatusBuffer == 0xF3 || runningStatusBuffer == 0xF2)
            {
                Message msg(runningStatusBuffer, ip);
                runningStatusBuffer = 0;
                this->standardMessageReceived(msg);
            }
        }
    }
}

void RxHandler::receiveMessage(Message msg, Word timestamp)
{
    if (msg.getStatus().isSystemRealtime())
    {
        this->realtimeMessageReceived(msg, timestamp);
    }
    else this->standardMessageReceived(msg);
}