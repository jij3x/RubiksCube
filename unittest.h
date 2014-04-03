#ifndef UNITTEST_H_
#define UNITTEST_H_

#include "globe_def.h"

typedef bool (*testcase_t)(void);

void test(testcase_t testcase, char *tc_name);
void print_stat(void);

#endif /* UNITTEST_H_ */
