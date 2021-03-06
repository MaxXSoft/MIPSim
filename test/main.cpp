#include <test.h>

#define ALL_TESTS(f) \
    f(MemoryTest) f(BusTest) f(CPUTest0) f(CPUTest1) \
    f(CPUTest2) f(CPUTest3) f(CPUTest4)

// expand function declarations & unit test array
UNIT_TEST(ALL_TESTS, unit_test);

int main(int argc, const char *argv[]) {
    // run all unit tests
    for (const auto &i : unit_test) i();
    // display result & return
    Test::ShowResult();
    return Test::is_passed() ? 0 : 1;
}
