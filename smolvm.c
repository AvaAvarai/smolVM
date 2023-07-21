/* 
- compile: gcc smolvm.c -o smolvm -Wall
- run: smolvm
- debug mode: smolvm debug

- MVP Goal: 3 opcodes -> 4 registers (byte size) [DONE]
- Stage 2: call structure and 8 bit width operands [DONE]
- Stage 3: debug args flag [DONE], read file for program
- Stage 4: more opcodes up to 16 total

call width of 20 bits.
Program call structure   opcode, operand1, operand2
                      0x F       FF        FF 
		     Example  0x 1       00        42 
Calls opcode 1 (add) with operand1 of 00 and operand2 of 42 adding 42 to register 0. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGISTER_COUNT 4


typedef struct {
    unsigned opcode;
    unsigned operand1;
    unsigned operand2;
} Call;

Call decodeInstruction(unsigned debug, unsigned instruction)
{
    Call call;
    call.opcode = (instruction & 0xF0000) >> 16;
    call.operand1 = (instruction & 0x0FF00) >> 8;
    call.operand2 = instruction & 0x000FF;
	if (debug) { printf("| instr %05x code %0x op1 %02x op2 %02x | ", instruction, call.opcode, call.operand1, call.operand2); }
    return call;
}

unsigned executeInstruction(unsigned char *registers, Call *call)
{
	unsigned running = 1;
    switch(call->opcode)
    {
        case 0: // exit vm
            running = 0;
            break;
        case 1: // add B to A
            registers[call->operand1] += call->operand2;
            break;
        case 2: // print according to ASCII table
            printf("%c", registers[call->operand1]);
            break;
    }
	return running;
}

void printState(unsigned char *registers, unsigned frame)
{	
    printf("Registers @%02d = ", frame);
    int i;
    for (i = 0; i < REGISTER_COUNT; i++)
    {
        printf("%03o ", registers[i]);
    }
}

void run(unsigned debug)
{
	unsigned program_counter = 0;
	unsigned char registers[REGISTER_COUNT];
	int i;
	for (i = 0; i < REGISTER_COUNT; i++)
	{
		registers[i] = 0;
	}
	
	// program prints "HI" then exits
	unsigned program[] = {0x10048, 0x20000, 0x10149, 0x20100, 0x0};
	
	unsigned running = 1;
	unsigned frame = 0;
	
    while(running)
    {
        if (debug) { printState(registers, frame); } // TODO: REPL mode vs script mode
        
		unsigned instruction = program[program_counter++];
		Call call = decodeInstruction(debug, instruction);
        running = executeInstruction(registers, &call);
		
		if (debug) { printf("\n"); }
		frame++;
	}
}

int main(int argc, char* argv[])
{
	unsigned debug = 0;
	if( argc > 2 )
	{
        printf("Too many arguments supplied only 'debug' or no flag are supported.\n");
		return -1;
    }
	else if( argc == 2 ) {
        if (strcmp(argv[1], "debug") == 0)
		{
			debug = 1;
		}
		else
		{
		    printf("Unknown arguments supplied only 'debug' or no flag are supported.\n");
			return -1;
		}
	}
    
    run(debug);
    return 0;
}
