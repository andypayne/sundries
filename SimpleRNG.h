
#ifndef SIMPLERNG_H
#define SIMPLERNG_H

class rng
{
public:
    rng (unsigned long long Seed = 7564231ULL) :
        seed(Seed),
        mult(62089911ULL),
        long_max(4294967295ULL),
        float_max(4294967295.0f)
    {
    }

    inline float operator() ()
    {
        seed = mult * seed;
        return float(seed % long_max) / float_max;
    }

    unsigned long long  seed;
    unsigned long long  mult;
    unsigned long long  long_max;
    float               float_max;
};


#ifdef SIMPLERNG_UNITTEST

struct UnitTestRng
{
	UnitTestRng () { }
	~UnitTestRng () { }

	int Test ()
	{
		rng		ARng;

		for (int i = 0; i < 10; ++i)
		{
			std::wcout << L"[" << i << L"]: " << ARng() << L"\n";
		}

		return 0;
	}
};

#endif


#endif  // #ifndef SIMPLERNG_H
