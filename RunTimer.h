
#ifndef RUNTIMER_H
#define RUNTIMER_H


#include <sys/time.h>
#include <unistd.h>
#include <iostream>	// printf() debug
#include <cassert>



struct UnixTimeProvider
{
	typedef timeval		TimeType;
	TimeType	StartTime;
	TimeType	StopTime;

	UnixTimeProvider ()
	{
		timerclear(&StartTime);
		timerclear(&StopTime);
	}

	inline void Start () { GetTime(StartTime); }
	inline void Stop () { GetTime(StopTime); }

	void GetTime (TimeType& TimeNow)
	{
		struct timezone	TimeZone = { 0, 0 };
		if (gettimeofday(&TimeNow, &TimeZone) < 0)
		{
			assert(0);
		}
	}

	inline TimeType Elapsed () const
	{
		TimeType	elapsed;
		timersub(&StopTime, &StartTime, &elapsed);
		return elapsed;
	}

	inline void PrintElapsed () const
	{
		printf("Elapsed time: %ld:%d (sec:microsec).\n",
			Elapsed().tv_sec,
			Elapsed().tv_usec);
	}


};



template <class TimeProvider = UnixTimeProvider>
struct ScopeTimer
{
	ScopeTimer (TimeProvider& t) : m_Timer(t) { m_Timer.Start(); }
	~ScopeTimer () { m_Timer.Stop(); }
	TimeProvider&	m_Timer;
};

typedef ScopeTimer<>	DefScopeTimer;


#ifdef RUNTIMER_UNITTEST

class UnitTestRunTimer
{
public:
	UnitTestRunTimer () { }
	~UnitTestRunTimer () { }

	int Test ()
	{
		UnixTimeProvider	utp;

		{
			DefScopeTimer		st(utp);
			sleep(3);
		}

		utp.PrintElapsed();
		return 0;
	}

};

#endif



#endif	// #ifndef RUNTIMER_H


