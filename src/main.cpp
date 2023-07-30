#include <string>
#include <map>

#include "ast.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "assembler.hpp"
#include "executor.hpp"
#include "logger.hpp"

using namespace std;
using namespace ofl;

typedef int (*CommandFunc)(Parser&);

int Execute(Parser&);
int Compile(Parser&);

map<string, CommandFunc> Commands = 
{
    pair{string("run"), &Execute},
    pair{string("compile"), &Compile}
};

void PrintUsage();
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        // Log that not enough args were passed
        PrintUsage();
        return 1;
    } 

    // Retrive args
    const char* filename = argv[2];
    string command = argv[1];

    // Validate command
    auto it = Commands.find(command);
    if(it == Commands.end())
    {
        // Log that uknown command was passed
        PrintUsage();
        return 2;
    }

    // Create parser
    Parser parser = Parser(filename);

    // Invoke command
    return (*it->second)(parser);
}

int Execute(Parser& parser)
{
    // Use the parser to read in tokens from the file
    TokenList tokens;
    Executor executor = Executor();
    while(parser.ParseLine(tokens))
    {
        Node* root = Lexer::Lex(tokens);
        bool success = executor.Execute(root);
        
        if(!success) break;
        tokens.clear();
        break; //remkove
    }

#ifndef OFL_PRODUCTION
    for(auto token : tokens)
        LOG(LogSeverity::Debug, token._contents << ": " << (int) token._type);
        
#endif

#ifndef OFL_PRODUCTION
    
#endif

    LOG(LogSeverity::Debug, "Execution of file ended.");
    return 0;
}

int Compile(Parser& parser)
{
    LOG(LogSeverity::Warning, "Compile not implemented.");
    return 0;
}

void PrintUsage()
{
    static string usage = "Usage place holder.";

    printf("%s\n", usage.c_str());
}