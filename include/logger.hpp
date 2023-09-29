#pragma once

#include <source_location>
#include <string>

#include "token.hpp"

namespace ofl
{
    class Logger;
    enum class LogSeverity
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    inline const char* to_string(LogSeverity sev)
    {
        static const char* strings[] = 
        {
            "Trace",
            "Debug",
            "Info",
            "Warn",
            "Error",
            "Fatal"
        };

        return strings[(int) sev];
    }

    class Log
    {
        friend Logger;

    public:
        Log() { }

        const char* c_str(){ return _message.c_str(); }
        
        template <typename T>
        Log& operator+(T& data)
        {
            _message += std::to_string(data);
            return *this;
        }

        template <typename T>
        Log& operator+(T&& data)
        {
            _message += std::to_string(data);
            return *this;
        }

        Log& operator+(Token& data)
        {
           _message += data.to_string();
            return *this; 
        }

        Log& operator+(std::string& data)
        {
           _message += data;
            return *this; 
        }

        Log& operator+(char* data)
        {
           _message += data;
            return *this; 
        }

        Log& operator+(const char* data)
        {
           _message += data;
            return *this; 
        }

    private:
        std::string _message;
    };

    class Logger
    {
    public:
        static void Log(Log& log, LogSeverity sev, std::source_location src)
        {
#ifdef OFL_PRODUCTION
            if(sev < LogSeverity::Info) return;
#endif
            printf("[%5s]: %s (%s %d:%d)\n", 
            to_string(sev), log._message.c_str(), 
            src.file_name(), src.line(), src.column());
        }
    };

    #ifndef LOG
    #define LOG(x,y) Logger::Log(Log() + y, x, std::source_location::current());
    #endif

    #ifndef MSG
    #define MSG(x) (Log() + x).c_str()
    #endif
}