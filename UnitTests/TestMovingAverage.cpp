#include <TestHarness.h>
#include <cmath>
#include "MovingAverage.h"

TEST(MovingAverageTest, IntegerBuffer)
{
    MovingAverage<int, int, 5> avg;

    CHECK(!avg.isBufferFilled());
    CHECK(std::isnan(avg.getAverage()));

    avg.addValue(1);
    CHECK_DOUBLES_EQUAL(1, avg.getAverage(), 0.001);

    avg.addValue(2);
    CHECK_DOUBLES_EQUAL(1.5, avg.getAverage(), 0.001);

    avg.addValue(3);
    avg.addValue(4);
    avg.addValue(5); // Buffer is now full.
    CHECK(avg.isBufferFilled());
    CHECK_DOUBLES_EQUAL(3, avg.getAverage(), 0.001);

    avg.addValue(6);
    CHECK_DOUBLES_EQUAL((2 + 3 + 4 + 5 + 6) / 5.0, avg.getAverage(), 0.001);
}

TEST(MovingAverageTest, FloatBuffer)
{
    MovingAverage<float, float, 3> avg;

    CHECK(!avg.isBufferFilled());
    CHECK(std::isnan(avg.getAverage()));

    avg.addValue(1.0f);
    CHECK_DOUBLES_EQUAL(1.0, avg.getAverage(), 0.0001);

    avg.addValue(2.0f);
    CHECK_DOUBLES_EQUAL(1.5, avg.getAverage(), 0.0001);

    avg.addValue(3.0f); // Buffer is now full.
    CHECK(avg.isBufferFilled());
    CHECK_DOUBLES_EQUAL(2.0, avg.getAverage(), 0.0001);

    avg.addValue(-9.0f);
    CHECK_DOUBLES_EQUAL((2.0f + 3.0f - 9.0f) / 3.0, avg.getAverage(), 0.0001);
}
