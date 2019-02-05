#ifndef MIPSIM_TEST_UNIT_UTIL_H_
#define MIPSIM_TEST_UNIT_UTIL_H_

#include <fstream>
#include <iomanip>

const auto kTempFile = "/tmp/mipsim_test.tmp";

template <typename T>
void ExportToBinary(const char *filename, T begin, T end) {
    std::ofstream ofs(filename, std::ios::binary);
    for (auto it = begin; it != end; ++it) {
        ofs.write(reinterpret_cast<const char *>(it), sizeof(*it));
    }
}

template <typename T>
void ExportToHex(const char *filename, T begin, T end) {
    std::ofstream ofs(filename);
    int count = 0;
    for (auto it = begin; it != end; ++it) {
        ofs << std::setw(2) << std::setfill('0');
        ofs << std::hex << static_cast<int>(*it);
        ofs << (++count % 4 != 0 ? ' ' : '\n');
    }
}

#endif // MIPSIM_TEST_UNIT_UTIL_H_
