#include <cpu.h>

// NOTE: this implementation will not consider exceptions

using namespace std;

namespace {

// CP0 registers
// const int kCP0BadVAddr = 8;
// const int kCP0Status = 12;
// const int kCP0Cause = 13;
// const int kCP0EPC = 14;

// opcode
const int kFunct    = 0b000000;
const int kBranch   = 0b000001;
const int kPriority = 0b010000;
const int kADDI     = 0b001000;
const int kADDIU    = 0b001001;
const int kSLTI     = 0b001010;
const int kSLTIU    = 0b001011;
const int kANDI     = 0b001100;
const int kLUI      = 0b001111;
const int kORI      = 0b001101;
const int kXORI     = 0b001110;
const int kBEQ      = 0b000100;
const int kBNE      = 0b000101;
const int kBGTZ     = 0b000111;
const int kBLEZ     = 0b000110;
const int kJ        = 0b000010;
const int kJAL      = 0b000011;
const int kLB       = 0b100000;
const int kLBU      = 0b100100;
const int kLH       = 0b100001;
const int kLHU      = 0b100101;
const int kLW       = 0b100011;
const int kSB       = 0b101000;
const int kSH       = 0b101001;
const int kSW       = 0b101011;

// funct
const int kADD      = 0b100000;
const int kADDU     = 0b100001;
const int kSUB      = 0b100010;
const int kSUBU     = 0b100011;
const int kSLT      = 0b101010;
const int kSLTU     = 0b101011;
const int kDIV      = 0b011010;
const int kDIVU     = 0b011011;
const int kMULT     = 0b011000;
const int kMULTU    = 0b011001;
const int kAND      = 0b100100;
const int kNOR      = 0b100111;
const int kOR       = 0b100101;
const int kXOR      = 0b100110;
const int kSLLV     = 0b000100;
const int kSLL      = 0b000000;
const int kSRAV     = 0b000111;
const int kSRA      = 0b000011;
const int kSRLV     = 0b000110;
const int kSRL      = 0b000010;
const int kJR       = 0b001000;
const int kJALR     = 0b001001;
const int kMFHI     = 0b010000;
const int kMFLO     = 0b010010;
const int kMTHI     = 0b010001;
const int kMTLO     = 0b010011;
const int kBREAK    = 0b001101;
const int kSYSCALL  = 0b001100;

// branch (rt)
const int kBGEZ     = 0b00001;
const int kBLTZ     = 0b00000;
const int kBGEZAL   = 0b10001;
const int kBLTZAL   = 0b10000;

// priority (rs)
const int kERET     = 0b10000;
const int kMFC0     = 0b00000;
const int kMTC0     = 0b00100;

inline uint32_t SignExt(uint32_t value, int bits) {
    return value & (1 << (bits - 1)) ? value | ~((1 << bits) - 1) : value;
}

} // namespace

bool CPU::DoCalucation(int opcode, int funct,
        int rs, int rt, int rd, int shamt, int imm) {
    auto signed_imm = SignExt(imm, 16);
    switch (opcode) {
        case kFunct: {
            switch (funct) {
                case kADD: case kADDU: reg_[rd] = reg_[rs] + reg_[rt]; break;
                case kSUB: case kSUBU: reg_[rd] = reg_[rs] - reg_[rt]; break;
                case kSLT: reg_[rd] = static_cast<int32_t>(reg_[rs])
                                    < static_cast<int32_t>(reg_[rt]); break;
                case kSLTU: reg_[rd] = reg_[rs] < reg_[rt]; break;
                case kAND: reg_[rd] = reg_[rs] & reg_[rt]; break;
                case kNOR: reg_[rd] = ~(reg_[rs] | reg_[rt]); break;
                case kOR: reg_[rd] = reg_[rs] | reg_[rt]; break;
                case kXOR: reg_[rd] = reg_[rs] ^ reg_[rt]; break;
                case kSLLV: reg_[rd] = reg_[rt] << (reg_[rs] & 0x1f); break;
                case kSLL: reg_[rd] = reg_[rt] << shamt; break;
                case kSRAV: reg_[rd] = static_cast<int32_t>(reg_[rt])
                                    >> (reg_[rs] & 0x1f); break;
                case kSRA: reg_[rd] = static_cast<int32_t>(reg_[rt])
                                   >> shamt; break;
                case kSRLV: reg_[rd] = reg_[rt] >> (reg_[rs] & 0x1f); break;
                case kSRL: reg_[rd] = reg_[rt] >> shamt; break;
                case kDIV: {
                    int32_t lhs = reg_[rs], rhs = reg_[rt];
                    lo_ = lhs / rhs;
                    hi_ = lhs % rhs;
                    break;
                }
                case kDIVU: {
                    lo_ = reg_[rs] / reg_[rt];
                    hi_ = reg_[rs] % reg_[rt];
                    break;
                }
                case kMULT: {
                    int32_t lhs = reg_[rs], rhs = reg_[rt];
                    int64_t result = lhs * rhs;
                    lo_ = result & 0xffffffff;
                    hi_ = result >> 32;
                    break;
                }
                case kMULTU: {
                    uint64_t lhs = reg_[rs], rhs = reg_[rt], result = lhs * rhs;
                    lo_ = result & 0xffffffff;
                    hi_ = result >> 32;
                    break;
                }
                default: return false;
            }
            break;
        }
        case kADDI: case kADDIU: reg_[rt] = reg_[rs] + signed_imm; break;
        case kSLTI: reg_[rt] = static_cast<int32_t>(reg_[rs])
                             < static_cast<int32_t>(signed_imm); break;
        case kSLTIU: reg_[rt] = reg_[rs] < signed_imm; break;
        case kANDI: reg_[rt] = reg_[rs] & imm; break;
        case kLUI: reg_[rt] = imm << 16; break;
        case kORI: reg_[rt] = reg_[rs] | imm; break;
        case kXORI: reg_[rt] = reg_[rs] ^ imm; break;
        default: return false;
    }
    return true;
}

void CPU::DoBranch(bool cond, int offset, bool al) {
    if (al) reg_[31] = pc_ + 4;
    if (cond) {
        auto target = SignExt(offset << 2, 18);
        branch_addr_ = pc_ + target;
        branch_ = true;
    }
}

bool CPU::NextCycle() {
    // fetch
    auto inst = bus_.ReadWord(pc_);
    auto opcode = inst >> 26;
    auto rs = (inst >> 21) & 0x1f;
    auto rt = (inst >> 16) & 0x1f;
    auto rd = (inst >> 11) & 0x1f;
    auto shamt = (inst >> 6) & 0x1f;
    auto funct = inst & 0x3f;
    auto imm = inst & 0xffff;
    auto signed_imm = SignExt(imm, 16);
    auto addr = inst & 0x3ffffff;
    // update program counter
    if (branch_) {
        pc_ = branch_addr_;
        branch_ = false;
    }
    else {
        pc_ += 4;
    }
    // decode & execute
    switch (opcode) {
        case kFunct: {
            switch (funct) {
                case kJR: DoJump(reg_[rs]); break;
                case kJALR: DoJump(reg_[rs], true, rd); break;
                case kMFHI: reg_[rd] = hi_; break;
                case kMFLO: reg_[rd] = lo_; break;
                case kMTHI: hi_ = reg_[rd]; break;
                case kMTLO: lo_ = reg_[rd]; break;
                case kBREAK: break;
                case kSYSCALL: break;
                default: {
                    if (!DoCalucation(opcode, funct, rs, rt, rd, shamt, imm)) {
                        return false;
                    }
                    break;
                }
            }
            break;
        }
        case kBranch: {
            switch (rt) {
                case kBGEZ: DoBranch(!(reg_[rs] & 0x80000000), imm); break;
                case kBLTZ: DoBranch(reg_[rs] & 0x80000000, imm); break;
                case kBGEZAL: DoBranch(!(reg_[rs] & 0x80000000), imm, true); break;
                case kBLTZAL: DoBranch(reg_[rs] & 0x80000000, imm, true); break;
                default: return false;
            }
            break;
        }
        case kPriority: {
            switch (rs) {
                case kERET: break;
                case kMFC0: reg_[rt] = cp0_[rd]; break;
                case kMTC0: cp0_[rd] = reg_[rt]; break;
                default: return false;
            }
            break;
        }
        case kBEQ: DoBranch(reg_[rs] == reg_[rt], imm); break;
        case kBNE: DoBranch(reg_[rs] != reg_[rt], imm); break;
        case kBGTZ: DoBranch(reg_[rs] & 0x7fffffff, imm); break;
        case kBLEZ: DoBranch(!(reg_[rs] & 0x7fffffff), imm); break;
        case kJ: DoJump((pc_ & 0xf0000000) | (addr << 2)); break;
        case kJAL: DoJump((pc_ & 0xf0000000) | (addr << 2), true); break;
        case kLB: {
            reg_[rt] = SignExt(bus_.ReadByte(reg_[rs] + signed_imm), 8);
            break;
        }
        case kLBU: reg_[rt] = bus_.ReadByte(reg_[rs] + signed_imm); break;
        case kLH: {
            reg_[rt] = SignExt(bus_.ReadHalf(reg_[rs] + signed_imm), 16);
            break;
        }
        case kLHU: reg_[rt] = bus_.ReadByte(reg_[rs] + signed_imm); break;
        case kLW: reg_[rt] = bus_.ReadWord(reg_[rs] + signed_imm); break;
        case kSB: bus_.WriteByte(reg_[rs] + signed_imm, reg_[rt]); break;
        case kSH: bus_.WriteHalf(reg_[rs] + signed_imm, reg_[rt]); break;
        case kSW: bus_.WriteWord(reg_[rs] + signed_imm, reg_[rt]); break;
        default: {
            if (!DoCalucation(opcode, funct, rs, rt, rd, shamt, imm)) {
                return false;
            }
            break;
        }
    }
    // $0 is read-only
    reg_[0] = 0;
    return true;
}
