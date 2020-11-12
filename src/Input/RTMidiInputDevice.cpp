

#include "./RTMidiInputDevice.h"

#include "../../../SAMD21/include/SAMD21_Debug.h"

using namespace RTMIDI;

void GenericInputDevice::realtimeMessageReceived(Message msg, Word timestamp)
{   
    if (!realtimeCtrl) return;
    auto code = msg.getStatus().getSystemCommonCode();
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
