#pragma once

#include <string>
#include <sstream>
#include <queue>
#include <mutex>
#include <thread>

enum Log_Severity {
    ERROR,
    WARN,
    INFO,
    DEBUG
};

struct LogData {
    char* msg;
    Log_Severity severity;
    char* timestamp;
    char* location;
};

namespace LoggerInfo {
    extern std::queue<LogData*> queue;
    extern std::mutex queueMutex;
    extern std::condition_variable queueSignal;
}

class Logger {
private:
    std::mutex killSignal;
    std::thread worker;

    void workerThread(std::mutex* killSig);
    void writeLogToFiles(LogData* logData);

public:
	Logger();
	~Logger();

    static void strCopy(char* dest, std::string* source);
    static void log(std::string loc, Log_Severity severity, std::string msg);
};
