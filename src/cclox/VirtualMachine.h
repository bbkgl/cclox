#ifndef CCLOX_VIRTUALMACHINE_H
#define CCLOX_VIRTUALMACHINE_H

#include <stack>
#include <string>
#include <memory>
#include "Chunk.h"
#include "Value.h"

namespace cclox {
    typedef enum {
        INTERPRET_OK,
        INTERPRET_COMPILE_ERROR,
        INTERPRET_RUNTIME_ERROR
    } InterpretResult;

    class Chunk;

    class VirtualMachine {
    public:
        explicit VirtualMachine();

        InterpretResult Interpret(std::unique_ptr<Chunk> chunk);
        InterpretResult Interpret(const std::string& source);
    private:
        typedef std::vector<Value> ValueStack;

        InterpretResult Run();

        size_t GetPCOffset();

        inline uint8 Step() {
            return *_ip++;
        }

        Value StepReadConstant();

        void BinaryOperation(OpCode binaryOperator);

        void Push(Value value);
        Value Pop();

        void NegateOperation();

        //
        std::unique_ptr<Chunk> _currentChunk;
        uint8* _ip;                   // offset in chunk op codes

        // stack
        ValueStack _valueStack;
    };

    extern VirtualMachine* GlobalVM;
}


#endif //CCLOX_VIRTUALMACHINE_H
