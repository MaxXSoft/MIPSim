#ifndef MIPSIM_IO_IO_H_
#define MIPSIM_IO_IO_H_

#include <memory>
#include <cstdint>

class IOInterface {
public:
    virtual ~IOInterface() = default;
    virtual std::uint8_t ReadByte(std::uint32_t addr) = 0;
    virtual void WriteByte(std::uint32_t addr, std::uint8_t value) = 0;
    virtual std::uint16_t ReadHalf(std::uint32_t addr) = 0;
    virtual void WriteHalf(std::uint32_t addr, std::uint16_t value) = 0;
    virtual std::uint32_t ReadWord(std::uint32_t addr) = 0;
    virtual void WriteWord(std::uint32_t addr, std::uint32_t value) = 0;
};

using IOPtr = std::shared_ptr<IOInterface>;

#endif // MIPSIM_IO_IO_H_
