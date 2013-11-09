// Console Allocation

#ifndef _CONSOLEWIN_H
#define _CONSOLEWIN_H


#include <iostream>
#include <string>

#include <windows.h>



class ConsoleWin
{
public:

	ConsoleWin ()
	{
		// TODO: Check for failure
		GetConsole();

		if (GetHandles() < 0)
		{
			// Error
		}
	}


	virtual ~ConsoleWin ()
	{
		ReleaseConsole();
	}


	inline DWORD Write (CONST TCHAR * Buf, DWORD Size)
	{
		return ::WriteConsole(hStdout, Buf, Size, NULL, NULL);
	}

	// TODO: This.
	DWORD Read (TCHAR * Buf, DWORD Size);


	inline BOOL ReleaseConsole ()
	{
		return ::FreeConsole();
	}


protected:

	int ConsoleWin::GetHandles ()
	{
		hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
		if (hStdin == INVALID_HANDLE_VALUE)
		{
			return -1;
		}

		hStdout = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdout == INVALID_HANDLE_VALUE)
		{
			return -1;
		}

		hStderr = ::GetStdHandle(STD_ERROR_HANDLE);
		if (hStderr == INVALID_HANDLE_VALUE)
		{
			return -1;
		}

		return 0;
	}


	int ConsoleWin::GetConsole ()
	{
		::AllocConsole();

		::freopen("CONIN$","rb",stdin);
		::freopen("CONOUT$","wb",stdout);
		::freopen("CONOUT$","wb",stderr);

		return 0;
	}


protected:
	HANDLE	hStdin;
	HANDLE	hStdout;
	HANDLE	hStderr;

};




#ifdef CONSOLEWIN_UNITTEST

struct UnitTestConsoleWin
{
	UnitTestConsoleWin () { }
	~UnitTestConsoleWin () { }

	int Test ()
	{
		ConsoleWin		cons;
		std::string		buf("Testing ConsoleWin.\nStill testing.\n");

		cons.Write(buf.c_str(), buf.length());
		return 0;
	}
};

#endif



#endif	// #ifndef _CONSOLEWIN_H

