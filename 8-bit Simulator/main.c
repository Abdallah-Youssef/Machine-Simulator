#include <stdio.h>
#include <stdlib.h>
#define byte unsigned short int
#define LDA 0
#define STA 1
#define ADD 2
#define BRA 3
#define BEQ 4
#define SUB 5
#define STOP 7
#define IMMEDIATE 1
#define ABSOLUTE 0
#define addI(a,b,c) pushInstruction(buildInstruction(a, b, c), memory, &i);

void print_opcode(int x){
    switch (x){
    case 0:
        printf("LDA"); break;
    case 1:
        printf("STA"); break;
    case 2:
        printf("ADD"); break;
    case 3:
        printf("BRA"); break;
    case 4:
        printf("BEQ"); break;
    case 7:
        printf("STOP"); break;
    }
}

void print_amode(int x){
    printf("\t");
    if (x == IMMEDIATE)
        printf("IMMEDIATE");
    else printf("ABSOULTE");
    printf("\t");
}

void print_operand(int x){
    printf("%d",x);
}

void binary(byte x){
    char str[8] = {0};
    for (int i = 0;i < 8;i++){
        str[7-i] = (x % 2) + '0';
        x /= 2;
    }

    printf("%s\n", str);
}

void print_mem(byte* memory){
    for (int i = 0;i < 16;i++){
        printf("%d\t%d\t",i, memory[i]);
        binary(memory[i]);
    }
}

void pushInstruction(byte buildInstruction, byte* memory,int *i){
    memory[*i] = buildInstruction;
    (*i)++;
}

int buildInstruction (int opcode, int amode, int operand){
    return (opcode << 5) + (amode << 4) + operand;
}


/**
    Add 3 + 5
*/
void loadProgram1(byte* memory){
    int i = 0;
    //opcode  amode   operand
    addI(LDA,IMMEDIATE,0);
    addI(ADD,IMMEDIATE,3);
    addI(ADD,IMMEDIATE,5);
    addI(STA,ABSOLUTE,10);
    addI(STOP,IMMEDIATE,15);

}

/**
    Multiply [11] by [12]
*/
void loadProgram2(byte* memory){

    memory[11] = 10;
    memory[12] = 3;
    memory[13] = 0;     ///sum result

    int i = 0;


    //  [11] * [12]
    //opcode  amode   operand
    addI(LDA,ABSOLUTE,11);      ///  0 : Load count of numbers stored in address 10
    addI(BEQ,IMMEDIATE,9);       ///  1 : if [11] = 0 branch to STOP
    addI(LDA,ABSOLUTE,13);     ///  2 : load sum to D0
    addI(ADD,ABSOLUTE,12)       ///  3 : D0 += [12]
    addI(STA,IMMEDIATE,13);     ///  4 : store D0 to sum
    addI(LDA,ABSOLUTE,11);      ///  5 : load [11] to D0
    addI(SUB,IMMEDIATE,1);      ///  6 : D0--
    addI(STA,ABSOLUTE,11);      ///  7 : store D0 back to [11]
    addI(BRA,IMMEDIATE,0);      ///  8 : repeat
    addI(STOP,IMMEDIATE,15);    ///  9 : stop

}

void loadProgram3(byte* memory){
    int i = 0;

    byte new_memory[16] = {
        0b00001010, // LDA 2
        0b01010001, // ADD 1
        0b00101010, // STA D0 in address 10
        0b00000000,
        0,0,0,0,0,0,2,0,0,0,0,
        0b11100000};

    for (int i = 0;i < 16;i++)
        memory[i] = new_memory[i];
}
int main()
{
    byte PC = 0;
    byte D0 = 0;
    byte MAR;
    byte MBR;
    byte IR;
    byte operand;
    byte source;
    byte opcode;
    byte amode;
    byte memory[16] = {0};
    ///4-bit operand -> 2^4 memory locations

    /**
    Instruction format
        x x x amode o o o o
        x => instruction code
        amode => addressing mode
        o => operand
    **/

    loadProgram2(memory);
    print_mem(memory);
    while (1){
        MAR = PC;   /// load address of next instruction into MAR
        PC += 1;    /// increment program counter
        MBR = memory[MAR];  /// get instruction
        IR = MBR;      /// load instruction into IR
        byte run = 1;

        opcode = IR >> 5;
        amode = (IR & 0b00010000) >> 4;
        operand = IR & 0b00001111;

        printf("Instruction : %d\n", PC-1);




        if (amode != IMMEDIATE)
            source = memory[operand];
        else source = operand;

        print_opcode(opcode);
        print_amode(amode);
        print_operand(operand);
        printf("\tSource = %d", source);

        switch (opcode){
            case LDA:{
                D0 = source;
                break;
            }

            case STA:{
                memory[operand] = D0;
                break;
            }

            case ADD:{
                D0 += source;
                break;
            }

            case SUB:{
                D0 -= source;
                break;
            }
            case BRA:{
                PC = operand;
                break;
            }

            case BEQ:{
                PC = D0 ? PC : operand;
                break;
            }
            case STOP:{
                run = 0;
                break;
            }

        }

        printf("\tD0 = %d\n", D0);

        if (!run)
            break;

    }
    print_mem(memory);
    return 0;
}
