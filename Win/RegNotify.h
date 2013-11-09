
#ifndef REGNOTIFY_H
#define REGNOTIFY_H


#include <iostream>
#include <string>

#include <windows.h>




class RegNotifier
{
public:
	RegNotifier () :
		m_hEvent(NULL),
		m_hBaseKey(NULL),
		m_hKey(NULL)
	{
	}


	~RegNotifier ()
	{
		Stop();
	}


	int Start (HKEY BaseKey,
			   const std::wstring& KeyStr)
	{
		Stop();

		m_hBaseKey = BaseKey;
		m_KeyStr = KeyStr;

		if (::RegOpenKeyEx(m_hBaseKey,
				m_KeyStr.c_str(),
				0,
				KEY_NOTIFY,
				&m_hKey) != ERROR_SUCCESS)
		{
			return -1;
		}

		m_hEvent = ::CreateEvent(NULL,
						TRUE,
						FALSE,
						NULL);

		if (m_hEvent == NULL)
		{
			return -1;
		}

		if (::RegNotifyChangeKeyValue(m_hKey,
				TRUE,
				REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_LAST_SET,
				m_hEvent,
				TRUE) != ERROR_SUCCESS)
		{
			return -1;
		}

		return 0;
	}


	void Stop ()
	{
		if (m_hEvent)
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL;
		}

		if (m_hKey)
		{
			::RegCloseKey(m_hKey);
			m_hKey = NULL;
		}
	}


	HANDLE		GetEvent () const
	{
		return m_hEvent;
	}


private:
	HANDLE			m_hEvent;
	HKEY			m_hBaseKey;
	HKEY			m_hKey;
	std::wstring	m_KeyStr;
};



#endif	// #ifndef REGNOTIFY_H

