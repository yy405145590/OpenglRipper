#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include "Log.h"
#include "conio.h"
#include <iostream>

ConsoleLog *ConsoleLog::sInstance = NULL;
ConsoleLog::ConsoleLog()
{
	Init();
}

char g_log_file[512] = ".\\ConSole.txt";
void ConsoleLog::DebugWrite(char *format, ...)
{
//#ifdef _DEBUG
	va_list args;
	va_start(args, format);
	Write(format, args);
	va_end(args);
//#endif	
}

void ConsoleLog::Write(char *format, ...)
{
	char printBuf[2048];
	va_list args;
	va_start(args, format);
	vsprintf_s<sizeof(printBuf)>(printBuf, format, args);
	va_end(args);
	DWORD length = (DWORD)strlen(printBuf);
	WriteFile(hStdout, printBuf, length, &length, 0);
	length = (DWORD)strlen("\n");
 	//WriteFile(hStdout, "\n", length, &length, 0);
	sClientMsg->PushLog(printBuf);
}

void ConsoleLog::WriteConsole(char *format, ...)
{
	char printBuf[2048];
	va_list args;
	va_start(args, format);
	vsprintf_s<sizeof(printBuf)>(printBuf, format, args);
	va_end(args);
	DWORD length = (DWORD)strlen(printBuf);
	WriteFile(hStdout, printBuf, length, &length, 0);
	length = (DWORD)strlen("\n");
	WriteFile(hStdout, "\n", length, &length, 0);
}
std::string ConsoleLog::PopInput()
{
	EnterCriticalSection(&m_inputCs);	
	std::string tmp = m_input;
	m_input = "";
	LeaveCriticalSection(&m_inputCs);
	return tmp;
}

void ConsoleLog::PushInputChar(std::string input)
{
	//D3DConSole::GetInstance()->Write("push input %s", input.c_str());
	EnterCriticalSection(&m_inputCs);	
	m_input = input;
	LeaveCriticalSection(&m_inputCs);
}

void ConsoleLog::AllocLogConsole()
{
	AllocConsole();
	hStdin  = GetStdHandle(STD_INPUT_HANDLE);  
	hStdout =GetStdHandle(STD_OUTPUT_HANDLE); 
	FILE *f;
	freopen_s(&f, "conin$","r+t",stdin);
}

void ConsoleLog::Init()
{
	AllocLogConsole();
	CreateInputThread();
}

void ConsoleLog::InputThread()
{
	while (true)
	{
		char tmp[50];
		std::cin.getline(tmp, 50, '\n');
		ConsoleLog::GetInstance()->PushInputChar(tmp);
	}
}

void ConsoleLog::CreateInputThread()
{
	InitializeCriticalSection(&m_inputCs);

	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&InputThread,NULL,0,NULL);
}