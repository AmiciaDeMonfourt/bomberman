#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include "..\common.hpp"

class Timer
{
    private:

        // static Timer mInstance;
        
        static float mDt;


        static Uint32 mStartTicks;

        static Uint32 mPausedTicks;


        static bool mPaused;

        static bool mStarted;

    public:

        Timer(void);


        static void Start(void);
    
        static void Stop(void);
        
        static void Pause(void);
        
        static void Unpause(void);

        
        static bool IsStarted(void);

        static bool IsPaused(void);


        // static Timer* GetInstance(void);

        static Uint32 GetTicks(void);


        static float GetDt(void);

        static void CalculateDt(void);
};


#endif 