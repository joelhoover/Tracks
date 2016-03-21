#pragma once
#include <SFGUI/Label.hpp>

class FrameCounter : public sfg::Label
{
public:
    typedef std::shared_ptr<FrameCounter> Ptr;
    typedef std::shared_ptr<const FrameCounter> PtrConst;

    static Ptr Create();

protected:
    FrameCounter() = default;

private:
    virtual void HandleUpdate(float seconds) override;

    float m_timeElapsed;
    int m_frameCount;
};
