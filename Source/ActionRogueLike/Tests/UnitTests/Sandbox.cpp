#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/UnitTests/TestsHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "Project.UnitTest.Math.MaxInt",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "Project.UnitTest.Math.Sqrt",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxIntImprove, "Project.UnitTest.Math.MaxIntImprove",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrtImprove, "Project.UnitTest.Math.SqrtImprove",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, "Project.UnitTest.Math.Sin",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)


bool FMathMaxInt::RunTest(const FString& Parameters)
{
	AddInfo("Max [int] func testing");

	TestTrue("2 different positive numbers", FMath::Max(13, 23) == 23);
	TestEqual("2 equal positive numbers", FMath::Max(25, 25), 25);
	TestTrueExpr(FMath::Max(0, 123) == 123);
	TestTrue("2 zeroes", FMath::Max(0, 0) == 0);
	TestTrue("Negative numbers and zero", FMath::Max(-2345, 0) == 0);
	TestTrue("2 different negative numbers", FMath::Max(-45, -67) == -45);
	TestTrue("2 equal negative numbers", FMath::Max(-9, -9) == -9);


	if (1 != 2)
	{
		AddWarning("1 not equal 2");
		return true;
	}

	//return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
	AddInfo("Sqrt function testing");

	TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.0f), 2.0f);
	TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.0f), 1.7f, 0.1f);
	TestEqual("Sqrt(3) [2]", FMath::Sqrt(3.0f), 1.73205f, 1.e-5f);


	return true;
}

bool FMathMaxIntImprove::RunTest(const FString& Parameters)
{
	AddInfo("Max [int] func testing");

	struct TestPayLoad
	{
		TInterval<int32> TestValue;
		int32 ExpectedValue;
	};

	typedef TArray<SGame::TestPayLoad<TInterval<int32>, int32>> MaxIntTestPayLoad;

	//const TArray<SGame::TestPayLoad<TInterval<int32>, int32>> TestData
	const MaxIntTestPayLoad TestData
	{
		{{13, 23}, 23},
		{{25, 25}, 25},
		{{0, 123}, 123},
		{{0, 0}, 0},
		{{-2345, 0}, 0},
		{{-45, -67}, -45},
		{{-9, -9}, -9},
	};

	for (const SGame::TestPayLoad Data : TestData)
	{
		TestTrueExpr(FMath::Max(Data.TestValue.Min, Data.TestValue.Max) == Data.ExpectedValue);

		//const FString InfoString = FString::Printf(
		//	TEXT("Test value: min %i, max %i,  Expected value: %i"), Data.TestValue.Min, Data.TestValue.Max, Data.ExpectedValue);
		//TestEqual(InfoString, FMath::Max(Data.TestValue.Min, Data.TestValue.Max), Data.ExpectedValue);
	}

	return true;
}

bool FMathSqrtImprove::RunTest(const FString& Parameters)
{
	AddInfo("Sqrt function testing");

	typedef TArray<SGame::TestPayLoad<float, float>> SqrtTestPayLoad;
	//clang-format off
	const SqrtTestPayLoad TestData
	{
		{4.0f, 2.0f},
		{3.0f, 1.7f, 0.1f},
		{3.0f, 1.73205f, 1.e-5f}
	};
	//clang-format on

	for (SGame::TestPayLoad Data : TestData)
	{
		const bool IsEqual = FMath::IsNearlyEqual(FMath::Sqrt(Data.TestValue), Data.ExpectedValue, Data.Tolerance);
		TestTrueExpr(IsEqual);
	}

	return true;
}

bool FMathSin::RunTest(const FString& Parameters)
{
	AddInfo("Sin func testing");

	typedef float Degrees;
	typedef TArray<SGame::TestPayLoad<Degrees, float>> SinTestPayLoad;

	//clang-format off
	const SinTestPayLoad TestData
	{
		{Degrees{0.00f}, 0.0f},
		{Degrees{30.0f}, 0.5f},
		{Degrees{45.0f}, 0.707f},
		{Degrees{60.0f}, 0.866f},
		{Degrees{90.0f}, 1.0f},
	};
	//clang-format on

	for (auto Data : TestData)
	{
		const float Rad = FMath::DegreesToRadians(Data.TestValue);
		TestTrueExpr(FMath::IsNearlyEqual(FMath::Sin(Rad), Data.ExpectedValue, 0.001f));
	}

	return true;
}


#endif
