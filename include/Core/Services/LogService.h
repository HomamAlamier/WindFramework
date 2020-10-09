#ifndef LOGSERVICE_H
	#define LOGSERVICE_H


	#define LOG_CONSOLE

	#include <iostream>
	#include <fstream>
	#include <vector>
	#include <string>
	enum class LogType {
		Info, Error, Warning, Debug
	};
	struct LogFile{
	public:
		std::string filename;
		std::fstream* stream;
		std::string path;
	};
	bool CreateNewLogFile(std::string name);
	void CloseAll();
	void WriteToLog(std::string data, std::string name, bool create = false);
	std::string MethodH(std::string m, std::string fl, int ln);
	void loglog(std::string data, std::string name, LogType t,
		std::string caller, std::string fl, int ln, bool tr);
	void Log_WriteLine(std::string, std::string, LogType, bool);
	void Log_WriteLine(std::string, LogType, bool);
	#undef Log_WriteLine
	#ifdef LOG_FN
		#define Log_WriteLine( str, type, tr ) loglog( str, LOG_FN, type, __FUNCTION__, __FILE__, __LINE__, tr)
	#else
		#define Log_WriteLine( str, name, type, tr ) loglog( str, name, type, __FUNCTION__, __FILE__, __LINE__, tr)
	#endif // LOG_FN
#endif // !LOGSERVICE_H
