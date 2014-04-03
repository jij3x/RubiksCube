#include "unittest.h"
#include "rubiks_cube.h"
#include "cube_inspector.h"

static size_t passed, failed;

#define KRST  "\x1B[00m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

void test(testcase_t testcase, char *tc_name) {
    clock_t begin = clock();

    bool result = testcase();

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    if (result) {
        printf("%50s: %sPassed%s (%4.3fs)\n", tc_name, KGRN, KRST,
                time_spent);
        passed++;
    } else {
        printf("%50s: %sFailed%s (%4.3fs)\n", tc_name, KRED, KRST,
                time_spent);
        failed++;
    }
}

void print_stat(void) {
    if (passed + failed == 0) {
        printf("\nNo test case.\n");
        return;
    }

    char *passed_cases = (passed > 1 ? "test cases" : "test case");
    char *failed_cases = (failed > 1 ? "test cases" : "test case");

    printf("\n%zu %s passed.\n", passed, passed_cases);
    if (failed > 0) {
        printf("%zu %s failed!\n", failed, failed_cases);
    }
}
