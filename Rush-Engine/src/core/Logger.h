#ifndef __RUSH_LOGGER_H__
#define __RUSH_LOGGER_H__

#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_map>

#include "Core.h"

namespace Rush {

enum RUSH_API LogLevel{
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

struct LogMessage{
    unsigned long timestamp;
    LogLevel level;
    std::string sender;
    std::string message;
};

class RUSH_API Logger{
    private:
        
        static bool s_close;
        std::chrono::system_clock::time_point m_LoggerStart;
        std::queue<LogMessage> m_LogQueue;
        std::condition_variable m_Logged;
        std::thread m_LoggerThread;
        std::mutex m_QueueLock;
        std::unordered_map<std::thread::id,std::string> m_AliasMap;
        LogLevel m_Threshold;

        Logger();
        void Log(std::string message, std::string sender, LogLevel level);
        static void LogPump();

    public:
        Logger(const Logger&) = delete;

        static RUSH_API void Init();
        static RUSH_API void Destroy();

        static RUSH_API Logger& getInstance(){
            static Logger s_Instance;
            return s_Instance;
        }
        
        static RUSH_API void Trace(std::string message, std::string sender = "");
        static RUSH_API void Debug(std::string message, std::string sender = "");
        static RUSH_API void Info(std::string message, std::string sender = "");
        static RUSH_API void Warning(std::string message, std::string sender = "");
        static RUSH_API void Error(std::string message, std::string sender = "");

        static RUSH_API void SetThreshold(LogLevel level);

        static RUSH_API void SetAlias(std::string alias, std::thread::id id = std::thread::id());
};
}

#endif  // __RUSH_LOGGER_H__