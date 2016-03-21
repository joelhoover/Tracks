#include "FrameCounter.hpp"

FrameCounter::Ptr FrameCounter::Create(){
    Ptr frameCounter(new FrameCounter());
    return frameCounter;
}

void FrameCounter::HandleUpdate(float seconds){
    m_timeElapsed += seconds;
    m_frameCount += 1;
    if (m_timeElapsed >= 1.f){
        SetText("FPS: " + std::to_string(m_frameCount));
        m_timeElapsed -= 1.f;
        m_frameCount = 0;
    }
}
