#ifndef MIPSIM_BUS_H_
#define MIPSIM_BUS_H_

#include <vector>
#include <cstdint>

#include <io/io.h>

class Bus {
public:
    Bus() {}

    bool AddIO(std::uint32_t base, std::uint32_t size, const IOPtr &io);
    IOInterface *GetIO(std::uint32_t addr);
    IOInterface *GetIO(std::uint32_t addr, std::uint32_t &io_addr);

    std::uint8_t ReadByte(std::uint32_t addr);
    void WriteByte(std::uint32_t addr, std::uint8_t value);
    std::uint16_t ReadHalf(std::uint32_t addr);
    void WriteHalf(std::uint32_t addr, std::uint16_t value);
    std::uint32_t ReadWord(std::uint32_t addr);
    void WriteWord(std::uint32_t addr, std::uint32_t value);

private:
    struct IOItem {
        std::uint32_t base, mask;
        IOPtr io;
    };
    std::vector<IOItem> ios_;
};

#endif // MIPSIM_BUS_H_
