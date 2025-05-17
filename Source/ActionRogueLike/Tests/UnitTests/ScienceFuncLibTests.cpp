#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Science/ScienceFunctLib.h"
#include "Tests/UnitTests/TestsHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "Project.UnitTest.Fibonacci.Simple",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "Project.UnitTest.Fibonacci.Stress",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasError, "Project.UnitTest.Fibonacci.LogHasError",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ProductFilter)


bool FFibonacciSimple::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci Simple testing");

	// 0 1 1 2 3 5 8 ...

	//TestTrueExpr(UScienceFunctLib::Fibonacci(0) == 0);
	//TestTrueExpr(UScienceFunctLib::Fibonacci(1) == 1);
	//TestTrueExpr(UScienceFunctLib::Fibonacci(2) == 1);
	//TestTrueExpr(UScienceFunctLib::Fibonacci(3) == 2);
	
	const TArray<SGame::TestPayLoad<int, int>> TestData
	{
		{0, 0},
		{1, 1},
		{2, 1},
		{3, 2},
		{4, 3},
		{5, 5}
	};

	for (const SGame::TestPayLoad Data : TestData)
	{
		// TestTrueExpr(UScienceFunctLib::Fibonacci(Data.TestValue) == Data.ExpectedValue);

		const FString InfoString = FString::Printf(TEXT("Test value: %i, Expected value: %i"), Data.TestValue, Data.ExpectedValue);
		TestEqual(InfoString, UScienceFunctLib::Fibonacci(Data.TestValue), Data.ExpectedValue);
	}

	return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci Stress testing");

	//for (int32 i = 2; i < 40; ++i)
	//{
	//	TestTrueExpr(UScienceFunctLib::Fibonacci(i) == UScienceFunctLib::Fibonacci(i -1 ) + UScienceFunctLib::Fibonacci(i - 2));
	//}

	int32 PrevValue = 1;
	int32 PrevPrevValue = 0;
	for (int32 i = 2; i < 40; ++i)
	{
		const int32 CurrentValue = UScienceFunctLib::Fibonacci(i);
		TestTrueExpr(CurrentValue == PrevPrevValue + PrevValue);

		PrevPrevValue = PrevValue;
		PrevValue = CurrentValue;
	}

	return true;
}

bool FFibonacciLogHasError::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci negative number on input produces error");

	AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::Contains);
	UScienceFunctLib::Fibonacci(-10);
	return true;
}


#endif
