#pragma once

#include <string>
#include <exception>

#include "logger.hpp"

namespace ofl
{
    class exception : public std::exception
    {
    public:
        exception(std::string message) :_message(message) { }
        virtual const char* what() const throw()
        {
            return _message.c_str();
        }
    protected:
        std::string _message;

    private:
        exception();
    };

    class parser_exception : public exception 
    {
    public:
        parser_exception(std::string message) :exception(message) { }
        parser_exception(std::string message, size_t line, size_t col) :exception(message) 
        { 
            _message += MSG("["+line+", "+col+"]");
        }
        virtual const char* what() const throw()
        {
            return _message.c_str();
        }
    };

    class lexer_exception : public exception 
    {
    public:
        lexer_exception(std::string message) :exception(message) { }
        lexer_exception(std::string message, Token& token) : exception(message) 
        { 
            _message += MSG(token);
        }
        virtual const char* what() const throw()
        { 
            //
            return _message.c_str();
        }
    };

     class executor_exception : public exception 
    {
    public:
        executor_exception(std::string message) :exception(message) {}
    };
}