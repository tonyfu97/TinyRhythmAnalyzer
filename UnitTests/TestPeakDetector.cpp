#include <TestHarness.h>
#include <cmath>
#include <iostream>
#include "PeakDetector.h"
#include "ecg_data.h"

// Helper function to generate a sine wave
void generateSineWave(float *sineWave, int samples, float frequency, float sampleRate)
{
    for (int i = 0; i < samples; ++i)
    {
        sineWave[i] = sin(2.0 * M_PI * frequency * (i / sampleRate));
    }
}

TEST(PeakDetector, DetectSineWavePeaks)
{
    const int sampleRate = 100;                 // Samples per second
    const float frequency = 1.0;                // Frequency of the sine wave
    const int samples = sampleRate / frequency; // Total samples for one cycle
    float sineWave[samples];

    generateSineWave(sineWave, samples, frequency, sampleRate);

    // Initialize PeakDetector with a decay rate and hysteresis ratio
    PeakDetector<20> peakDetector(0.1f, 0.2f);
    int detectedPeaks = 0;

    // Feed the sine wave into the peak detector
    for (int i = 0; i < samples; ++i)
    {
        peakDetector.addSample(sineWave[i]);
        if (peakDetector.isPeakDetected())
        {
            detectedPeaks++;
            peakDetector.clearPeakFlag(); // Reset after detecting a peak
        }
    }

    // Since it's a sine wave of frequency 1Hz, we expect to detect 1 peak in 1 second
    CHECK_EQUAL(1, detectedPeaks);
}

TEST(PeakDetector, DetectMultipleSineWavePeaks)
{
    const int sampleRate = 100;                          // Samples per second
    const float frequency = 1.0;                         // Frequency of the sine wave
    const int cycles = 5;                                // Total number of cycles to test
    const int samples = cycles * sampleRate / frequency; // Total samples for multiple cycles
    float sineWave[samples];

    generateSineWave(sineWave, samples, frequency, sampleRate);

    PeakDetector<20> peakDetector(0.1f, 0.2f);
    int detectedPeaks = 0;

    for (int i = 0; i < samples; ++i)
    {
        peakDetector.addSample(sineWave[i]);
        if (peakDetector.isPeakDetected())
        {
            detectedPeaks++;
            peakDetector.clearPeakFlag();
        }
    }

    // Expect to detect 5 peaks over 5 cycles
    CHECK_EQUAL(cycles, detectedPeaks);
}

TEST(PeakDetector, NoFalsePositivesWithFlatSignal)
{
    const int samples = 100;
    float flatSignal[samples] = {0.0f};

    PeakDetector<20> peakDetector(0.1f, 0.2f);
    bool falsePositiveDetected = false;

    for (int i = 0; i < samples; ++i)
    {
        peakDetector.addSample(flatSignal[i]);
        if (peakDetector.isPeakDetected())
        {
            falsePositiveDetected = true;
            peakDetector.clearPeakFlag();
        }
    }

    CHECK(!falsePositiveDetected);
}

static int runPeakDetectionTest(float ecg_signal[], const int ecg_signal_length, int verbose = 0)
{
    PeakDetector<5> peakDetector(0.001f, 0.2f);
    int detectedPeaks = 0;

    for (int i = 0; i < ecg_signal_length; ++i)
    {
        peakDetector.addSample(ecg_signal[i]);
        if (peakDetector.isPeakDetected())
        {
            detectedPeaks++;
            peakDetector.clearPeakFlag();
            if (verbose)
            {
                std::cout << "Peak detected at sample " << i << "\n";
                std::cout << "UpperThreshold: " << peakDetector.getUpperThreshold() << "\n";
                std::cout << "LowerThreshold: " << peakDetector.getLowerThreshold() << "\n";
                std::cout << std::endl;
            }
        }
    }
    return detectedPeaks;
}

TEST(PeakDetector, DetectECGPeaks101)
{
    extern float ecg_signal_101[];
    extern const int ecg_signal_101_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_101, ecg_signal_101_length);

    // The first 2000 samples of patient 101 should have 6 or 7 peaks (the first
    // P wave might be misidentified as a peak)
    CHECK(6 == detectedPeaks || 7 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks102)
{
    extern float ecg_signal_102[];
    extern const int ecg_signal_102_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_102, ecg_signal_102_length);

    // The first 2000 samples of patient 102 should have around 7 peaks (the first
    // P wave and noises might be misidentified as peaks)
    CHECK(7 == detectedPeaks || 8 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks103)
{
    extern float ecg_signal_103[];
    extern const int ecg_signal_103_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_103, ecg_signal_103_length);

    CHECK(7 == detectedPeaks || 8 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks104)
{
    extern float ecg_signal_104[];
    extern const int ecg_signal_104_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_104, ecg_signal_104_length);

    CHECK(7 == detectedPeaks || 8 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks105)
{
    extern float ecg_signal_105[];
    extern const int ecg_signal_105_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_105, ecg_signal_105_length);

    CHECK(7 == detectedPeaks || 8 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks106)
{
    extern float ecg_signal_106[];
    extern const int ecg_signal_106_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_106, ecg_signal_106_length);

    CHECK(5 == detectedPeaks || 6 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks119)
{
    extern float ecg_signal_119[];
    extern const int ecg_signal_119_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_119, ecg_signal_119_length);

    CHECK(6 == detectedPeaks || 7 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks200)
{
    extern float ecg_signal_200[];
    extern const int ecg_signal_200_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_200, ecg_signal_200_length);

    CHECK(4 == detectedPeaks || 5 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks208)
{
    extern float ecg_signal_208[];
    extern const int ecg_signal_208_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_208, ecg_signal_208_length);

    CHECK(9 == detectedPeaks);
}

TEST(PeakDetector, DetectECGPeaks217)
{
    extern float ecg_signal_217[];
    extern const int ecg_signal_217_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_217, ecg_signal_217_length);

    // Patient 217 has 6 peaks but also very prominent T waves, so we expect
    // to detect more than 6 peaks
    CHECK(detectedPeaks > 6 || detectedPeaks < 12);
}

TEST(PeakDetector, DetectECGPeaks233)
{
    extern float ecg_signal_233[];
    extern const int ecg_signal_233_length;

    int detectedPeaks = runPeakDetectionTest(ecg_signal_233, ecg_signal_233_length);

    CHECK(8 == detectedPeaks || 9 == detectedPeaks);
}
