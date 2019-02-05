#include <test.h>

#include <bus.h>
#include <io/rom.h>
#include <io/ram.h>

#include <unit/util.h>

using namespace std;

namespace {

uint8_t bytes[] = {
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
};

} // namespace

void BusTest() {
    // initialize rom
    ExportToBinary(kTempFile, bytes, bytes + 8);
    auto rom = make_shared<ROM>();
    rom->LoadBinary(kTempFile);
    // test bus
    Bus bus;
    TEST_EXPECT(true, bus.AddIO(0xbfc00000, 1024, rom));
    TEST_EXPECT(true, bus.AddIO(0x00000000, 16384, make_shared<RAM>()));
    TEST_EXPECT(false, bus.AddIO(0xbfc10000, 1000, rom));
    TEST_EXPECT(false, bus.AddIO(0xbfc00100, 1024, rom));
    TEST_EXPECT(false, bus.AddIO(0xbfbfff00, 1024, rom));
    bus.WriteWord(0x00000000, 0xdeadbeef);
    bus.WriteWord(0x00000004, 0xcafebabe);
    TEST_EXPECT(0x11, bus.ReadByte(0xbfc00000));
    TEST_EXPECT(0x55, bus.ReadByte(0xbfc00004));
    TEST_EXPECT(0x2211, bus.ReadHalf(0xbfc00000));
    TEST_EXPECT(0x6655, bus.ReadHalf(0xbfc00004));
    TEST_EXPECT(0x44332211, bus.ReadWord(0xbfc00000));
    TEST_EXPECT(0x88776655, bus.ReadWord(0xbfc00004));
    TEST_EXPECT(0xef, bus.ReadByte(0x00000000));
    TEST_EXPECT(0xbe, bus.ReadByte(0x00000004));
    TEST_EXPECT(0xbeef, bus.ReadHalf(0x00000000));
    TEST_EXPECT(0xbabe, bus.ReadHalf(0x00000004));
    TEST_EXPECT(0xdeadbeef, bus.ReadWord(0x00000000));
    TEST_EXPECT(0xcafebabe, bus.ReadWord(0x00000004));
}
