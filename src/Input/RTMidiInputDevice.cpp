

#include "./RTMidiInputDevice.h"

using namespace RTMIDI;

void InputDevice::standardMessageReceived(Message msg)
{
    messageBuffer.push(msg);
}

void InputDevice::realtimeMessageReceived(Message msg, Word timestamp)
{   
    auto code = msg.getStatus().getSystemCommonCode();
    switch(code)
    {
        case SystemCommonCode::TimingClock:
            if (clockController) 
                clockController->registerClockPulse(timestamp);
            return;
        case SystemCommonCode::Start:
            if (realtimeController) 
                realtimeController->start();
            return;
        case SystemCommonCode::Continue:
            if (realtimeController) 
                realtimeController->resume();
            return;
        case SystemCommonCode::Stop:
            if (realtimeController) 
                realtimeController->stop();
            return;
        default:
            return;
    }
}

void InputDevice::sysExStatusChanged(bool terminated, bool startedOrValid)
{

}

void InputDevice::sysExByteReceived(Byte byte)
{

}

void InputDevice::processChannelVoiceMessage(Message msg)
{
    channels.dispatchMessage(msg);
}

void InputDevice::processSystemCommonMessage(Message msg)
{

}