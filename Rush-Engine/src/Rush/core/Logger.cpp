#include "Rushpch.h"
#include "Logger.h"

#include <iostream>
#include <iomanip>

#ifdef RUSH_WINDOWS
    #include <Windows.h>
#endif

namespace Rush {

bool Logger::s_close = false;

std::ostream& operator<<(std::ostream& os, Color col) {
#ifndef RUSH_WINDOWS
    return os << "\e[" << col.col << "m";
#else
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (col.col) {
    case BLACK:
        SetConsoleTextAttribute(hConsole, 0);
        break;
    case WHITE:
        SetConsoleTextAttribute(hConsole, 15);
        break;
    case RED:
        SetConsoleTextAttribute(hConsole, 12);
        break;
    case GREEN:
        SetConsoleTextAttribute(hConsole, 10);
        break;
    case YELLOW:
        SetConsoleTextAttribute(hConsole, 14);
        break;
    case BLUE:
        SetConsoleTextAttribute(hConsole, 9);
        break;
    default:
        RUSH_LOG_WARNING("Invalid color specified!");
        break;
    }
    return os;
#endif
}

Logger::Logger()
:   m_LogQueue(), m_Threshold(LogLevel::Info), m_QueueLock(), m_AliasMap(){
    m_LoggerStart = std::chrono::system_clock::now();
    s_close = false;
    m_LoggerThread = std::thread(LogPump);
    Log("Logger started!","Logger",LogLevel::Info);
}

std::string getLevelString(LogLevel level){
    switch(level){
    case LogLevel::Trace:     return "TRACE";
    case LogLevel::Debug:     return "DEBUG";
    case LogLevel::Info:      return "INFO";
    case LogLevel::Warning:   return "WARNING";
    case LogLevel::Error:     return "ERROR";
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
            switch(m.level){
            case LogLevel::Trace:
                std::cout << Color(BLUE);
                break;
            case LogLevel::Info:
                std::cout << Color(GREEN);
                break;
            case LogLevel::Warning:
                std::cout << Color(YELLOW);
                break;
            case LogLevel::Error:
                std::cout << Color(RED);
                break;
            }
            std::cout << m.timestamp << ": " << std::left << std::setw(7) << getLevelString(m.level) <<  " [" << m.sender << "] " << m.message << std::endl;
            i.m_LogQueue.pop();
            std::cout << Color(WHITE);
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

void Logger::SetAlias(std::string alias,std::thread::id id){
    if(id == std::thread::id())
        id = std::this_thread::get_id();
    
    getInstance().m_AliasMap[id] = alias;
}

}