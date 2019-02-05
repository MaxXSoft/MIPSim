#include <bus.h>

bool Bus::AddIO(std::uint32_t base, std::uint32_t size, const IOPtr &io) {
    // 'size' must be power of 2
    if ((size & -size) != size) return false;
    auto mask = ~(size - 1);
    // address space does not allow overlap
    for (const auto &i : ios_) {
        if (i.base >= base && i.base < base + size) return false;
        if (base >= i.base && base <= i.base + ~i.mask) return false;
    }
    // add io device
    ios_.push_back({base, mask, io});
    return true;
}

IOInterface *Bus::GetIO(std::uint32_t addr) {
    // TODO: optimize time complexity (<= O(logn))
    for (const auto &i : ios_) {
        if ((addr & i.mask) == i.base) {
            return i.io.get();
        }
    }
    return nullptr;
}

IOInterface *Bus::GetIO(std::uint32_t addr, std::uint32_t &io_addr) {
    // TODO: optimize time complexity (<= O(logn))
    for (const auto &i : ios_) {
        if ((addr & i.mask) == i.base) {
            io_addr = addr & ~i.mask;
            return i.io.get();
        }
    }
    return nullptr;
}

std::uint8_t Bus::ReadByte(std::uint32_t addr) {
    std::uint32_t io_addr;
    auto io = GetIO(addr, io_addr);
    return io ? io->ReadByte(io_addr) : 0;
}

void Bus::WriteByte(std::uint32_t addr, std::uint8_t value) {
    std::uint32_t io_addr;
    auto io = GetIO(addr, io_addr);
    if (io) io->WriteByte(io_addr, value);
}

std::uint16_t Bus::ReadHalf(std::uint32_t addr) {
    std::uint32_t io_addr;
    auto io = GetIO(addr, io_addr);
    return io ? io->ReadHalf(io_addr) : 0;
}

void Bus::WriteHalf(std::uint32_t addr, std::uint16_t value) {
    std::uint32_t io_addr;
    auto io = GetIO(addr, io_addr);
    if (io) io->WriteHalf(io_addr, value);
}

std::uint32_t Bus::ReadWord(std::uint32_t addr) {
    std::uint32_t io_addr;
    auto io = GetIO(addr, io_addr);
    return io ? io->ReadWord(io_addr) : 0;
}

void Bus::WriteWord(std::uint32_t addr, std::uint32_t value) {
    std::uint32_t io_addr;
    auto io = GetIO(addr, io_addr);
    if (io) io->WriteWord(io_addr, value);
}
