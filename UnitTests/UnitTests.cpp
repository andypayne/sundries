
#include <iostream>

#define RUNTIMER_UNITTEST
#include "../RunTimer.h"

#define BINDATADUMPER_UNITTEST
#include "../DispBinData.h"

#define SIMPLERNG_UNITTEST
#include "../SimpleRNG.h"

#define THREADHELPER_UNITTEST
#include "../ThreadHelper.h"

#define NEURALNET_UNITTEST
#include "../NeuralNet.h"

#define STRINGUTILS_UNITTEST
#include "../StringUtils.h"


int main (int argc, char *argv[])
{

	{
		std::wcout << L"Testing RunTimer.\n";
		UnitTestRunTimer	utrt;
		if (utrt.Test() < 0)
		{
			std::wcout << L"Failed.\n";
		}
	}


	{
		std::wcout << L"Testing DispBinData.\n";
		UnitTestBinDataDumper	utbdd;
		if (utbdd.Test() < 0)
		{
			std::wcout << L"Failed.\n";
		}
	}


	{
		std::wcout << L"Testing SimpleRNG.\n";
		UnitTestRng		utrng;
		if (utrng.Test() < 0)
		{
			std::wcout << L"Failed.\n";
		}
	}


	{
		std::wcout << L"Testing ThreadHelper.\n";
		UnitTestThreadHelper	utth;
		if (utth.Test() < 0)
		{
			std::wcout << L"Failed.\n";
		}
	}


	{
		std::wcout << L"Testing NeuralNet.\n";
		UnitTestNeuralNet	utnn;
		if (utnn.Test() < 0)
		{
			std::wcout << L"Failed.\n";
		}
	}


	{
		std::wcout << L"\nTesting String Stuff.\n";
		UnitTestStringUtils		utsu;
		if (utsu.Test() < 0)
		{
			std::wcout << L"Failed.\n";
		}
		else
		{
			std::wcout << L"Passed.\n";
		}
	}


	return 0;
}

