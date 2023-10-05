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
    return (it->second)(parser);
}

int Execute(Parser& parser)
{
    // Use the parser to read in tokens from the file
    Lexer lexer = Lexer();
    Executor executor = Executor();
    while(parser.ParseLine(lexer._tokens))
    {
        // Continue if no tokens were found
        if(lexer._tokens.size() == 0)
            continue;

        auto& last_token = lexer._tokens[lexer._tokens.size()-1];
        if(last_token.Type() != TokenType::ENDOFFILE)
        {
            if(parser.Scoped())
                continue;

            if(!(last_token.Type() == TokenType::Delemiter && last_token.Data() == (void*) Character::SEMICOLON)
            && last_token.Type() != TokenType::Curly)
                continue;
        }
        /*
        for(auto& token : lexer._tokens)
            printf("%s ", token.to_string().c_str());
        printf("\n");
        */
        // Turn tokens into an AST
        Node root = lexer.Lex(executor.types);

        bool success = executor.Execute(&root);
        
        if(!success) break;
    }

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