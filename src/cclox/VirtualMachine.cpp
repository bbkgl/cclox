#include "Common.h"
#include "VirtualMachine.h"
#include "Debug.h"
#include "Compiler.h"

namespace cclox {
    static VirtualMachine StaticVM;

    VirtualMachine::VirtualMachine()
            : _currentChunk(nullptr), _ip(nullptr), _valueStack(STACK_MAX) {

    }

    InterpretResult VirtualMachine::Interpret(std::unique_ptr<Chunk> chunk) {
        _currentChunk = std::move(chunk);
        _ip = _currentChunk->GetCodePointer();
        _valueStack.clear();
        return Run();
    }

    InterpretResult VirtualMachine::Run() {
        if (nullptr == _currentChunk)
            return INTERPRET_RUNTIME_ERROR;

        for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
            for (auto slot = _valueStack.begin(); slot < _valueStack.end(); slot++) {
                printf("[ ");
                PrintValue(*slot);
                printf(" ]");
            }
            printf("\n");

            DisassembleInstruction(_currentChunk.get(), GetPCOffset());
#endif
            uint8 instruction;
            switch (instruction = Step()) {
                case OP_CONSTANT: {
                    Value constant = StepReadConstant();
                    Push(constant);
                    break;
                }
                case OP_NEGATE: {
                    NegateOperation();
                    break;
                }
                case OP_MULTIPLY:
                case OP_ADD:
                case OP_SUBTRACT:
                case OP_DIVIDE:
                    BinaryOperation(static_cast<OpCode>(instruction));
                    break;
                case OP_RETURN:
                    PrintValue(Pop());
                    printf("\n");
                    return INTERPRET_OK;
            }
        }
        return INTERPRET_RUNTIME_ERROR;
    }

    size_t VirtualMachine::GetPCOffset() {
        return _ip - _currentChunk->GetCodePointer();
    }

    Value VirtualMachine::StepReadConstant() {
        Chunk::ConstantIndex constantIndex = Step();
        Value constant = _currentChunk->_constants[constantIndex];
        return constant;
    }

    Value VirtualMachine::Pop() {
        Value topValue = _valueStack.back();
        _valueStack.pop_back();
        return topValue;
    }

    void VirtualMachine::Push(Value value) {
        _valueStack.push_back(value);
    }

    void VirtualMachine::BinaryOperation(OpCode binOperator) {
        Value right = Pop();
        Value left = Pop();
        switch (binOperator) {
            case OP_ADD:
                return Push(left + right);
            case OP_SUBTRACT:
                return Push(left - right);
            case OP_DIVIDE:
                return Push(left / right);
            case OP_MULTIPLY:
                return Push(left * right);
            default:
                fprintf(stderr, "The operator %d not supported!", binOperator);
                return ;
        }
    }

    void VirtualMachine::NegateOperation() {
//        Push(-Pop());
        _valueStack.back() = -_valueStack.back();
    }

    InterpretResult VirtualMachine::Interpret(const std::string &source) {
        auto compiledChunk = Compiler::Get().Compile(source);
        _currentChunk = std::move(compiledChunk);
        InterpretResult result = Interpret(std::move(_currentChunk));
        return result;
    }
}
cclox::VirtualMachine *cclox::GlobalVM = &cclox::StaticVM;