#pragma once

#include <string>
#include <exception>

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
    private:
        std::string _message;

        exception();
    };

    class parser_exception : public exception 
    {
    public:
        parser_exception(std::string message) :exception(message) {}
    };

    class lexer_exception : public exception 
    {
    public:
        lexer_exception(std::string message) :exception(message) {}
    };

     class executor_exception : public exception 
    {
    public:
        executor_exception(std::string message) :exception(message) {}
    };
}