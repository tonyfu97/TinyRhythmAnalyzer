#include <TestHarness.h>
#include "CircularBuffer.h"

TEST(CircularBuffer, CreateEmptyBuffer)
{
    CircularBuffer<int, 5> buffer;

    CHECK(buffer.isEmpty());
    CHECK(!buffer.isFilled());
    CHECK_EQUAL(0, buffer.size());
}

TEST(CircularBuffer, AddItems)
{
    CircularBuffer<int, 5> buffer;
    buffer.push(1);
    buffer.push(2);

    CHECK(!buffer.isEmpty());
    CHECK(!buffer.isFilled());
    CHECK_EQUAL(2, buffer.size());
    CHECK_EQUAL(1, buffer[0]);
    CHECK_EQUAL(2, buffer[1]);
}

TEST(CircularBuffer, FillBuffer)
{
    CircularBuffer<int, 5> buffer;
    for (int i = 0; i < 5; ++i)
    {
        buffer.push(i + 1);
    }

    CHECK(buffer.isFilled());
    CHECK_EQUAL(5, buffer.size());
    for (int i = 0; i < 5; ++i)
    {
        CHECK_EQUAL(i + 1, buffer[i]);
    }
}

TEST(CircularBuffer, OverwriteBuffer)
{
    CircularBuffer<int, 5> buffer;
    for (int i = 0; i < 5; ++i)
    {
        buffer.push(i + 1);
    }
    buffer.push(6);

    CHECK(buffer.isFilled());
    CHECK_EQUAL(5, buffer.size());
    CHECK_EQUAL(2, buffer[0]);
    CHECK_EQUAL(3, buffer[1]);
    CHECK_EQUAL(4, buffer[2]);
    CHECK_EQUAL(5, buffer[3]);
    CHECK_EQUAL(6, buffer[4]);
}

TEST(CircularBuffer, ClearBuffer)
{
    CircularBuffer<int, 5> buffer;
    for (int i = 0; i < 5; ++i)
    {
        buffer.push(i + 1);
    }
    buffer.clear();

    CHECK(buffer.isEmpty());
    CHECK_EQUAL(0, buffer.size());
}
