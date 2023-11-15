#include <TestHarness.h>
#include "ElasticEnvelope.h"

TEST(ElasticEnvelope, AddValue)
{
    ElasticEnvelope envelope(0.1f);

    envelope.addValue(10.0f);
    CHECK_EQUAL(10.0f, envelope.getMax());
    CHECK_EQUAL(10.0f, envelope.getMin());
    CHECK_DOUBLES_EQUAL(10.0, envelope.getAverage(), 0.001);

    envelope.addValue(20.0f);
    CHECK_DOUBLES_EQUAL(19.5f, envelope.getMax(), 0.001);
    CHECK_DOUBLES_EQUAL(10.5f, envelope.getMin(), 0.001);
    CHECK_DOUBLES_EQUAL(15.0, envelope.getAverage(), 0.001);

    envelope.addValue(5.0f);
    CHECK(envelope.getMax() < 20.0f && envelope.getMax() > 12.5f);
    CHECK_DOUBLES_EQUAL(5.725f, envelope.getMin(), 0.001);
    CHECK_DOUBLES_EQUAL((envelope.getMax() + envelope.getMin()) / 2.0f, envelope.getAverage(), 0.001);
}

TEST(ElasticEnvelope, Decay)
{
    ElasticEnvelope envelope(0.1f); // Initialize with a decay rate of 0.1

    envelope.addValue(10.0f);
    envelope.addValue(20.0f);

    for (int i = 0; i < 5; ++i)
    {
        envelope.addValue(envelope.getAverage());
    }

    CHECK(envelope.getMax() < 20.0f);
    CHECK(envelope.getMin() > 10.0f);
    CHECK_DOUBLES_EQUAL((envelope.getMax() + envelope.getMin()) / 2.0f, envelope.getAverage(), 0.001);
}
