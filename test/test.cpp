#include <test.h>

#include <cstdio>

using namespace std;

int Test::test_count_ = 0;
int Test::pass_count_ = 0;

void Test::TestExpression(bool expr, const char *expr_str,
        uint64_t expect, uint64_t actual,
        const char *format, const char *file, int line) {
    ++test_count_;
    if (!expr) {
        char buf[50];
        fprintf(stderr, "%s:%d: in expression \"%s\": ",
                file, line, expr_str);
        sprintf(buf, "expect: %s, actual: %s\n", format, format);
        fprintf(stderr, buf, expect, actual);
    }
    else {
        ++pass_count_;
    }
}

void Test::ShowMessage(const char *msg) {
    printf("%s\n", msg);
}

void Test::ShowResult() {
    printf("%d/%d (%3.2f%%) passed\n",
            pass_count_, test_count_, pass_count_ * 100.0 / test_count_);
}
