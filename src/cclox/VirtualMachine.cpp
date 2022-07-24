#include "GarbageCollect/GarbageCollector.h"
#include "Common.h"
#include "VirtualMachine.h"
#include "Chunk.h"
#include "Debug.h"
#include "Compiler.h"
#include "ObjString.h"

namespace cclox {
    static VirtualMachine StaticVM;

    VirtualMachine::VirtualMachine()
            : _currentChunk(nullptr), _ip(nullptr), _valueStack(STACK_MAX) {

    }

    InterpretResult VirtualMachine::Interpret(std::unique_ptr<Chunk>&& chunk) {
        if (!chunk)
            return INTERPRET_COMPILE_ERROR;

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
                case OP_FALSE: {
                    PushSpecific(false);
                    break;
                }
                case OP_TRUE: {
                    PushSpecific(true);
                    break;
                }
                case OP_NIL: {
                    PushSpecific(nullptr);
                    break;
                }
                case OP_NOT: {
                    Value& topValue = _valueStack.back();
                    topValue = VAL_VAL(!AS_BOOL(topValue));
                    break;
                }
                case OP_NEGATE: {
                    if (!IS_NUMBER(_valueStack.back()))
                    {
                        RuntimeError("Operand must be a number, but %s!", GetValueTypeStr(_valueStack.back().type));
                        return INTERPRET_RUNTIME_ERROR;
                    }
                    NegateOperation();
                    break;
                }
                case OP_EQUAL:
                case OP_LESS:
                case OP_GREATER:
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
            case OP_ADD: {
                if (likely(IS_NUMBER(left) && IS_NUMBER(right)))
                    return PushSpecific(left + right);
                else {
                    if (likely(IS_STRING(left) && IS_STRING(right))) {
                        ObjString* newStr = ObjString::Concatenate(AS_STRING(left), AS_STRING(right));
                        PushSpecific(newStr);
                    }
                    else {
                        // todo:
                    }
                }
                break;
            }
            case OP_SUBTRACT:
                return PushSpecific(left - right);
            case OP_DIVIDE:
                return PushSpecific(left / right);
            case OP_MULTIPLY:
                return PushSpecific(left * right);
            case OP_EQUAL:
                return PushSpecific(left == right);
            case OP_LESS:
                return PushSpecific(left < right);
            case OP_GREATER:
                return PushSpecific(left > right);
            default:
                fprintf(stderr, "The operator %d not supported!", binOperator);
                return ;
        }
    }

    void VirtualMachine::NegateOperation() {
        AS_NUMBER(_valueStack.back()) = -AS_NUMBER(_valueStack.back());
    }

    InterpretResult VirtualMachine::Interpret(const std::string &source) {
        auto compiledChunk = Compiler::Get().Compile(source);
        _currentChunk = std::move(compiledChunk);
        InterpretResult result = Interpret(std::move(_currentChunk));
        GarbageCollector::Get().CollectGarbage();
        return result;
    }

    void VirtualMachine::ErrorAtLatest() {
        size_t preInstructionLineIndex = GetPCOffset() - 1;
        const auto& lines = _currentChunk->GetLines();
        uint32 line = lines[preInstructionLineIndex];
        fprintf(stderr, "[line %d] in script\n", line);
    }

    VirtualMachine::~VirtualMachine() {
    }
}
cclox::VirtualMachine *cclox::GlobalVM = &cclox::StaticVM;