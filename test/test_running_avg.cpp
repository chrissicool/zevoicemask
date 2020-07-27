#include <running_avg.h>
#include <unity.h>

static Running_avg<1, unsigned> single;

void test_running_avg_single()
{
  single = 100;
  TEST_ASSERT_EQUAL(100, single);

  single += 1;
  TEST_ASSERT_EQUAL(50, single);
  single += 1;
  TEST_ASSERT_EQUAL(25, single);
  single += 1;
  TEST_ASSERT_EQUAL(13, single);
  single += 1;
  TEST_ASSERT_EQUAL(7, single);
  single += 1;
  TEST_ASSERT_EQUAL(4, single);
  single += 1;
  TEST_ASSERT_EQUAL(2, single);
  single += 1;
  TEST_ASSERT_EQUAL(1, single);
  single += 1;
  TEST_ASSERT_EQUAL(1, single);

  single += 100;
  TEST_ASSERT_EQUAL(51, single);
  single += 100;
  TEST_ASSERT_EQUAL(76, single);
  single += 100;
  TEST_ASSERT_EQUAL(88, single);
  single += 100;
  TEST_ASSERT_EQUAL(94, single);
  single += 100;
  TEST_ASSERT_EQUAL(97, single);
  single += 100;
  TEST_ASSERT_EQUAL(99, single);
  single += 100;
  TEST_ASSERT_EQUAL(100, single);
  single += 100;
  TEST_ASSERT_EQUAL(100, single);
}

int main(int argc, char **argv)
{
  UNITY_BEGIN();
  RUN_TEST(test_running_avg_single);
  UNITY_END();

  return 0;
}
