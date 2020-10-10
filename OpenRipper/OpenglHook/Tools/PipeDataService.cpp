#include "stdafx.h"
#include "PipeDataService.h"
#define PIPE_NAME "\\\\.\\pipe\\OglRipper"
// VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbBytesRead,
// 	LPOVERLAPPED lpOverLap)
// {
// 
// }
// 
// VOID WINAPI CompletedCheckWriteRoutine(DWORD dwErr, DWORD cbBytesRead,
// 	LPOVERLAPPED lpOverLap)
// {
// 
// }
Session::Session()
{
	m_pWriteOverlap = new IoOverlap(IO_WRITE, this);
	m_pWriteOverlap = new IoOverlap(IO_READ, this);
}

PipeDataService::PipeDataService()
{
}


PipeDataService::~PipeDataService()
{
}
// CreateAndConnectInstance(LPOVERLAPPED) 
// This function creates a pipe instance and connects to the client. 
// It returns TRUE if the connect operation is pending, and FALSE if 
// the connection has been completed. 

BOOL PipeDataService::CreateAndConnectInstance(LPOVERLAPPED lpoOverlap, HANDLE &hPipe)
{
	hPipe = CreateNamedPipe(
		PIPE_NAME,					// pipe name 
		PIPE_ACCESS_DUPLEX |		// read/write access 
		FILE_FLAG_OVERLAPPED,		// overlapped mode 
		PIPE_TYPE_MESSAGE |			// message-type pipe 
		PIPE_READMODE_MESSAGE |		// message read mode 
		PIPE_WAIT,					// blocking mode 
		PIPE_UNLIMITED_INSTANCES,	// unlimited instances 
		4096,						// output buffer size 
		4096,						// input buffer size 
		5000,						// client time-out 
		NULL);						// default security attributes
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		printf("CreateNamedPipe failed with %d.\n", GetLastError());
		return 0;
	}

	// Call a subroutine to connect to the new client. 

	return ConnectToNewClient(hPipe, lpoOverlap);
}

BOOL PipeDataService::ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
	BOOL fConnected, fPendingIO = FALSE;

	// Start an overlapped connection for this pipe instance. 
	fConnected = ConnectNamedPipe(hPipe, lpo);

	// Overlapped ConnectNamedPipe should return zero. 
	if (fConnected)
	{
		printf("ConnectNamedPipe failed with %d.\n", GetLastError());
		return 0;
	}

	switch (GetLastError())
	{
		// The overlapped connection in progress. 
	case ERROR_IO_PENDING:
		fPendingIO = TRUE;
		break;

		// Client is already connected, so signal an event. 

	case ERROR_PIPE_CONNECTED:
		if (SetEvent(lpo->hEvent))
			break;

		// If an error occurs during the connect operation... 
	default:
	{
		printf("ConnectNamedPipe failed with %d.\n", GetLastError());
		return 0;
	}
	}
	return fPendingIO;
}
void PipeDataService::ServerLoop()
{
	HANDLE hConnectEvent;
	OVERLAPPED oConnect;
	DWORD dwWait, cbRet;
	BOOL fSuccess, fPendingIO;

	// Create one event object for the connect operation. 

	hConnectEvent = CreateEvent(
		NULL,    // default security attribute
		TRUE,    // manual reset event 
		TRUE,    // initial state = signaled 
		NULL);   // unnamed event object 

	if (hConnectEvent == NULL)
	{
		printf("CreateEvent failed with %d.\n", GetLastError());
	}

	oConnect.hEvent = hConnectEvent;

	// Call a subroutine to create one instance, and wait for 
	// the client to connect. 
	HANDLE hPipe;
	fPendingIO = CreateAndConnectInstance(&oConnect, hPipe);

	while (1)
	{
		// Wait for a client to connect, or for a read or write 
		// operation to be completed, which causes a completion 
		// routine to be queued for execution. 

		dwWait = WaitForSingleObjectEx(
			hConnectEvent,  // event object to wait for 
			INFINITE,       // waits indefinitely 
			TRUE);          // alertable wait enabled 

		switch (dwWait)
		{
				// The wait conditions are satisfied by a completed connect 
				// operation. 
			case 0:
				// If an operation is pending, get the result of the 
				// connect operation. 

				if (fPendingIO)
				{
					fSuccess = GetOverlappedResult(
						hPipe,     // pipe handle 
						&oConnect, // OVERLAPPED structure 
						&cbRet,    // bytes transferred 
						FALSE);    // does not wait 
					if (!fSuccess)
					{
						printf("ConnectNamedPipe (%d)\n", GetLastError());
					}
					else
					{

					}
				}

				// Allocate storage for this instance. 

				OnClientConnect(hPipe);
				// Create new pipe instance for the next client. 

				fPendingIO = CreateAndConnectInstance(
					&oConnect, hPipe);
				break;

				// The wait is satisfied by a completed read or write 
				// operation. This allows the system to execute the 
				// completion routine. 

			case WAIT_IO_COMPLETION:
				break;

				// An error occurred in the wait function. 

			default:
			{
				printf("WaitForSingleObjectEx (%d)\n", GetLastError());
				return;
			}
		}
	}
}

void PipeDataService::OnClientConnect(HANDLE hPipe)
{

}
