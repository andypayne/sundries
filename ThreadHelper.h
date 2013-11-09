// Simple thread helper

#ifndef THREADHELPER_H_
#define THREADHELPER_H_

#include <pthread.h>
#include <assert.h>


class ThreadHelper
{
public:
	ThreadHelper () :
		m_ThreadId(0)
	{
	}


	virtual ~ThreadHelper ()
	{
		if (m_ThreadId != 0)
		{
			Stop();
		}
	}


	inline pthread_t ThreadId ()
	{
		return m_ThreadId;
	}


	inline int Start ()
	{
		return ::pthread_create(&m_ThreadId,
					0,
					StaticThreadRoutine,
					reinterpret_cast<void*>(this));
	}


	// Not very gut.
	inline int Stop ()
	{
		return ::pthread_cancel(m_ThreadId);
	}


	inline int Wait ()
	{
		// We're not currently providing valuable return codes
		// right now.
		return ::pthread_join(m_ThreadId, NULL);
	}


protected:
	static void* StaticThreadRoutine (void *Arg)
	{
		ThreadHelper*	THlpr = reinterpret_cast<ThreadHelper*>(Arg);
		assert(THlpr != NULL);
		THlpr->ThreadRoutine();
		return 0;
	}


	virtual void ThreadRoutine ()
	{
		// Base does nothing.
	}


protected:
	pthread_t	m_ThreadId;
};


#ifdef THREADHELPER_UNITTEST

#define TH_LOOP_COUNT	1000


class Thread1 : public ThreadHelper
{
public:
	void ThreadRoutine ()
	{
		for (int i = 0; i < TH_LOOP_COUNT; ++i)
		{
			printf("Thread1: %d\n", i);
		}
	}
};


class Thread2 : public ThreadHelper
{
public:
	void ThreadRoutine ()
	{
		for (int i = 0; i < TH_LOOP_COUNT; ++i)
		{
			printf("Thread2: %d\n", i);
		}
	}
};


class UnitTestThreadHelper
{
public:
	int Test ()
	{
		Thread1		t1;
		Thread2		t2;

		t1.Start();
		t2.Start();
		t1.Wait();
		t2.Wait();
		return 0;
	}
};


#endif



#endif	// #ifndef THREADHELPER_H_

