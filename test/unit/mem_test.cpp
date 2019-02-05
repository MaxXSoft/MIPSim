#include <test.h>

#include <io/rom.h>
#include <io/ram.h>

#include <unit/util.h>

using namespace std;

namespace {

uint8_t bytes[] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

} // namespace

void MemoryTest() {
    // rom test
    ROM rom;
    ExportToBinary(kTempFile, bytes, bytes + 8);
    rom.LoadBinary(kTempFile);
    TEST_EXPECT(0x02, rom.ReadByte(1));
    TEST_EXPECT(0x06, rom.ReadByte(5));
    TEST_EXPECT(0x0403, rom.ReadHalf(2));
    TEST_EXPECT(0x0807, rom.ReadHalf(6));
    TEST_EXPECT(0x04030201, rom.ReadWord(0));
    TEST_EXPECT(0x08070605, rom.ReadWord(4));
    ExportToHex(kTempFile, bytes, bytes + 8);
    rom.LoadHex(kTempFile);
    TEST_EXPECT(0x01, rom.ReadByte(0));
    TEST_EXPECT(0x05, rom.ReadByte(4));
    TEST_EXPECT(0x0201, rom.ReadHalf(0));
    TEST_EXPECT(0x0605, rom.ReadHalf(4));
    TEST_EXPECT(0x04030201, rom.ReadWord(0));
    TEST_EXPECT(0x08070605, rom.ReadWord(4));
    // ram test
    RAM ram;
    ram.Reset();
    TEST_EXPECT(0, ram.ReadWord(0));
    ram.WriteWord(0, 0x01020304);
    ram.WriteWord(4, 0x05060708);
    TEST_EXPECT(0x04, ram.ReadByte(0));
    TEST_EXPECT(0x08, ram.ReadByte(4));
    TEST_EXPECT(0x0304, ram.ReadHalf(0));
    TEST_EXPECT(0x0708, ram.ReadHalf(4));
    TEST_EXPECT(0x01020304, ram.ReadWord(0));
    TEST_EXPECT(0x05060708, ram.ReadWord(4));
}
