#include <io/rom.h>

#include <fstream>
#include <cctype>
#include <cassert>

namespace {

inline char HexConvert(char c) {
    assert(std::isxdigit(c));
    c = std::tolower(c);
    return c <= '9' ? c - '0' : c - 'a' + 10;
}

} // namespace

bool ROM::LoadBinary(const std::string &filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs.is_open()) return false;
    rom_.clear();
    // read bytes
    unsigned cur_byte = ifs.get();
    while (cur_byte != std::char_traits<unsigned char>::eof()) {
        rom_.push_back(cur_byte);
        cur_byte = ifs.get();
    }
    return true;
}

bool ROM::LoadHex(const std::string &filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) return false;
    rom_.clear();
    // read current hex
    std::string hex;
    while (ifs >> hex) {
        std::uint8_t cur_byte = HexConvert(hex[1]);
        cur_byte |= HexConvert(hex[0]) << 4;
        rom_.push_back(cur_byte);
    }
    return true;
}

std::uint8_t ROM::ReadByte(std::uint32_t addr) {
    return rom_[addr];
}

void ROM::WriteByte(std::uint32_t addr, std::uint8_t value) {
    // write word in ROM is not allowed
    assert(false);
}

std::uint16_t ROM::ReadHalf(std::uint32_t addr) {
    assert((addr & 1) == 0);
    std::uint16_t half = rom_[addr] | (rom_[addr + 1] << 8);
    return half;
}

void ROM::WriteHalf(std::uint32_t addr, std::uint16_t value) {
    // write word in ROM is not allowed
    assert(false);
}

std::uint32_t ROM::ReadWord(std::uint32_t addr) {
    assert((addr & 3) == 0);
    std::uint32_t word = rom_[addr] | (rom_[addr + 1] << 8);
    word |= (rom_[addr + 2] << 16) | (rom_[addr + 3] << 24);
    return word;
}

void ROM::WriteWord(std::uint32_t addr, std::uint32_t value) {
    // write word in ROM is not allowed
    assert(false);
}
