#pragma once

#include "CoreMinimal.h"

namespace ARL
{
	class ACTIONROGUELIKE_API Battery
	{
	public:
		Battery() = default;
		Battery(float PercentIn);

		void Charge();
		void UnCharge();

		float GetPercent() const;
		FColor GetColor() const;
		FString ToString() const;

		bool operator>=(const Battery& rhs) const { return GetPercent() >= rhs.GetPercent(); }
		bool operator<=(const Battery& rhs) const { return GetPercent() <= rhs.GetPercent(); }
		bool operator==(const Battery& rhs) const { return FMath::IsNearlyEqual(GetPercent(), rhs.GetPercent()); }
		bool operator<(const Battery& rhs) const { return GetPercent() < rhs.GetPercent(); };
		bool operator>(const Battery& rhs) const { return GetPercent() > rhs.GetPercent(); };

	private:
		float Percent{1.0f};
		void SetPercent(float PercentIn);


		// For testing
	public:
#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)
		void SetPetcentTest(float PercentIn) { SetPercent(PercentIn); }
#endif
	};
}
