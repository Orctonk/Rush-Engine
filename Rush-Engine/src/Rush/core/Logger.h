#ifndef __RUSH_LOGGER_H__
#define __RUSH_LOGGER_H__

#include "Core.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

namespace Rush {

enum class RUSH_API LogLevel{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4
};

struct LogMessage{
    unsigned long timestamp;
    LogLevel level;
    std::string sender;
    std::string message;
};

enum ColorValue {
    BLACK = 30,
    WHITE = 97,
    RED = 91,
    GREEN  = 92,
    YELLOW = 93,
    BLUE = 94
};

struct RUSH_API Color {
    ColorValue col;

    Color(ColorValue color) { col = color; }

    friend std::ostream;
};

std::ostream& operator<<(std::ostream &os,Color col);

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

        static void Init();
        static void Destroy();

        static Logger& getInstance(){
            static Logger s_Instance;
            return s_Instance;
        }
        
        static void Trace(std::string message, std::string sender = "");
        static void Debug(std::string message, std::string sender = "");
        static void Info(std::string message, std::string sender = "");
        static void Warning(std::string message, std::string sender = "");
        static void Error(std::string message, std::string sender = "");

        static void SetThreshold(LogLevel level);

        static void SetAlias(std::string alias, std::thread::id id = std::thread::id());
};
}

#define RUSH_LOG_TRACE(x) Rush::Logger::getInstance().Trace(x)
#define RUSH_LOG_DEBUG(x) Rush::Logger::getInstance().Debug(x)
#define RUSH_LOG_INFO(x) Rush::Logger::getInstance().Info(x)
#define RUSH_LOG_WARNING(x) Rush::Logger::getInstance().Warning(x)
#define RUSH_LOG_ERROR(x) Rush::Logger::getInstance().Error(x)

#endif  // __RUSH_LOGGER_H__