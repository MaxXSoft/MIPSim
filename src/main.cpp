#include <cpu.h>
#include <bus.h>
#include <io/ram.h>
#include <io/rom.h>
#include <io/gpio.h>
#include <io/vga.h>

#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

bool EndWith(const char *src, const char *dst) {
    int len1 = strlen(src), len2 = strlen(dst);
    if (len1 < len2) return false;
    src += len1 - len2;
    while (*src) {
        if (toupper(*src++) != toupper(*dst++)) return false;
    }
    return true;
}

int main(int argc, const char *argv[]) {
    Bus bus;
    CPU cpu(bus);
    // create & add devices
    auto ram = make_shared<RAM>(0x8000);
    auto rom = make_shared<ROM>();
    auto gpio = make_shared<GPIO>();
    auto vga = make_shared<VGA>();
    bus.AddIO(0x00000000, 0x8000, ram);
    bus.AddIO(0xbfc00000, 0x100000, rom);
    bus.AddIO(0xbfd05000, 0x1000, gpio);
    bus.AddIO(0x85000000, 0x200000, vga);
    // load programs
    bool loaded = false;
    if (argc != 2) {
        cout << "no program file input" << endl;
        cout << "usage: mipsim [file.bin|file.hex]" << endl;
        return 1;
    }
    if (EndWith(argv[1], ".bin")) {
        loaded = rom->LoadBinary(argv[1]);
    }
    else if (EndWith(argv[1], ".hex")) {
        loaded = rom->LoadHex(argv[1]);
    }
    if (!loaded) {
        cout << "invalid input file" << endl;
        return 1;
    }
    // start simulation
    uint64_t cycle = 0;
    bool vga_running = true;
    while (!gpio->halt()) {
        ++cycle;
        if (!cpu.NextCycle()) {
            cout << endl <<  "error: illegal instruction, ";
            cout << "cycle = " << cycle << endl;
            return 1;
        }
        // VGA refresh
        if ((cycle & 0xffff) == 0) vga_running = vga->Refresh();
    }
    cout << endl << "info: simulation ended, cycle = " << cycle << endl;
    while (vga_running) vga_running = vga->Refresh();
    return 0;
}
