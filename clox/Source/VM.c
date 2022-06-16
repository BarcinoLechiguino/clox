#include <stdio.h>

#include "Common.h"
#include "Debug.h"

#include "VM.h"

void InitVM(VM* vm)
{
	InitStack(&vm->stack);
	//ResetStack(vm);
}

void FreeVM(VM* vm)
{
	FreeStack(&vm->stack);
}

InterpretResult Interpret(VM* vm, Chunk* chunk)
{
	vm->chunk	= chunk;
	vm->ip		= vm->chunk->code;

	return Run(vm);
}

static InterpretResult Run(VM* vm)
{
	#define READ_BYTE()		(*vm->ip++)																// Ex: Returns instruction 0 and increases cursor to instruction 1.
	#define READ_CONSTANT()	(vm->chunk->constants.values[READ_BYTE()])
	#define BINARY_OP(op)				\
		do								\
		{								\
			double b = Pop(&vm->stack);	\
			double a = Pop(&vm->stack);	\
			Push(&vm->stack, (a op b));	\
		} while (false)				

	for (;;)																						// Infinite loop
	{
		#ifdef DEBUG_TRACE_EXECUTION
			printf("          ");
			for (Value* slot = vm->stack.elements; slot < vm->stack.top; ++slot)
			{
				printf("[ "); PrintValue(*slot); printf(" ]");
			}

			if (vm->stack.top == vm->stack.elements)
			{
				printf("[ EMPTY STACK ]");
			}
			printf("\n");

			DisassembleInstruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
		#endif

		uint8_t instruction;
		switch (instruction = READ_BYTE())
		{
		case OP_CONSTANT:		{ Push(&vm->stack, READ_CONSTANT());}								break;
		case OP_CONSTANT_LONG:	{ Push(&vm->stack, READ_CONSTANT());}								break;
		case OP_ADD:			{ BINARY_OP(+); }													break;
		case OP_SUBTRACT:		{ BINARY_OP(-); }													break;
		case OP_MULTIPLY:		{ BINARY_OP(*); }													break;
		case OP_DIVIDE:			{ BINARY_OP(/); }													break;
		case OP_NEGATE:			{ NegateTop(&vm->stack); }											break; // Push(vm, -Pop(vm)); would also work, but it implies unnecessary operations.
		case OP_RETURN:			{ PrintValue(Pop(&vm->stack)); printf("\n"); return INTERPRET_OK; }	break;
		}
	}

	#undef READ_BYTE
	#undef READ_CONSTANT
	#undef BINARY_OP
}