#pragma once

class SUtils
{
public:
	template <typename T>
	static T* GetComponent(const AActor* SomeActor)
	{
		if (!SomeActor) return nullptr;

		const auto Component = SomeActor->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};
