#include <Core/Services/LogService.h>
#include <ctime>
#include <chrono>
#include <limits.h>
#ifdef __ANDROID__
	#include <android/log.h>
	#define DEBUG 1
	#define LOG_TAG "GLES3JNI"
	#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
	#if DEBUG
	#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
	#else
	#define ALOGV(...)
	#endif
#endif


std::vector<LogFile> files;



//#define NO_LOG_FILE
bool CreateNewLogFile(std::string name) {
	for (auto& lf : files) {
		if (lf.filename == name) {
			return false; 
		}
	}
	LogFile lf;
	lf.filename = name;
#ifdef __ANDROID__
    lf.path = "/sdcard/" + name + ".txt";
#else
	lf.path = name + ".txt";
#endif
	lf.stream = new std::fstream(lf.path, std::fstream::out);
	files.push_back(lf);
	return true;
}
void CloseAll() {
	for (auto& lf : files) {
		lf.stream->close();
	}
	files.clear();
}
void WriteToLog(std::string data, std::string name, bool create) {
	#ifdef LOG_CONSOLE
		std::cout << data << std::endl;
	#endif
	#ifdef __ANDROID__
	#ifdef DEBUG
		auto c = data.c_str();
		__android_log_print(ANDROID_LOG_VERBOSE, "LogService", "%s", c);
	#else
		ALOGE(data);
	#endif
	#endif
	#ifndef NO_LOG_FILE
		bool found = false;
		for (auto& lf : files) {
			if (lf.filename == name) {
				*lf.stream << data << std::endl;
				lf.stream->flush();
				found = true;
				break;
			}
		}
		if (create && !found) {
			CreateNewLogFile(name);
			WriteToLog(data, name);
		}
	#endif
}
void loglog(std::string data, std::string name, LogType t, std::string caller, std::string fl, int ln, bool tr) {
	std::string d;
	// Get Time
	auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	d += "[";
	d += std::ctime(&tt);
	d.erase(d.end() - 1);
	//Sub filename only
	size_t win = fl.find_last_of('\\');
	size_t oth = fl.find_last_of('/');
	if (win < ULONG_MAX) fl = fl.substr(win + 1);
	else if (oth < ULONG_MAX) fl = fl.substr(oth + 1);


	d += ", " + fl + "@" + std::to_string(ln) + ", ";
	switch (t)
	{
	case LogType::Info:
		d += "Info";
		break;
	case LogType::Error:
		d += "Error";
		break;
	case LogType::Warning:
		d += "Warning";
		break;
	case LogType::Debug:
		d += "Debug";
		break;
	}
	d += "] ";
	d += data;
	WriteToLog(d, name, true);
}