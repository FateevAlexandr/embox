/**
 * @file
 * @brief Fair policy test
 *
 * @author  Anna Kudryashova
 * @date    29.06.2017
 */

#include <pthread.h>
#include <embox/test.h>
#include <unistd.h>
#include <stdbool.h>

volatile bool flag;

EMBOX_TEST_SUITE("posix/scheduling policy");

static void *func(void *prt);

TEST_CASE("scheduling_with_two_threads") {
	flag = true;
	void *total_t1, *total_t2;
	int prt1 = 0, prt2 = 17;
	pthread_t t1, t2;

	test_assert_zero(pthread_create(&t1, 0, func, &prt1));
	test_assert_zero(pthread_create(&t2, 0, func, &prt2));

	sleep(2);
	flag = false;

	test_assert_zero(pthread_join(t1, &total_t1));
	test_assert_zero(pthread_join(t2, &total_t2));

	test_assert_true((int)total_t1/(int)total_t2 > prt2 - prt1);
}

static void *func(void *prt) {
	int priority = *((int *) prt);
	int cnt = 0;
	nice(priority);

	while (flag) {
		volatile int c = 1000;
		while (c) {
			c -= 1;
		}
		cnt += 1;
	}
	return (void *) cnt;
}
