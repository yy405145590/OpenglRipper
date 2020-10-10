// Tools.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "VertDataDecoder.h"
#include "ObjExporter.h"
#include <vector>
#include <fstream>
#include <windows.h>
#include <thread>
#include "RingBuffer.hpp"
#include "MemoryStream.h"

void OglExporter(const std::string &folder, 
	const std::string &vFormat, 
	const std::string &texFormat,
	UINT32 startIdx, UINT32 endIdx,
	bool flipXy, bool flipUv)
{
	std::string exportFolder = folder + "\\";
	if (startIdx > endIdx)
	{
		return;
	}

	for (UINT i = startIdx; i < endIdx + 1; ++i)
	{
		std::vector<Vector3d<float>> v;
		std::vector<Vector3d<float>> vn;
		std::vector<Vector2d<float>> vt;
		std::vector<unsigned int> triangles;
		std::stringstream ssV;
		ssV << i << vFormat;
		std::stringstream ssT;
		ssT << i << texFormat;
		std::stringstream ssI;
		ssI << i << ".indx";
		std::stringstream ssTexInfo;
		ssTexInfo << i << ".texinfo.txt";
		VertDataDecoder::VertDecode(exportFolder + ssV.str(),
			exportFolder + ssI.str(), exportFolder + ssT.str(), flipUv, flipXy, v, vn, vt, triangles);
		std::ifstream textInfoStream;
		textInfoStream.open(exportFolder + ssTexInfo.str(), std::fstream::binary);
		std::vector<std::string> texFiles;
		if (textInfoStream)
		{
			std::string filename;
			while (getline(textInfoStream, filename))
			{
				texFiles.push_back(filename);
			}
		}
		ObjExporter exporter(i);
		exporter.SetMat(texFiles, exportFolder);
		exporter.SetVert(v, vn, vt, triangles);
		std::stringstream ssTmp;
		ssTmp << i << ".obj";
		exporter.ExportToFile(exportFolder + ssTmp.str());
	}
	
}
VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbBytesRead,
	LPOVERLAPPED lpOverLap)
{

}

VOID WINAPI CompletedCheckWriteRoutine(DWORD dwErr, DWORD cbBytesRead,
	LPOVERLAPPED lpOverLap)
{

}
#define PIPE_NAME "\\\\.\\pipe\\TestOgl"
void TestPipeServer()
{
	HANDLE pipeServer = CreateNamedPipe(PIPE_NAME,
		PIPE_ACCESS_DUPLEX |     // read/write access 
		FILE_FLAG_OVERLAPPED,    // overlapped mode 
		PIPE_TYPE_MESSAGE |      // message-type pipe 
		PIPE_READMODE_MESSAGE |  // message-read mode 
		PIPE_WAIT,               // blocking mode 
		11,               // number of instances 
		1024 * sizeof(TCHAR),   // output buffer size 
		1024 * sizeof(TCHAR),   // input buffer size 
		50,            // client time-out 
		NULL);                   // default security attributes 
	OVERLAPPED oConnect;
	HANDLE hConnectEvent = CreateEvent(
		NULL,    // default security attribute
		TRUE,    // manual reset event 
		TRUE,    // initial state = signaled 
		NULL);   // unnamed event object 
	oConnect.hEvent = hConnectEvent;
	BOOL b = ConnectNamedPipe(pipeServer, &oConnect);
	BOOL fPendingIO = false;
	OVERLAPPED* lpPipeInst = (OVERLAPPED*)GlobalAlloc(
		GPTR, sizeof(OVERLAPPED));
	switch (GetLastError())
	{
		// The overlapped connection in progress. 
	case ERROR_IO_PENDING:
		fPendingIO = TRUE;
		break;

		// Client is already connected, so signal an event. 

	case ERROR_PIPE_CONNECTED:
		if (SetEvent(hConnectEvent))
			break;

		// If an error occurs during the connect operation... 
	default:
	{
		printf("ConnectNamedPipe failed with %d.\n", GetLastError());
	}
	}
	char buf[100];
	DWORD cbRead = 0;
	//ReadFile(pipeServer, buf, sizeof(buf), &cbRead, NULL);
	BOOL fRead = FALSE;

	// lpOverlap points to storage for this instance. 
	// The write operation has finished, so read the next request (if 
	// there is no error). 
	DWORD dwWait;
	DWORD cbRet;
	BOOL fSuccess;
	while (true)
	{
		dwWait = WaitForSingleObjectEx(
			hConnectEvent,  // event object to wait for 
			INFINITE,       // waits indefinitely 
			TRUE);          // alertable wait enabled 
		if (fPendingIO)
		{
			fSuccess = GetOverlappedResult(
				pipeServer,     // pipe handle 
				&oConnect, // OVERLAPPED structure 
				&cbRet,    // bytes transferred 
				FALSE);    // does not wait 
			if (!fSuccess)
			{
				printf("ConnectNamedPipe (%d)\n", GetLastError());
			}
		}
		
		fRead = ReadFileEx(
			pipeServer,
			buf,
			sizeof(buf),
			lpPipeInst,
			(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedReadRoutine);
	//	fRead = WriteFileEx(pipeServer, NULL, 0, lpPipeInst, CompletedCheckWriteRoutine);
		uint32 nRett = GetLastError();
		printf("%d", nRett);
	}
	
		
	printf(buf);
}

void TestPipeClient()
{
	HANDLE pipeClient = NULL;
	while (1)
	{
		pipeClient = CreateFile(
			PIPE_NAME,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

							// Break if the pipe handle is valid. 

		if (pipeClient != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			return;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(PIPE_NAME, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return;
		}
	}
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL fSuccess = SetNamedPipeHandleState(
		pipeClient,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 

	DWORD cbWriten = 0;
	WriteFile(pipeClient, "Hello", sizeof("Hello"), &cbWriten, NULL);
}

void TestPipe()
{
	std::thread serverThread(TestPipeServer);
	std::thread clientThread(TestPipeClient);

	serverThread.join();
	clientThread.join();
}
#undef max
#include <limits>  
void TestRingBuffer()
{
	Ringbuffer<char, 8> testBuf;
	for (int i = 0; i < 10; ++i)
	{
		if (!testBuf.insert(i))
		{
			printf("%d", i);
			break;
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		char b;
		if (!testBuf.remove(b))
		{
			printf("%d", i);
			break;
		}
	}

	//Ringbuffer<char, 8> testBuf;
	for (int i = 0; i < 10; ++i)
	{
		if (!testBuf.insert(i))
		{
			printf("%d", i);
			break;
		}
	}

}

void TestMemoryStream()
{
	MemoryStream<8> ms;
	for (int i = 0; i < 10; ++i)
	{
		ms << i;
	}
	std::string ss = "hello ms123123123";
	ms << ss;

	for (int i = 0; i < 10; ++i)
	{
		int b;
		ms >> b;
		printf("%d\n", b);
	}
	std::string s;
	ms >> s;
	printf("%s\n", s.c_str());
}

void TestAtomic()
{
	std::atomic<long> a = 123;
	a.store(234);
	long b = a.load();
}


template <typename T>
void TestRightRef1(const T &&a)
{

}


