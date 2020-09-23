#include "Rushpch.h"
#include "Logger.h"

#include <iostream>
#include <iomanip>

namespace Rush {

bool Logger::s_close = false;

Logger::Logger()
:   m_LogQueue(), m_Threshold(INFO), m_QueueLock(), m_AliasMap(){
    m_LoggerStart = std::chrono::system_clock::now();
    s_close = false;
    m_LoggerThread = std::thread(LogPump);
    Log("Logger started!","Logger",INFO);
}

void Logger::Log(std::string message, std::string sender, LogLevel level){
    using namespace std::chrono;
    if(level < m_Threshold)
        return;

    LogMessage m;
    m.level = level;
    m.message = message;
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
    m_QueueLock.lock();
    m.timestamp = duration_cast<milliseconds>(system_clock::now() - m_LoggerStart).count();
    m_LogQueue.push(m);
    m_Logged.notify_all();
    m_QueueLock.unlock();
}

std::string getLevelString(LogLevel level){
    switch(level){
    case TRACE:     return "TRACE";
    case DEBUG:     return "DEBUG";
    case INFO:      return "INFO";
    case WARNING:   return "WARNING";
    case ERROR:     return "ERROR";
    default:        return "";
    }
}

void Logger::LogPump(){
    using namespace std::chrono;
    LogMessage m;
    Logger& i = getInstance();

    while(true){
        std::unique_lock<std::mutex> l(i.m_QueueLock);
        if(i.m_LogQueue.empty()) {
            if(s_close)
                break;
            i.m_Logged.wait(l);
        }
        else {
            m = i.m_LogQueue.front();
            std::cout << "\e[";
            switch(m.level){
            case TRACE: 
                std::cout << BLUE;
                break;
            case INFO:
                std::cout << GREEN;
                break;
            case WARNING:
                std::cout << YELLOW;
                break;
            case ERROR:
                std::cout << RED;
                break;
            }
            std::cout << "m";
            std::cout << m.timestamp << ": " << std::left << std::setw(7) << getLevelString(m.level) <<  " [" << m.sender << "] " << m.message << std::endl;
            i.m_LogQueue.pop();
            std::cout << "\e[" << WHITE << "m";
        }
    }
}

void Logger::Init(){
    getInstance();
}

void Logger::Destroy(){
    s_close = true;
    getInstance().m_Logged.notify_all();
    getInstance().m_LoggerThread.join();
}

void Logger::Trace(std::string message, std::string sender){
    getInstance().Log(message,sender,TRACE);
}
void Logger::Debug(std::string message, std::string sender){
#ifdef  RUSH_BUILD_DEBUG
    getInstance().Log(message,sender,DEBUG);
#endif
}
void Logger::Info(std::string message, std::string sender){
    getInstance().Log(message,sender,INFO);
}
void Logger::Warning(std::string message, std::string sender){
    getInstance().Log(message,sender,WARNING);
}
void Logger::Error(std::string message, std::string sender){
    getInstance().Log(message,sender,ERROR);
}

void Logger::SetThreshold(LogLevel level){
    getInstance().m_Threshold = level;
}

void Logger::SetAlias(std::string alias,std::thread::id id){
    if(id == std::thread::id())
        id = std::this_thread::get_id();
    
    getInstance().m_AliasMap[id] = alias;
}

}