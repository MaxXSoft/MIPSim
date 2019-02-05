#include <test.h>

#include <io/rom.h>
#include <io/ram.h>
#include <bus.h>
#include <cpu.h>

#include <unit/util.h>

using namespace std;

namespace {

Bus bus;
CPU cpu(bus);
auto rom = make_shared<ROM>();
auto ram = make_shared<RAM>();

const char *asm_str1 = {
    "00 00 00 00\n"   // nop
    "07 00 08 24\n"   // addiu $8, $0, 7
    "08 00 09 24\n"   // addiu $9, $0, 8
    "20 50 09 01\n"   // add $10, $8, $9
    "22 50 48 01\n"   // sub $10, $10, $8
    "23 50 49 01\n"   // subu $10, $10, $9
    "24 50 48 01\n"   // and $10, $10, $8
    "25 50 49 01\n"   // or $10, $10, $9
    "26 50 4a 01\n"   // xor $10, $10, $10
    "27 50 09 01\n"   // nor $10, $8, $9
    "2a 58 48 01\n"   // slt $11, $10, $8
    "2b 58 48 01\n"   // sltu $11, $10, $8
    "c2 58 0a 00\n"   // srl $11, $10, 3
    "00 59 0a 00\n"   // sll $11, $10, 4
    "03 59 0a 00\n"   // sra $11, $10, 4
    "06 58 0a 01\n"   // srlv $11, $10, $8
    "04 58 2a 01\n"   // sllv $11, $10, $9
    "03 00 00 10\n"   // beq $0, $0, end
    "07 58 0a 01\n"   // srav $11, $10, $8
    "07 58 0a 01\n"   // srav $11, $10, $8
    "07 58 0a 01\n"   // srav $11, $10, $8
    "07 00 08 24\n"   // addiu $8, $0, 7
    "08 00 09 24\n"   // addiu $9, $0, 8
    "09 00 09 24\n"   // addiu $9, $0, 9
    "e7 ff 00 10\n"   // beq $0, $0, begin
    "0a 00 00 24\n"   // addiu $0, $0, 10
};

const char *asm_str2 = {
    "00 00 00 00\n"   // nop
    "00 00 08 3c\n"   // li $8, 7
    "07 00 08 35\n"   // (pseudo-inst ext)
    "09 00 0a 31\n"   // andi $10, $8, 9
    "09 00 0a 35\n"   // ori $10, $8, 9
    "09 00 0a 3c\n"   // lui $10, 9
    "ff ff 09 3c\n"   // li $9, -9
    "f7 ff 29 35\n"   // (pseudo-inst ext)
    "08 00 2a 29\n"   // slti $10, $9, 8
    "f6 ff 48 15\n"   // bne $10, $8, begin
    "08 00 2a 2d\n"   // sltiu $10, $9, 8
};

const char *asm_str3 = {
    "ff ff 08 3c\n"   // li $t0, -12
    "f4 ff 08 35\n"   // (pseudo-inst ext)
    "00 00 09 3c\n"   // li $t1, 45
    "2d 00 29 35\n"   // (pseudo-inst ext)
    "18 00 09 01\n"   // mult $t0, $t1
    "10 50 00 00\n"   // mfhi $t2
    "12 58 00 00\n"   // mflo $t3
    "19 00 09 01\n"   // multu $t0, $t1
    "10 50 00 00\n"   // mfhi $t2
    "12 58 00 00\n"   // mflo $t3
    "1a 00 28 01\n"   // div $t1, $t0
    "10 50 00 00\n"   // mfhi $t2
    "12 58 00 00\n"   // mflo $t3
    "1b 00 09 01\n"   // divu $t0, $t1
    "10 50 00 00\n"   // mfhi $t2
    "12 58 00 00\n"   // mflo $t3
};

const char *asm_str4 = {
    "00 00 08 3c\n"   // li $t0, 3
    "03 00 08 35\n"   // (pseudo-inst ext)
    "05 00 08 ad\n"   // sw $t0, 5($t0)
    "05 00 09 8d\n"   // lw $t1, 5($t0)
    "07 00 09 10\n"   // beq $zero, $t1, end
    "cd ab 08 3c\n"   // li $t0, 0xabcdef12
    "12 ef 08 35\n"   // (pseudo-inst ext)
    "01 00 08 a0\n"   // sb $t0, 1
    "01 00 09 80\n"   // lb $t1, 1
    "04 00 09 90\n"   // lbu $t1, 4
    "06 00 08 a4\n"   // sh $t0, 6
    "06 00 09 84\n"   // lh $t1, 6
    "04 00 09 94\n"   // lhu $t1, 4
    "04 00 09 8c\n"   // lw $t1, 4
};

} // namespace

void CPUTest0() {
    // initialize CPU
    TEST_EXPECT(true, bus.AddIO(0xbfc00000, 1024, rom));
    TEST_EXPECT(true, bus.AddIO(0x00000000, 16384, ram));
}

void CPUTest1() {
    // initialize test case
    {
        ofstream ofs(kTempFile);
        ofs << asm_str1;
    }
    TEST_EXPECT(true, rom->LoadHex(kTempFile));
    cpu.Reset();
    // run test
    TEST_EXPECT(true, cpu.NextCycle());   // nop
    TEST_EXPECT(true, cpu.NextCycle());   // addiu $8, $0, 7
    TEST_EXPECT(7, cpu.ReadReg(8));
    TEST_EXPECT(true, cpu.NextCycle());   // addiu $9, $0, 8
    TEST_EXPECT(8, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // add $10, $8, $9
    TEST_EXPECT(15, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // sub $10, $10, $8
    TEST_EXPECT(8, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // subu $10, $10, $9
    TEST_EXPECT(0, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // and $10, $10, $8
    TEST_EXPECT(0, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // or $10, $10, $9
    TEST_EXPECT(8, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // xor $10, $10, $10
    TEST_EXPECT(0, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // nor $10, $8, $9
    TEST_EXPECT(~(8 | 7), cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // slt $11, $10, $8
    TEST_EXPECT(1, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // sltu $11, $10, $8
    TEST_EXPECT(0, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // srl $11, $10, 3
    TEST_EXPECT(0x1ffffffe, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // sll $11, $10, 4
    TEST_EXPECT(0xffffff00, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // sra $11, $10, 4
    TEST_EXPECT(0xffffffff, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // srlv $11, $10, $8
    TEST_EXPECT(0x01ffffff, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // sllv $11, $10, $9
    TEST_EXPECT(0xfffff000, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // beq $0, $0, end
    TEST_EXPECT(true, cpu.NextCycle());   // srav $11, $10, $8
    TEST_EXPECT(0xffffffff, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // addiu $8, $0, 7
    TEST_EXPECT(7, cpu.ReadReg(8));
    TEST_EXPECT(true, cpu.NextCycle());   // addiu $9, $0, 8
    TEST_EXPECT(8, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // addiu $9, $0, 9
    TEST_EXPECT(9, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // beq $0, $0, begin
    TEST_EXPECT(true, cpu.NextCycle());   // addiu $0, $0, 10
    TEST_EXPECT(0, cpu.ReadReg(0));
    TEST_EXPECT(true, cpu.NextCycle());   // nop
    TEST_EXPECT(true, cpu.NextCycle());   // addiu $8, $0, 7
    TEST_EXPECT(true, cpu.NextCycle());   // addiu $9, $0, 8
    TEST_EXPECT(8, cpu.ReadReg(9));
}

void CPUTest2() {
    // initialize test case
    {
        ofstream ofs(kTempFile);
        ofs << asm_str2;
    }
    TEST_EXPECT(true, rom->LoadHex(kTempFile));
    cpu.Reset();
    // run test
    TEST_EXPECT(true, cpu.NextCycle());   // nop
    TEST_EXPECT(true, cpu.NextCycle());   // li $8, 7
    TEST_EXPECT(true, cpu.NextCycle());   // (pseudo-inst ext)
    TEST_EXPECT(7, cpu.ReadReg(8));
    TEST_EXPECT(true, cpu.NextCycle());   // andi $10, $8, 9
    TEST_EXPECT(7 & 9, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // ori $10, $8, 9
    TEST_EXPECT(7 | 9, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // lui $10, 9
    TEST_EXPECT(0x00090000, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // li $9, -9
    TEST_EXPECT(true, cpu.NextCycle());   // (pseudo-inst ext)
    TEST_EXPECT(-9, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // slti $10, $9, 8
    TEST_EXPECT(1, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // bne $10, $8, begin
    TEST_EXPECT(true, cpu.NextCycle());   // sltiu $10, $9, 8
    TEST_EXPECT(0, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // nop
    TEST_EXPECT(true, cpu.NextCycle());   // li $8, 7
    TEST_EXPECT(true, cpu.NextCycle());   // (pseudo-inst ext)
    TEST_EXPECT(7, cpu.ReadReg(8));
}

void CPUTest3() {
    // initialize test case
    {
        ofstream ofs(kTempFile);
        ofs << asm_str3;
    }
    TEST_EXPECT(true, rom->LoadHex(kTempFile));
    cpu.Reset();
    // run test
    TEST_EXPECT(true, cpu.NextCycle());   // li $t0, -12
    TEST_EXPECT(true, cpu.NextCycle());   // (pseudo-inst ext)
    TEST_EXPECT(-12, cpu.ReadReg(8));
    TEST_EXPECT(true, cpu.NextCycle());   // li $t1, 45
    TEST_EXPECT(true, cpu.NextCycle());   // (pseudo-inst ext)
    TEST_EXPECT(45, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // mult $t0, $t1
    uint64_t hi = cpu.hi();
    TEST_EXPECT(45 * -12, (hi << 32) | cpu.lo());
    TEST_EXPECT(true, cpu.NextCycle());   // mfhi $t2
    TEST_EXPECT(0xffffffff, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // mflo $t3
    TEST_EXPECT(0xfffffde4, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // multu $t0, $t1
    hi = cpu.hi();
    TEST_EXPECT(0xfffffff4 * 45ULL, (hi << 32) | cpu.lo());
    TEST_EXPECT(true, cpu.NextCycle());   // mfhi $t2
    TEST_EXPECT(0x0000002c, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // mflo $t3
    TEST_EXPECT(0xfffffde4, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // div $t1, $t0
    TEST_EXPECT(true, cpu.NextCycle());   // mfhi $t2
    TEST_EXPECT(45 % -12, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // mflo $t3
    TEST_EXPECT(45 / -12, cpu.ReadReg(11));
    TEST_EXPECT(true, cpu.NextCycle());   // divu $t0, $t1
    TEST_EXPECT(true, cpu.NextCycle());   // mfhi $t2
    TEST_EXPECT(0xfffffff4 % 45, cpu.ReadReg(10));
    TEST_EXPECT(true, cpu.NextCycle());   // mflo $t3
    TEST_EXPECT(0xfffffff4 / 45, cpu.ReadReg(11));
}

void CPUTest4() {
    // initialize test case
    {
        ofstream ofs(kTempFile);
        ofs << asm_str4;
    }
    TEST_EXPECT(true, rom->LoadHex(kTempFile));
    cpu.Reset();
    // run test
    TEST_EXPECT(true, cpu.NextCycle());   // li $t0, 3
    TEST_EXPECT(true, cpu.NextCycle());   // (pseudo-inst ext)
    TEST_EXPECT(true, cpu.NextCycle());   // sw $t0, 5($t0)
    TEST_EXPECT(3, ram->ReadWord(8));
    TEST_EXPECT(true, cpu.NextCycle());   // lw $t1, 5($t0)
    TEST_EXPECT(3, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // beq $zero, $t1, end
    TEST_EXPECT(true, cpu.NextCycle());   // li $t0, 0xabcdef12
    TEST_EXPECT(true, cpu.NextCycle());   // (pseudo-inst ext)
    TEST_EXPECT(true, cpu.NextCycle());   // sb $t0, 1
    TEST_EXPECT(0x12, ram->ReadByte(1));
    TEST_EXPECT(true, cpu.NextCycle());   // lb $t1, 1
    TEST_EXPECT(0x00000012, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // lbu $t1, 4
    TEST_EXPECT(0, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // sh $t0, 6
    TEST_EXPECT(0xef12, ram->ReadHalf(6));
    TEST_EXPECT(true, cpu.NextCycle());   // lh $t1, 6
    TEST_EXPECT(0xffffef12, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // lhu $t1, 4
    TEST_EXPECT(0, cpu.ReadReg(9));
    TEST_EXPECT(true, cpu.NextCycle());   // lw $t1, 4
    TEST_EXPECT(0xef120000, cpu.ReadReg(9));
}
