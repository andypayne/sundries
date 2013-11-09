
#ifndef STRINGUTILS_H
#define STRINGUTILS_H


#include <iostream>
#include <string>

#include <wchar.h>


#ifndef MAX_PATH
#define MAX_PATH 255
#endif


namespace std
{
#ifdef _UNICODE
	typedef wstring			tstring;
	typedef wostringstream	tostringstream;
#else
	typedef string			tstring;
	typedef ostringstream	tostringstream;
#endif
};


#define MBTOW(dst, src)		\
	(mbstowcs(dst, src, strlen(src) + 1))

#define WTOMB(dst, src)		\
	(wcstombs(dst, src, wcslen(src) + 1))


// TODO: This depends on std::(w)string::c_str() being contiguous.
std::wstring str2wstr (const std::string& Str)
{
	std::wstring	Wstr;

	Wstr.resize(Str.size());
	MBTOW((wchar_t*)(Wstr.c_str()), Str.c_str());
	return Wstr;
}


// TODO: This depends on std::(w)string::c_str() being contiguous.
std::string wstr2str (const std::wstring& Wstr)
{
	std::string	Str;

	Str.resize(Wstr.size());
	WTOMB((char*)(Str.c_str()), Wstr.c_str());
	return Str;
}


std::tstring wstr2tstr (const std::wstring& Wstr)
{
#ifdef _UNICODE
	return Wstr;
#else
	return wstr2str(Wstr);
#endif
}


std::wstring tstr2wstr (const std::tstring& Tstr)
{
#ifdef _UNICODE
	return Tstr;
#else
	return str2wstr(Tstr);
#endif
}


std::tstring str2tstr (const std::string& Str)
{
#ifdef _UNICODE
	return str2wstr(Str);
#else
	return Str;
#endif
}


std::string tstr2str (const std::tstring& Tstr)
{
#ifdef _UNICODE
	return wstr2str(Tstr);
#else
	return Tstr;
#endif
}




#ifdef STRINGUTILS_UNITTEST

struct UnitTestStringUtils
{
	UnitTestStringUtils () { }
	~UnitTestStringUtils () { }

	int Test ()
	{
		std::string		Str("abcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+`-=;':\"\\/,.<>");
		std::wstring	Wstr(str2wstr(Str));

		if (Str != wstr2str(Wstr))
		{
			return -1;
		}

		return 0;
	}
};

#endif


#endif


