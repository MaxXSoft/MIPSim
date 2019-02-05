#ifndef MPISIM_TEST_TEST_H_
#define MPISIM_TEST_TEST_H_

// a simple unit test framework by MaxXing

#include <type_traits>
#include <cstdint>

// class definition
class Test {
public:
    static void TestExpression(bool expr, const char *expr_str,
            std::uint64_t expect, std::uint64_t actual,
            const char *format, const char *file, int line);
    static void ShowMessage(const char *msg);
    static void ShowResult();

    static int test_count() { return test_count_; }
    static int pass_count() { return pass_count_; }
    static bool is_passed() { return test_count_ == pass_count_; }

private:
    static int test_count_;
    static int pass_count_;
};

// helper macros & functions
template<typename T>
constexpr const char *GetFormat(T) { return nullptr; }
template <> constexpr const char *GetFormat(char) { return "%hhd"; }
template <> constexpr const char *GetFormat(unsigned char) { return "%hhu"; }
template <> constexpr const char *GetFormat(short) { return "%hd"; }
template <> constexpr const char *GetFormat(unsigned short) { return "%hu"; }
template <> constexpr const char *GetFormat(int) { return "%d"; }
template <> constexpr const char *GetFormat(unsigned) { return "%u"; }
template <> constexpr const char *GetFormat(long) { return "%ld"; }
template <> constexpr const char *GetFormat(unsigned long) { return "%lu"; }
template <> constexpr const char *GetFormat(long long) { return "%lld"; }
template <> constexpr const char *GetFormat(unsigned long long) { return "%llu"; }
template <> constexpr const char *GetFormat(float) { return "%f"; }
template <> constexpr const char *GetFormat(double) { return "%lf"; }
template <> constexpr const char *GetFormat(long double) { return "%Lf"; }
template <> constexpr const char *GetFormat(void *) { return "%p"; }
template <> constexpr const char *GetFormat(char *) { return "%s"; }
template <> constexpr const char *GetFormat(const char *) { return "%s"; }
template <> constexpr const char *GetFormat(bool) { return "%d"; }

#define TEST_EXPECT(expect, actual) \
    do { \
        auto temp = (actual);\
        Test::TestExpression((decltype(temp)(expect)) == temp, \
                "(" #expect ") == (" #actual ")", expect, \
                temp, GetFormat(temp), __FILE__, __LINE__); \
    } while (0)
#define UNIT_TEST_DECLARE(name) void name();
#define UNIT_TEST_ARRAY(name) name,
#define UNIT_TEST(tests, arr) \
    tests(UNIT_TEST_DECLARE) \
    void (*arr[])() = {tests(UNIT_TEST_ARRAY)}

#endif // MPISIM_TEST_TEST_H_
