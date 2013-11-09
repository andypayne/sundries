#ifndef CREGKEYBOLTON_H
#define CREGKEYBOLTON_H


#pragma warning(disable: 4786)
#include <iostream>
#include <string>
#include <vector>

#include <windows.h>
#include <atlbase.h>



class CRegKeyBoltOn : public CRegKey
{
public:
	// Just in case.
	~CRegKeyBoltOn () { Close(); }


	LONG EnumKey (DWORD iIndex,
				  LPTSTR pszName,
				  LPDWORD pnNameLength,
				  FILETIME* pftLastWriteTime = NULL)
	{
		return ::RegEnumKeyEx(m_hKey, iIndex, pszName, pnNameLength, 0, NULL, NULL, pftLastWriteTime);
	}


	LONG QueryStringValue (LPCTSTR pszValueName,
						   LPBYTE pszValue,
						   ULONG* pnChars)
	{
		return ::RegQueryValueEx(m_hKey, (LPTSTR)pszValueName, NULL, NULL, pszValue, pnChars);
	}


	// Handle std::wstring
	LONG QueryStringValue (LPCTSTR pszValueName,
						   std::wstring & StrValue)
	{
		DWORD					ValueSize = 0;
		std::vector<wchar_t>	TmpValue;
		LONG					ret = -1;

		QueryStringValue(pszValueName,
						 NULL,
						 &ValueSize);

		TmpValue.resize(ValueSize/sizeof(wchar_t));

		// I'm not sure if the cast is kosher.
		if ( (ret = QueryStringValue(pszValueName,
									 reinterpret_cast<BYTE *>(&TmpValue[0]),
									 &ValueSize)) != ERROR_SUCCESS)
		{
			return ret;
		}

		StrValue.assign(TmpValue.begin(), TmpValue.begin() + ValueSize);

		return ret;
	}


	LONG QueryDWORDValue (LPCTSTR pszValueName,
						  DWORD* pdwValue)
	{
		DWORD	dwType = -1;
		DWORD	cValue = sizeof(DWORD);
		LONG ret = ::RegQueryValueEx(m_hKey,
									 (LPTSTR)pszValueName,
									 NULL, &dwType,
									 reinterpret_cast<BYTE *>(pdwValue),
									 &cValue);
		return (dwType == REG_DWORD) ? ret : -1;
	}


	// Fill the vector with the list of subkeys.
	int EnumKey (std::vector<std::wstring> & NameList,
				 const DWORD NameLength)
	{
		std::vector<wchar_t>	NameString;
		DWORD					NameStringLength(NameLength);
		bool					EnumFinished = false;
		int						i = 0;


		while (EnumFinished == false)
		{
			NameStringLength = NameLength;
			NameString.resize(NameStringLength);
			if (EnumKey(i, &NameString[0], &NameStringLength) == ERROR_NO_MORE_ITEMS)
			{
				EnumFinished = true;
			} else {
				NameList.push_back(std::wstring(NameString.begin(), NameString.begin() + NameStringLength));
				NameString.clear();
				++i;
			}
		}
		return ERROR_SUCCESS;
	}


	// Fill the vector with the list of values.
	int EnumStringValues (std::vector<std::pair<std::wstring, std::wstring> > & ValueList)
	{
		std::vector<wchar_t>	NameString;
		std::vector<wchar_t>	ValueString;
		DWORD					MaxNameLength = 0;
		DWORD					MaxValueLength = 0;
		DWORD					NameLength = 0;
		DWORD					ValueLength = 0;
		bool					EnumFinished = false;
		int						i = 0;
		LONG					ret = 0;

		if ( (ret = ::RegQueryInfoKey(m_hKey,
									NULL,
									NULL,
									0,
									NULL,
									NULL,
									NULL,
									NULL,
									&MaxNameLength,
									&MaxValueLength,
									NULL,
									NULL)) != ERROR_SUCCESS)
		{
			return ret;
		}

		NameString.clear();
		ValueString.clear();

		while (EnumFinished == false)
		{

			NameLength = MaxNameLength + 2;
			ValueLength = MaxValueLength + 2;
			NameString.resize(NameLength);						// In TCHARs
			ValueString.resize((ValueLength)/sizeof(wchar_t));	// In BYTEs

			if ((ret = ::RegEnumValue(m_hKey,
									  i,
									  &NameString[0],
									  &NameLength,
									  0,
									  NULL,
									  reinterpret_cast<unsigned char *>(&ValueString[0]),
									  &ValueLength)) == ERROR_NO_MORE_ITEMS)
			{
				EnumFinished = true;
			} else {

				ValueList.push_back(std::pair<std::wstring, std::wstring>(
					std::wstring(NameString.begin(), NameString.begin() + NameLength),	// NameLength does not include a final NULL.
					std::wstring(ValueString.begin(), ValueString.begin() + ValueLength/sizeof(wchar_t) - 1))); // ValueLength includes final NULL that we don't need.

				++i;
			}
		}
		return ERROR_SUCCESS;
	}


};


#endif	// #ifndef CREGKEYBOLTON_H
