#include "Logger.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <experimental/filesystem>

#define LOGS_FOLDER "logs"
#define LOG_FILE_ERROR "logs/error.log"
#define LOG_FILE_WARN "logs/warn.log"
#define LOG_FILE_INFO "logs/info.log"
#define LOG_FILE_DEBUG "logs/debug.log"

namespace LoggerInfo {
    std::queue<LogData*> queue;
    std::mutex queueMutex;
    std::condition_variable queueSignal;
}

void Logger::strCopy(char* dest, std::string* source) {
    char* d = dest;
    for (char c: (*source)) {
        (*d) = c;
        d = &d[1];
    }
    (*d) = '\0';
}

void Logger::log(std::string loc, Log_Severity severity, std::string msg) {
    LogData* logDataOnHeap = (LogData*)malloc(sizeof(LogData));
    logDataOnHeap->severity = severity;

    logDataOnHeap->msg = (char*)malloc(msg.length() + 1);
    strCopy(logDataOnHeap->msg, &msg);

    logDataOnHeap->location = (char*)malloc(loc.length() + 1);
    strCopy(logDataOnHeap->location, &loc);

    // Get timestamp
    std::time_t time = std::time(nullptr);
    std::tm* tm = std::localtime(&time);
    std::ostringstream ts;
    ts << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec;
    logDataOnHeap->timestamp = (char*)malloc(ts.str().length() + 1);
    strCopy(logDataOnHeap->timestamp, &ts.str());

    LoggerInfo::queueMutex.lock();
    LoggerInfo::queue.emplace(logDataOnHeap);
    LoggerInfo::queueMutex.unlock();

    LoggerInfo::queueSignal.notify_one();
}

Logger::Logger() {
    using namespace std::experimental::filesystem;
    // Remove log files and recreate it
    remove_all(LOGS_FOLDER);
    auto created_new_directory = create_directory(LOGS_FOLDER);
    if (!created_new_directory) {
        std::cout << "Logger failed to create logs folder again" << std::endl;
    }

    // When this killSignal is unlocked, the worker threads will exit
    killSignal.lock();

    // Create worker threads
    worker = std::thread(&Logger::workerThread, this, &killSignal);
    worker.detach();
}

Logger::~Logger() {
    std::cout << "Killing logger" << std::endl;
    killSignal.unlock();
    LoggerInfo::queueSignal.notify_all();
    worker.join();

    while (!LoggerInfo::queue.empty()) {
        LogData* logData = LoggerInfo::queue.front();
        LoggerInfo::queue.pop();

        std::cout << logData->timestamp << " | " << logData->severity << " | " << logData->location << " | " << logData->msg << std::endl;

        free(logData->msg);
        free(logData->timestamp);
        free(logData->location);
        free(logData);
    }
}

void Logger::workerThread(std::mutex* killSig) {
    while (!killSig->try_lock()) {
        std::unique_lock<std::mutex> lk(LoggerInfo::queueMutex);
        LoggerInfo::queueSignal.wait(lk);
        // It has been awoken

        while (!LoggerInfo::queue.empty()) {
            LogData* logData = LoggerInfo::queue.front();
            LoggerInfo::queue.pop();

            writeLogToFiles(logData);

            free(logData->msg);
            free(logData->timestamp);
            free(logData->location);
            free(logData);
        }

        lk.unlock();
    }
}

void Logger::writeLogToFiles(LogData* logData) {
    std::string severity;
    switch (logData->severity) {
    case ERROR:
        severity = "ERROR";
        break;
    case WARN:
        severity = "WARN";
        break;
    case INFO:
        severity = "INFO";
        break;
    case DEBUG:
        severity = "DEBUG";
        break;
    default:
        severity = "UNKNOWN";
        break;
    }

    std::string msg = "";
    msg.append(logData->timestamp);
    msg.append(" [");
    msg.append(severity);
    msg.append("] ");
    msg.append(logData->location);
    msg.append(" | ");
    msg.append(logData->msg);

    std::ofstream file;
    switch (logData->severity) {
        case ERROR:
            file.open(LOG_FILE_ERROR, std::ios::out | std::ios::app);
            file << msg << std::endl;
            file.close();
        case WARN:
            file.open(LOG_FILE_WARN, std::ios::out | std::ios::app);
            file << msg << std::endl;
            file.close();
        case INFO:
            file.open(LOG_FILE_INFO, std::ios::out | std::ios::app);
            file << msg << std::endl;
            file.close();
        case DEBUG:
            file.open(LOG_FILE_DEBUG, std::ios::out | std::ios::app);
            file << msg << std::endl;
            file.close();
            break;
        default:
            std::cout << "LOGGER: Unknown severity level for log: \"" << msg << "\"" << std::endl;
            break;
    }
}
