//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//!  @file RTMidiThruDevice.cpp 
//!  @brief RTMIDI ThruDevice class implementations
//!
//!  @author Nate Taylor 

//!  Contact: nate@rtelectronix.com
//!  @copyright (C) 2020  RT ElecTRonix - All Rights Reserved.
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
//      |                                                                                    |
//      |      Copyright (c) 2020 RT ElecTRonix, All Rights Reserved                         |
//      |                                                                                    |
//      |      This file is exclusive property of RT ElecTRonix.                             |
//      |      This file may not be reproduced or distributed without expressed              |
//      |      consent from RT ElecTRonix.                                                   |
//      |                                                                                    |
//      |------------------------------------------------------------------------------------|
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "./RTMidiThruDevice.h"

using namespace RTMIDI;

void GenericThruDevice::realtimeMessageReceived(Message msg, Word timestamp)
{   
    auto status = msg.getStatus();
    if (realtimeThruEnabled) this->setRealtimeByte(static_cast<Byte>(status));
    if (!realtimeCtrl) return;
    auto code = status.getSystemCommonCode();
    switch(code)
    {
        case SystemCommonCode::TimingClock:
            realtimeCtrl->registerClockPulse(timestamp);
            return;
        case SystemCommonCode::Start:
            realtimeCtrl->start();
            return;
        case SystemCommonCode::Continue:
            realtimeCtrl->resume();
            return;
        case SystemCommonCode::Stop:
            realtimeCtrl->stop();
            return;
        default:
            return;
    }
}