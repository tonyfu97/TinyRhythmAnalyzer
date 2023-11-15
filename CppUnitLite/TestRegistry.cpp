#include "Test.h"
#include "TestResult.h"
#include "TestRegistry.h"
#include <iostream>

void TestRegistry::addTest (Test *test) 
{
    instance ().add (test);
}

void TestRegistry::runAllTests (TestResult& result) 
{
    instance ().run (result);
}

TestRegistry& TestRegistry::instance () {
    static TestRegistry registry;
    return registry;
}

void TestRegistry::add (Test *test) {
    tests.push_back (test);
}

void TestRegistry::run (TestResult& result) {
    auto testCount = 0;
    auto errorCount = 0;
    result.startTests ();

    for (auto& test : tests)
    {
        ++testCount;
        try {
            test->run (result);
        }
        catch (std::exception &e) {
            ++errorCount;
            std::cout << std::endl
                      << test->getFileName()
                      << "(" << test->getLineNumber() << ") : "
                      << "Error: exception "
                      << "'" << e.what() << "'"
                      << " thrown in "
                      << test->getName()
                      << std::endl;
        }
        catch (...) {
            ++errorCount;
            std::cout << std::endl
                      << test->getFileName()
                      << "(" << test->getLineNumber() << ") : "
                      << "Error: unknown exception thrown in "
                      << test->getName()
                      << std::endl;
        }
    }
    result.endTests ();
    const auto failureCount = result.getFailureCount();
    if (failureCount > 0 || errorCount > 0) std::cout << std::endl;
    std::cout << testCount << " tests, "
              << failureCount << " failures, "
              << errorCount << " errors" << std::endl;
}
