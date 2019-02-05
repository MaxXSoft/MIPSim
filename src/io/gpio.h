#ifndef MIPSIM_IO_GPIO_H_
#define MIPSIM_IO_GPIO_H_

#include <io/io.h>

class GPIO : public IOInterface {
public:
    GPIO() : halt_(false) {}

    std::uint8_t ReadByte(std::uint32_t addr) override;
    void WriteByte(std::uint32_t addr, std::uint8_t value) override;
    std::uint16_t ReadHalf(std::uint32_t addr) override;
    void WriteHalf(std::uint32_t addr, std::uint16_t value) override;
    std::uint32_t ReadWord(std::uint32_t addr) override;
    void WriteWord(std::uint32_t addr, std::uint32_t value) override;

    bool halt() const { return halt_; }

private:
    bool halt_;
};

#endif // MIPSIM_IO_GPIO_H_
