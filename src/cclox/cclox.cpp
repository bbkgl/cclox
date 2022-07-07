#include "Chunk.h"
#include "Debug.h"
#include "VirtualMachine.h"
#include <iostream>
#include <sstream>
#include <fstream>

namespace cclox {
    static std::string ReadFileToString(const char* filePath) {
        std::ifstream fileStream(filePath);
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        return buffer.str();
    }

    void TestDisassemblyChunk()
    {
        auto chunk = std::make_unique<Chunk>();
        Chunk::ConstantIndex constantIndex = chunk->AddConstant(1.2);
        chunk->WriteChunk(OP_CONSTANT, 123);
        chunk->WriteChunk(constantIndex, 123);

        constantIndex = chunk->AddConstant(3.4);
        chunk->WriteChunk(OP_CONSTANT, 123);
        chunk->WriteChunk(constantIndex, 123);
        chunk->WriteChunk(OP_ADD, 123);

        constantIndex = chunk->AddConstant(5.6);
        chunk->WriteChunk(OP_CONSTANT, 123);
        chunk->WriteChunk(constantIndex, 123);
        chunk->WriteChunk(OP_DIVIDE, 123);

        chunk->WriteChunk(OP_NEGATE, 123);

        chunk->WriteChunk(OP_RETURN, 123);
        DisassembleChunk("test chunk", chunk.get());

        GlobalVM->Interpret(std::move(chunk));
    }

    void DoFile(const char* filePath) {
        std::string source = ReadFileToString(filePath);
        GlobalVM->Interpret(source);
    }

    void Repl()
    {
        std::string line;
        for (;;) {
            printf("> ");
            std::getline(std::cin, line);
            if (line == "exit()")
                break;
            GlobalVM->Interpret(line);
        }
    }
}

void RunFile(const char* filePath)
{
    cclox::DoFile(filePath);
}

void RunPrompt()
{

}

void Repl()
{
    cclox::Repl();
}

void TestDisassemblyChunk()
{
    cclox::TestDisassemblyChunk();

    //test ide
    cclox::VirtualMachine vm;
}
