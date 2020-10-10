#ifndef __LOG__
#define __LOG__
#include <string>

class ConsoleLog
{
public:
	static ConsoleLog *GetInstance()
	{
		if (sInstance == NULL)
		{
			sInstance = new ConsoleLog();
			
		}
		return sInstance;
	}
	ConsoleLog();
	void AllocLogConsole();
	void Init();
	void DebugWrite(char *format, ...);
	void Write(char *format, ...);
	void WriteConsole(char *format, ...);
	std::string PopInput();
	void PushInputChar(std::string input);
	void CreateInputThread();
	
private:
	static void InputThread();
	static ConsoleLog *sInstance;
	HANDLE hStdin;  
	HANDLE hStdout;
	std::string m_input;
	CRITICAL_SECTION m_inputCs;
};

#define sConsoleLog ConsoleLog::GetInstance()
#endif