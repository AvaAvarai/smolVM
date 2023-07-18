/* Written in a summer afternoon in notepad. */

/* compile & run: gcc smolvm.c -o smolvm -Wall && smolvm */
/* MVP Goal: 3 opcodes -> 4 registers (byte size) */

#include <stdio.h>

#define REGISTER_COUNT 4
#define DEBUG 1 // TODO: accept a runtime flag to toggle this

unsigned char registers[REGISTER_COUNT];

unsigned program_counter = 0;

// program prints "HI" then exits
unsigned program[] = {0x101, 0x102, 0x11F, 0x11F, 0x11F, 0x11F, 0x11C, 0x210, 0x12F, 0x12F, 0x12F, 0x12F, 0x12D, 0x220, 0x0};

unsigned running = 1;

unsigned opcode = 0; // TODO: make this a struct
unsigned operand1 = 0;
unsigned operand2 = 0;

unsigned frame = 0;

unsigned nextInstruction()
{
    return program[program_counter++];
}

void decodeInstruction(unsigned instruction)
{
    opcode = (instruction & 0xF00) >> 8;
    operand1 = (instruction & 0x0F0) >> 4;
    operand2 = instruction & 0x00F;
}

void executeInstruction()
{
    switch(opcode)
    {
        case 0: // exit vm
            running = 0;
            break;
        case 1: // add B to A
            registers[operand1] += operand2;
            break;
        case 2: // print according to ASCII table
            printf("%c", registers[operand1]);
            break;
    }
}

void printState()
{
    printf("Registers @%02d = ", frame++);
    int i;
    for (i = 0; i < REGISTER_COUNT; i++)
    {
        printf("%03o ", registers[i]);
    }
}

void run()
{
    while(running)
    {
        if (DEBUG) { printState(); } // TODO: REPL mode vs script mode
        decodeInstruction(nextInstruction());
        executeInstruction();
		if (DEBUG) { printf("\n"); }
    }
}

int main(int argc, char* argv[])
{
    run();
    return 0;
}
