#ifndef MIPSIM_IO_MEM_H_
#define MIPSIM_IO_MEM_H_

#include <vector>
#include <cstddef>

#include <io/io.h>

class RAM : public IOInterface {
public:
    RAM() { ram_.resize(16384); }
    RAM(std::size_t size) { ram_.resize(size); }

    void Reset();

    std::uint8_t ReadByte(std::uint32_t addr) override;
    void WriteByte(std::uint32_t addr, std::uint8_t value) override;
    std::uint16_t ReadHalf(std::uint32_t addr) override;
    void WriteHalf(std::uint32_t addr, std::uint16_t value) override;
    std::uint32_t ReadWord(std::uint32_t addr) override;
    void WriteWord(std::uint32_t addr, std::uint32_t value) override;

    void set_size(std::size_t size) { ram_.resize(size); }
    std::size_t size() const { return ram_.size(); }

private:
    std::vector<std::uint8_t> ram_;
};

#endif // MIPSIM_IO_MEM_H_
