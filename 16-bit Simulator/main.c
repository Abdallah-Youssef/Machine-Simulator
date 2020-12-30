#include <stdio.h>
#include <stdlib.h>
#define byte unsigned short int

/// opcodes
#define MOVE    0
#define ADD     1
#define SUB     2
#define BRA     3
#define CMP     4
#define BEQ     5
#define BNE     6
#define EXG     7
#define AND     8
#define OR      9
#define XOR     10
#define LSL     11
#define LSR     12
#define MULU    13
#define STOP    15


/// addressing modes
#define ABSOLUTE    0
#define IMMEDIATE   1
#define INDEXED     2
#define RELATIVE    3


/// direction
#define REG_TO_MEM  0
#define MEM_TO_REG  1


#define addI(opcode,direction,amode,operand) pushInstruction(buildInstruction(opcode, direction, amode), operand, memory, &i);

void throw_error(char* error_msg, int PC){
    system("cls");  //clears console
    printf("%s at Instruction %d", error_msg, PC);
}

void print_opcode(byte opcode){
    switch (opcode){
    case MOVE:
        printf("MOVE"); break;
    case ADD:
        printf("ADD"); break;
    case SUB:
        printf("SUB"); break;
    case BRA:
        printf("BRA"); break;
    case CMP:
        printf("CMP"); break;
    case BEQ:
        printf("BEQ"); break;
    case BNE:
        printf("BNE"); break;
    case EXG:
        printf("EXG"); break;
    case STOP:
        printf("STOP"); break;
    }

    printf("\t");
}

void print_amode(byte amode){
    switch (amode){
        case ABSOLUTE:  {printf("ABSOULUTE");   break;}
        case IMMEDIATE: {printf("IMMEDIATE");   break;}
        case INDEXED:   {printf("INDEXED");     break;}
        case RELATIVE:  {printf("RELATIVE");    break;}
    }

    printf("\t");
}

void print_direction(byte direction){
    switch (direction){
        case MEM_TO_REG: {printf("MEM_TO_REG");   break;}
        case REG_TO_MEM: {printf("REG_TO_MEM");   break;}
    }

    printf("\t");
}

void print_operand(byte operand){
    printf("%d\t",operand);
}


void print_binary(byte x){
    char str[8] = {0};
    for (int i = 0;i < 8;i++){
        str[7-i] = (x % 2) + '0';
        x /= 2;
    }
    printf("%s\n", str);
}

void print_mem(byte* memory){
    for (int i = 0;i < 40;i++){
        printf("%d\t%d\t",i, memory[i]);
        print_binary(memory[i]);
    }
}

void pushInstruction(byte instruction, byte operand, byte* memory,int *i){
    memory[*i] = instruction;
    (*i)++;

    memory[*i] = operand;
    (*i)++;
}

int buildInstruction (byte opcode, byte direction, byte amode){
    return (opcode << 4) + (direction << 2) + amode;
}



/**
    Multiply [30] by [31]
*/
void loadProgram2(byte* memory){

    int count_address = 32;
    int sum_address = 33;


    memory[30] = 13;
    memory[31] = 14;
    memory[count_address] = 0;     ///will be init with = [21]
    memory[sum_address] = 0;     ///sum result


    int i = 0;

    //  [21] * [22]
    //opcode  amode   operand
    addI(MOVE, MEM_TO_REG, ABSOLUTE,30);            ///0 : load [30] into D0
    addI(MOVE, REG_TO_MEM, ABSOLUTE,count_address); ///2 : copy D0 into count_address

    printf("\n==============%d\n====================\n", memory[count_address]);
    print_mem(memory);
    // Loop
    addI(MOVE, MEM_TO_REG, ABSOLUTE,count_address); ///4 : Copy [23] into  D0, first line in loop
    addI(CMP,  MEM_TO_REG, IMMEDIATE,0);            ///6 : is D0 equal to 0?
    addI(BNE,  MEM_TO_REG, IMMEDIATE, 12);          ///8 : not done yet, jump over the stop
    addI(STOP, 1,3,255);                            ///10 : Stop
    addI(MOVE, MEM_TO_REG, ABSOLUTE,sum_address)    ///12 : Load sum
    addI(ADD, MEM_TO_REG, ABSOLUTE,31);             ///14 : increase sum by [31]
    addI(MOVE, REG_TO_MEM, ABSOLUTE, sum_address);  ///16 : save the new sum

    //Decrement count
    addI(MOVE, MEM_TO_REG, ABSOLUTE,count_address); ///18 : Load count
    addI(SUB, MEM_TO_REG, IMMEDIATE, 1);            ///20 : Decrement
    addI(MOVE, REG_TO_MEM, ABSOLUTE,count_address); ///22 : Save new count
    addI(BRA,MEM_TO_REG,IMMEDIATE, 4);              ///24 : branch to the loop start
}


/**
calculate a^b in O(log(b))
*/
void loadFastExpo(byte* memory){
    int i = 0;
    int res_address = 200;
    int a_address = 201;
    int b_address = 202;
    int temp_address = 203;


    memory[res_address] = 1;
    memory[a_address]   = 3;
    memory[b_address]   = 3;

    /*
    res = 1
    while b != 0
        if (b & 1) == 0
            res *= a
        a *= a
        b >>= 1
    */


    addI(MOVE, MEM_TO_REG,ABSOLUTE,b_address);      ///0: load b into D0
    addI(CMP, MEM_TO_REG,IMMEDIATE,0);              ///2; is b == 0?
    addI(BEQ, MEM_TO_REG,IMMEDIATE,34);             ///4; yes, branch to stop
    addI(MOVE, REG_TO_MEM,ABSOLUTE,temp_address);   ///6; copy b (currently in D0) into temp
    addI(MOVE, MEM_TO_REG,IMMEDIATE,1);             ///8;   D0 = 1
    addI(AND, REG_TO_MEM,ABSOLUTE,temp_address);    ///10; [temp_address] = D0 & [temp_address]
    addI(BEQ, MEM_TO_REG, IMMEDIATE,18);            ///12: if [temp_address] = 0, CCR = 1, branch to after if

    addI(MOVE, MEM_TO_REG,ABSOLUTE,res_address);    ///14; load res into D0
    addI(MULU, MEM_TO_REG,ABSOLUTE,a_address);      ///16: D0 *= a
    addI(MOVE, REG_TO_MEM,ABSOLUTE,res_address);    ///18: save res

    addI(MOVE, MEM_TO_REG,ABSOLUTE,a_address);      ///20; load a into D0
    addI(MULU, MEM_TO_REG,ABSOLUTE,a_address);      ///22: D0 *= a
    addI(MOVE, REG_TO_MEM,ABSOLUTE,a_address);      ///24: save a

    addI(MOVE, MEM_TO_REG,ABSOLUTE,b_address);      ///26; load b into D0
    addI(LSR,  MEM_TO_REG,IMMEDIATE,1);             ///28: D0 >>= 1
    addI(MOVE, REG_TO_MEM,ABSOLUTE,b_address);      ///30: save b

    addI(BRA, MEM_TO_REG,IMMEDIATE,2);              ///32: repeat
    addI(STOP, 1,3,255);                            ///34: stop
}

void print_FastExpo(byte* memory){
    int res_address = 200;
    int a_address = 201;
    int b_address = 202;
    int temp_address = 203;

    printf("a = %d\n", memory[a_address]);
    printf("b = %d\n", memory[b_address]);
    printf("res = %d\n", memory[res_address]);
}
void loadBranchTest(byte* memory){
    int i = 0;
    addI(BRA,MEM_TO_REG,IMMEDIATE,10);
    i = 12;
    addI(STOP,1,3,255);
}

void loadMoveTest(byte* memory){
    int i = 0;
    addI(MOVE,MEM_TO_REG,IMMEDIATE,32);
    addI(MOVE,REG_TO_MEM,ABSOLUTE,20);
    addI(MOVE,REG_TO_MEM,ABSOLUTE,21);
    addI(MOVE,MEM_TO_REG,ABSOLUTE,21);
    addI(ADD,MEM_TO_REG,IMMEDIATE,100);
    addI(MOVE,REG_TO_MEM,ABSOLUTE,22);
    addI(STOP,1,3,255);
}


int main()
{
    byte PC = 0;
    byte D0 = 0;
    byte A0 = 0;
    byte CCR = 0;
    byte MAR;
    byte MBR;
    byte IR;
    byte operand;
    byte source;
    byte* destination;
    byte opcode;
    byte amode;
    byte direction;
    byte memory[256] = {0};    ///8-bit operand -> 2^8 memory locations
    byte run = 1;


    /**
    Instruction format
        7 6 5 4 3 2 1 0
        [7,4] -> opcode
        [2]   -> direction
        [1,0] -> addressing mode

        ,then 8 bits for an operand
    **/

    loadFastExpo(memory);
    //print_mem(memory);

    /// 16-bit: first load 8-bit instruction then the address in the following 8-bits
    while (run){

        /// instruction
        MAR = PC;           /// load address of next opcode into MAR
        PC += 1;            /// increment program counter
        MBR = memory[MAR];  /// get instruction from memory
        IR = MBR;           /// load instruction into IR
        opcode = IR;        /// load instruction bits into opcode

        /// operand
        MAR = PC;           /// load address of operand into MAR
        PC += 1;            /// increment program counter
        MBR = memory[MAR];  /// get operand from memory
        IR = MBR;           /// load operand into IR
        operand = IR;       /// store operand


        /// extracting instruction data
        amode       = (opcode & 0b00000011);
        direction   = (opcode & 0b00000100) >> 2;
        opcode >>= 4;


        /// setting up source and destination
        switch (amode){
            // operand is address
            case ABSOLUTE:  {
                //operand = memory[operand];
                if (direction == MEM_TO_REG){
                    source = memory[operand];       //source from memory
                    destination = &D0;              //destination is register
                }
                if (direction == REG_TO_MEM){
                    source = D0;                    //source is the register
                    destination=&(memory[operand]); //destination is memory
                }
                break;
            }

            // operand is the value. It is only allowed to be MEM_TO_REG
            case IMMEDIATE: {
                if (direction == MEM_TO_REG){
                    source = operand;       //source from instruction's operand
                    destination = &D0;      //destination is register
                }
                if (direction == REG_TO_MEM){
                    throw_error("Immediate destination error", PC-2);
                    return -1;
                }
                break;
            }

            //default case is memory address but shifted
            default:   {
                byte shift = (amode == INDEXED ? A0 : PC);
                if (direction == MEM_TO_REG){
                    source = memory[operand + shift];      //source from address shifted
                    destination = &D0;                  //destination is register
                }
                if (direction == REG_TO_MEM){
                    source = D0;                         //source is the register
                    destination = &(memory[operand + shift]); //destination is memory
                }
                break;
            }
        }




        /// Prints
        printf("Instruction : %d\n", PC-2);
        print_opcode(opcode);
        print_direction(direction);
        print_amode(amode);
        printf("operand = %d\t", operand);
        printf("%d -> %d\t", source, (*destination));

        switch (opcode){
            case MOVE:{
                *destination = source;
                CCR = !D0;
                break;
            }

            case ADD:{
                *destination += source;
                CCR = !(*destination);
                break;
            }

            case MULU:{
                *destination *= source;
                CCR = !(*destination);
                break;
            }

            case SUB:{
                *destination -= source;
                CCR = !(*destination);
                break;
            }

            case BRA:{
                // If direction is MEM_TO_REG, then the operand value is found in the source
                //  else the operand value is in the *destination
                byte operand_value = (direction == MEM_TO_REG) ? source : *destination;
                PC = operand_value;
                break;
            }

            case CMP:{
                // CCR is equal to 1 if : *destination - source = 0
                CCR = (( (*destination) - source ) == 0);
                break;
            }
            case BEQ:{
                byte operand_value = (direction == MEM_TO_REG) ? source : *destination;
                PC = CCR ? operand_value : PC;
                break;
            }
            case BNE:{
                byte operand_value = (direction == MEM_TO_REG) ? source : *destination;
                PC = !CCR ? operand_value : PC;
                break;
            }
            case EXG:{
                MBR = D0;
                D0 = A0;
                A0 = MBR;
                break;
            }

            case AND:{
                *destination &= source;
                CCR = !(*destination);
                break;
            }
            case OR:{
                *destination |= source;
                CCR = !(*destination);
                break;
            }
            case XOR:{
                *destination ^= source;
                CCR = !(*destination);
                break;
            }
            case LSL:{
                *destination <<= source;
                CCR = !(*destination);
                break;
            }
            case LSR:{
                *destination >>= source;
                CCR = !(*destination);
                break;
            }

            case STOP:{
                run = 0;
                break;
            }
        }

        printf("\tD0 = %d\n", D0);

    }

    //print_mem(memory);
    print_FastExpo(memory);

    return 0;
}
