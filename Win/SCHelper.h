// Control Services and Drivers.

#ifndef SCHELPER_H
#define SCHELPER_H

#include <windows.h>


class SCHelper
{
public:
	SCHelper (const std::string& ServiceName,
			  const std::string& ServicePath = "",
			  const std::string& ServiceDisplayName = "none") :
		m_hSCM(NULL),
		m_hServ(NULL),
		m_ServiceName(ServiceName),
		m_ServicePath(ServicePath),
		m_ServiceDisplayName(ServiceDisplayName)

	{
		try
		{
			OpenSCM();
		}
		catch (...)
		{
		}
	}


	~SCHelper ()
	{
		try
		{
			CloseServ();
			CloseSCM();
		}
		catch (...)
		{
		}
	}


	int OpenSCM ()
	{
		CloseSCM();
		m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (m_hSCM == NULL)
		{
			return -1;
		}
		return 0;
	}


	void CloseSCM ()
	{
		if (m_hSCM != NULL)
		{
			::CloseServiceHandle(m_hSCM);
			m_hSCM = NULL;
		}
	}


	int OpenServ ()
	{
		CloseServ();
		m_hServ = ::OpenService(m_hSCM,
								m_ServiceName.c_str(),
								SERVICE_ALL_ACCESS);
		if (m_hServ)
		{
			return 0;
		}
		return -1;
	}


	int CloseServ ()
	{
		if (m_hServ)
		{
			::CloseServiceHandle(m_hServ);
			m_hServ = NULL;
		}
		return 0;
	}


	int DeleteServ ()
	{
		// Assuming opened if not NULL.
		if (!m_hServ)
		{
			if (OpenServ() != 0)
			{
				std::cout << "Trying to delete a service that we're unable to open.\n";
				return -1;
			}
		}

		if (::DeleteService(m_hServ) == 0)
		{
			std::cout << "DeleteService() failed.\n";
			return -1;
		}

		return 0;
	}


	int CreateServ ()
	{

		if (OpenServ() == 0)
		{
			std::cout << "Service exists; deleting it.\n";
			if (DeleteServ() != 0)
			{
				return -1;
			}
			else
			{
				CloseServ();
			}
		}


		m_hServ = ::CreateService(m_hSCM,
								m_ServiceName.c_str(),
								m_ServiceDisplayName.c_str(),
								SERVICE_ALL_ACCESS,
								SERVICE_KERNEL_DRIVER,
								SERVICE_DEMAND_START,
								SERVICE_ERROR_NORMAL,
								m_ServicePath.c_str(),
								NULL,
								NULL,
								NULL,
								NULL,
								NULL);

		if (!m_hServ)
		{
			if (::GetLastError() == ERROR_SERVICE_EXISTS)
			{
				if (OpenServ() == -1)
				{
					std::cout << "Service exists, but couldn't open it.\n";
					return -1;
				}
			}
			else
			{
				std::cout << "CreateService error.\n";
				return -1;
			}
		}
		CloseServ();
		return 0;
	}


	int StartServ ()
	{
		SERVICE_STATUS	ServStatus;

		if (OpenServ() == -1)
		{
			std::cout << "Could not open service.\n";
			return -1;
		}

		::ControlService(m_hServ,
						 SERVICE_CONTROL_INTERROGATE,
						 &ServStatus);

		if (ServStatus.dwCurrentState != SERVICE_STOPPED)
		{
			std::cout << "Service is not in SERVICE_STOPPED state.\n";
			return -1;
		}

		BOOL rc = ::StartService(m_hServ,
								 0,
								 NULL);
		if (!rc)
		{
			std::cout << "StartService() failed: " << ::GetLastError() << "\n";
			return -1;
		}


		int		t = 0;
		while (t < 20)
		{
			::ControlService(m_hServ,
							 SERVICE_CONTROL_INTERROGATE,
							 &ServStatus);
			if (ServStatus.dwCurrentState == SERVICE_RUNNING)
			{
				std::cout << "Service started.\n";
				return 0;
			}
			::Sleep(2000);
			t += 2;
		}

		std::cout << "Service failed to start.\n";
		return -1;
	}


	int StopServ ()
	{
		SERVICE_STATUS	ServStatus;
		if (OpenServ() == -1)
		{
			std::cout << "OpenService error.\n";
			return -1;
		}

		if (!::ControlService(m_hServ, SERVICE_CONTROL_STOP, &ServStatus))
		{
			std::cout << "ControlService error. Could not stop service.\n";
			return -1;
		}

		return 0;
	}


private:
	SC_HANDLE		m_hSCM;
	SC_HANDLE		m_hServ;
	std::string		m_ServiceName;
	std::string		m_ServicePath;
	std::string		m_ServiceDisplayName;
protected:
	SCHelper () :
		m_hSCM(NULL),
		m_hServ(NULL)
	{ }		// Not usable.
};


#endif	// #ifndef SCHELPER_H
