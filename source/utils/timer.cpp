#include "timer.hpp"
#include "../consts.hpp"

Timer::Timer(void)
{
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;

    mDt = 0;
}


void Timer::Start(void)
{
    mStarted = true;
    mPaused = false;

    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;   
}


void Timer::Stop(void)
{
    mStarted = false;
    mPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}


void Timer::Pause(void)
{
    if(mStarted && !mPaused)
    {
        mPaused = true;
       
        mPausedTicks = SDL_GetTicks() - mStartTicks;
       
        mStartTicks = 0;
    }
}


void Timer::Unpause(void)
{
    if(mStarted && mPaused)
    {
        mPaused = false;

        mStartTicks = SDL_GetTicks() - mPausedTicks;

        mPausedTicks = 0;
    }
}


void Timer::CalculateDt(void)
{
    static Uint32 oldTime = GetTicks();
    
    Uint32 currTime = GetTicks();
    
    mDt = (currTime - oldTime) / Consts::FACTOR_GAME_SPEED;

    oldTime = currTime;
}


Uint32 Timer::GetTicks(void)
{
    Uint32 time = 0;

    if(mStarted)
    {
        if(mPaused)
            time = mPausedTicks;
        else
            time = SDL_GetTicks() - mStartTicks;
    }
    return time;
}


float Timer::GetDt(void) { return mDt; }

bool Timer::IsPaused(void)  { return mPaused; }

bool Timer::IsStarted(void) { return mStarted; }


Uint32 Timer::mStartTicks;

Uint32 Timer::mPausedTicks;

bool Timer::mStarted;

bool Timer::mPaused;

float Timer::mDt;
