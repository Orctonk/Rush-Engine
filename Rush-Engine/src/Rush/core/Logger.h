#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "Core.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <fmt/core.h>

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
        template<typename... Args>
        void Log(std::string message, std::string sender, LogLevel level, Args ... args);
        static void LogPump();

    public:
        Logger(const Logger&) = delete;

        static void Init();
        static void Destroy();

        static Logger& getInstance(){
            static Logger s_Instance;
            return s_Instance;
        }
        
        template<typename... Args>
        static void Trace(std::string message, std::string sender = "", Args... args);
        template<typename... Args>
        static void Debug(std::string message, std::string sender = "", Args... args);
        template<typename... Args>
        static void Info(std::string message, std::string sender = "", Args... args);
        template<typename... Args>
        static void Warning(std::string message, std::string sender = "", Args... args);
        template<typename... Args>
        static void Error(std::string message, std::string sender = "", Args... args);

        static void SetThreshold(LogLevel level);

        static void SetAlias(std::string alias, std::thread::id id = std::thread::id());
};

template<typename... Args>
void Logger::Trace(std::string message, std::string sender, Args ... args){
    getInstance().Log(message,sender, LogLevel::Trace, args...);
}

template<typename... Args>
void Logger::Debug(std::string message, std::string sender, Args ... args){
    getInstance().Log(message,sender, LogLevel::Debug, args...);
}

template<typename... Args>
void Logger::Info(std::string message, std::string sender, Args ... args){
    getInstance().Log(message,sender, LogLevel::Info, args...);
}

template<typename... Args>
void Logger::Warning(std::string message, std::string sender, Args ... args){
    getInstance().Log(message,sender, LogLevel::Warning, args...);
}

template<typename... Args>
void Logger::Error(std::string message, std::string sender, Args ... args){
    getInstance().Log(message,sender, LogLevel::Error, args...);
}

template<typename... Args>
void Logger::Log(std::string message, std::string sender, LogLevel level, Args... args){
    using namespace std::chrono;
    if(level < m_Threshold)
        return;

    LogMessage m;
    m.level = level;
    m.message = fmt::format(message,args...);
    std::stringstream ss;
    if(sender.empty()){
        auto id = std::this_thread::get_id();
        try{
            ss << m_AliasMap.at(id);
        } catch (std::out_of_range e){
            ss << id;
        }
    }
    else
        ss << sender;
    m.sender = ss.str();
    std::unique_lock<std::mutex> guard(m_QueueLock);
    m.timestamp = duration_cast<milliseconds>(system_clock::now() - m_LoggerStart).count();
    m_LogQueue.push(m);
    m_Logged.notify_all();
}

}

#define RUSH_LOG_TRACE(x, ...) Rush::Logger::getInstance().Trace(x,"", __VA_ARGS__)
#ifdef RUSH_DEBUG
    #define RUSH_LOG_DEBUG(x, ...) Rush::Logger::getInstance().Debug(x,"", __VA_ARGS__)
#else
    #define RUSH_LOG_DEBUG(x, ...) do {} while()
#endif
#define RUSH_LOG_INFO(x, ...) Rush::Logger::getInstance().Info(x,"", __VA_ARGS__)
#define RUSH_LOG_WARNING(x, ...) Rush::Logger::getInstance().Warning(x,"", __VA_ARGS__)
#define RUSH_LOG_ERROR(x, ...) Rush::Logger::getInstance().Error(x,"", __VA_ARGS__)

#endif // __LOGGER_H__
