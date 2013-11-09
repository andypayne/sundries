
#ifndef DISPBINDATA_H
#define DISPBINDATA_H


#include <iostream>



class BinDataDumper
{
public:
	BinDataDumper () { }
	~BinDataDumper () { }

	void DispBinData (const void* DataPtr,
					const unsigned long DataSize,
					const int NumCols = 0xf)
	{
		int		i = 0, j = 0, k = 0;

		for (i = 0; i < (DataSize/NumCols + 1); ++i)
		{
			printf("0x%10.10x:", ((NumCols + 1)*i));

			// Numerical values
			for (j=0; (j < NumCols) && ((NumCols*i + j) < DataSize); ++j)
			{
				printf(" %2.2x", *((unsigned char *)DataPtr + (NumCols*i + j)));
			}

			// Pad for last row
			for (k=0; k<((NumCols + 1) - j); ++k)
			{
				printf("   ");
			}

			printf(" | ");

			// Character values
			for (j = 0; (j < NumCols) && ((NumCols*i + j) < DataSize); ++j)
			{
				// Printable check
	//			if (!isgraph((char)map_ptr[0xF*i + j]))
				if (((unsigned char *)DataPtr)[NumCols*i + j] == '\n' ||
					((unsigned char *)DataPtr)[NumCols*i + j] == '\r' ||
					((unsigned char *)DataPtr)[NumCols*i + j] == '\b' ||
					((unsigned char *)DataPtr)[NumCols*i + j] == '\t' ||
					((unsigned char *)DataPtr)[NumCols*i + j] == '\v' ||
					((unsigned char *)DataPtr)[NumCols*i + j] == '\0')
				{
					printf(".");
				}
				else
				{
					printf("%c", ((unsigned char *)DataPtr)[(NumCols*i + j)]);
				}
			}

			printf("\n");
		}
	}

};



#ifdef BINDATADUMPER_UNITTEST

struct UnitTestBinDataDumper
{
	UnitTestBinDataDumper () { }
	~UnitTestBinDataDumper () { }

	int Test ()
	{
		std::string		str;

		for (int i = 0; i < 255; ++i)
		{
			str += i;
		}


		BinDataDumper	bdd;
		int				NumCols = 10;
		bdd.DispBinData(reinterpret_cast<const void*>(str.c_str()), str.size(), NumCols);
		return 0;
	}
};

#endif



#endif	// #ifndef DISPBINDATA_H

