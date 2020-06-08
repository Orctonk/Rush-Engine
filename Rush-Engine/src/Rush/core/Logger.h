#ifndef __RUSH_LOGGER_H__
#define __RUSH_LOGGER_H__

#include "Rushpch.h"

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

#define RUSH_LOG_TRACE(x) Rush::Logger::getInstance().Trace(x)
#define RUSH_LOG_DEBUG(x) Rush::Logger::getInstance().Debug(x)
#define RUSH_LOG_INFO(x) Rush::Logger::getInstance().Info(x)
#define RUSH_LOG_WARNING(x) Rush::Logger::getInstance().Warning(x)
#define RUSH_LOG_ERROR(x) Rush::Logger::getInstance().Error(x)

#endif  // __RUSH_LOGGER_H__