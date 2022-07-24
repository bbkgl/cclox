#ifndef CCLOX_VIRTUALMACHINE_H
#define CCLOX_VIRTUALMACHINE_H

#include <stack>
#include <string>
#include <memory>
#include <string_view>
#include <vector>
#include "Value.h"
#include "Utils.h"

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
        ~VirtualMachine();

        InterpretResult Interpret(std::unique_ptr<Chunk>&& chunk);
        InterpretResult Interpret(const std::string& source);
    private:
        typedef std::vector<Value> ValueStack;

        // ------------------------------------Methods------------------------------------
        InterpretResult Run();

        size_t GetPCOffset();

        inline uint8 Step() {
            return *_ip++;
        }

        Value StepReadConstant();

        void BinaryOperation(OpCode binaryOperator);

        template <typename T>
        void PushSpecific(T value) {
            Push(VAL_VAL(value));
        }

        void Push(Value value);
        Value Pop();
        void NegateOperation();

        void ErrorAtLatest();

        template<typename ... Args>
        void RuntimeError(std::string_view format, Args... args) {
            std::string&& errorMsg = string_format(format, args...);
            fprintf(stderr, "%s\n", errorMsg.c_str());

            ErrorAtLatest();
            _valueStack.clear();
        }

        // ------------------------------------Properties------------------------------------
        std::unique_ptr<Chunk> _currentChunk;
        uint8* _ip;                   // offset in chunk op codes

        // stack
        ValueStack _valueStack;
    };

    extern VirtualMachine* GlobalVM;
}


#endif //CCLOX_VIRTUALMACHINE_H
