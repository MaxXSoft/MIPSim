#ifndef MIPSIM_IO_ROM_H_
#define MIPSIM_IO_ROM_H_

#include <vector>
#include <string>

#include <io/io.h>

class ROM : public IOInterface {
public:
    ROM() {}

    bool LoadBinary(const std::string &filename);
    bool LoadHex(const std::string &filename);

    std::uint8_t ReadByte(std::uint32_t addr) override;
    void WriteByte(std::uint32_t addr, std::uint8_t value) override;
    std::uint16_t ReadHalf(std::uint32_t addr) override;
    void WriteHalf(std::uint32_t addr, std::uint16_t value) override;
    std::uint32_t ReadWord(std::uint32_t addr) override;
    void WriteWord(std::uint32_t addr, std::uint32_t value) override;

    std::size_t size() const { return rom_.size(); }

private:
    std::vector<std::uint8_t> rom_;
};

#endif // MIPSIM_IO_ROM_H_
