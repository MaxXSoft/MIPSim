#ifndef MIPSIM_CPU_H_
#define MIPSIM_CPU_H_

#include <cstdint>

#include <bus.h>

class CPU {
public:
    CPU(Bus &bus) : bus_(bus) { Reset(); }

    void Reset() {
        for (auto &&i : reg_) i = 0;
        for (auto &&i : cp0_) i = 0;
        pc_ = 0xbfc00000;
        hi_ = lo_ = branch_addr_ = 0;
        branch_ = false;
    }
    bool NextCycle();

    std::uint32_t ReadReg(std::size_t i) const { return reg_[i]; }
    std::uint32_t ReadCP0(std::size_t i) const { return cp0_[i]; }

    std::uint32_t pc() const { return pc_; }
    std::uint32_t hi() const { return hi_; }
    std::uint32_t lo() const { return lo_; }

private:
    bool DoCalucation(int opcode, int funct,
            int rs, int rt, int rd, int shamt, int imm);
    void DoBranch(bool cond, int offset, bool al = false);
    void DoJump(std::uint32_t addr, bool al = false, int rd = 31) {
        branch_addr_ = addr;
        branch_ = true;
        if (al) reg_[rd] = pc_ + 4;
    }

    Bus &bus_;
    std::uint32_t reg_[32], cp0_[32];
    std::uint32_t pc_, hi_, lo_, branch_addr_;
    bool branch_;
};

#endif // MIPSIM_CPU_H_
