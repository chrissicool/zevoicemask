#include <running_avg.h>
#include <unity.h>

/*
 * Test operator+()
 */
void test_running_avg_plus()
{
  enum { Elements = 10, };
  typedef Running_avg<Elements, int> Avg;
  Avg p1, p2;

  p1 = p2 = 1;
  TEST_ASSERT_TRUE(!p1.primed());
  TEST_ASSERT_TRUE(!p2.primed());

  /* warmup */
  for (int i = 0; i < Elements - 2; i++) {
    p1 = p1 + Avg(1);
    TEST_ASSERT_TRUE(!p1.primed());
    TEST_ASSERT_EQUAL(1, p1);
  }

  /* final prime */
  p1 = p1 + Avg(1);
  TEST_ASSERT_TRUE(p1.primed());
  TEST_ASSERT_EQUAL(1, p1);

  p2 = p1;
  TEST_ASSERT_TRUE(p2.primed());
  TEST_ASSERT_EQUAL(1, p2);

  p2 = 1;
  TEST_ASSERT_TRUE(!p2.primed());
  p2 = p1 + p2;
  TEST_ASSERT_TRUE(p2.primed());
  TEST_ASSERT_EQUAL(1, p2);

  p2 = 1;
  TEST_ASSERT_TRUE(!p2.primed());
  p2 = p1 + p2 + Avg(1);
  TEST_ASSERT_TRUE(p2.primed());
  TEST_ASSERT_EQUAL(1, p2);

  p1 = 0;
  p2 = 10;
  p2 = p1 + p2 + Avg(10);
  TEST_ASSERT_EQUAL(7, p2);
}

/*
 * Simple test with big steps up/down.
 */
static void single_sign(Running_avg<1, int> &single, int sign)
{
  single = 100 * sign;
  TEST_ASSERT_EQUAL(100 * sign, single);
  TEST_ASSERT_TRUE(single.primed());

  single += 1 * sign;
  TEST_ASSERT_EQUAL(51 * sign, single);
  single += 1 * sign;
  TEST_ASSERT_EQUAL(26 * sign, single);
  single += 1 * sign;
  TEST_ASSERT_EQUAL(13 * sign, single);
  single += 1 * sign;
  TEST_ASSERT_EQUAL(7 * sign, single);
  single += 1 * sign;
  TEST_ASSERT_EQUAL(4 * sign, single);
  single += 1 * sign;
  TEST_ASSERT_EQUAL(3 * sign, single);
  single += 1 * sign;
  TEST_ASSERT_EQUAL(2 * sign, single);
  single += 1 * sign;
  TEST_ASSERT_EQUAL(1 * sign, single);
  single += 1 * sign;
  TEST_ASSERT_EQUAL(1 * sign, single);

  single += 100 * sign;
  TEST_ASSERT_EQUAL(51 * sign, single);
  single += 100 * sign;
  TEST_ASSERT_EQUAL(75 * sign, single);
  single += 100 * sign;
  TEST_ASSERT_EQUAL(88 * sign, single);
  single += 100 * sign;
  TEST_ASSERT_EQUAL(94 * sign, single);
  single += 100 * sign;
  TEST_ASSERT_EQUAL(97 * sign, single);
  single += 100 * sign;
  TEST_ASSERT_EQUAL(98 * sign, single);
  single += 100 * sign;
  TEST_ASSERT_EQUAL(99 * sign, single);
  single += 100 * sign;
  TEST_ASSERT_EQUAL(100 * sign, single);
  single += 100 * sign;
  TEST_ASSERT_EQUAL(100 * sign, single);
}

void test_running_avg_single()
{
  Running_avg<1, int> single;

  single_sign(single, 1);
  single_sign(single, -1);
}

/*
 * Test gradually sloping up the average.
 */
void test_running_avg_slope()
{
  enum { Elements = 1000, };
  Running_avg<Elements, int> slope;

  TEST_ASSERT_TRUE(!slope.primed());

  /* prime with undefined initial value */
  for (unsigned i = 0 ; i < Elements * 2; i++)
    slope += 1000;
  TEST_ASSERT_EQUAL(1000, slope);
  TEST_ASSERT_TRUE(slope.primed());

  /* inch up */
  for (unsigned i = 0 ; i < Elements * 2; i++)
    slope += 1001;
  TEST_ASSERT_EQUAL(1001, slope);

  /* inch down */
  for (unsigned i = 0 ; i < Elements * 2; i++)
    slope += 1000;
  TEST_ASSERT_EQUAL(1000, slope);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_running_avg_plus);
  RUN_TEST(test_running_avg_single);
  RUN_TEST(test_running_avg_slope);
  UNITY_END();

  return 0;
}
