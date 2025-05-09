#pragma once

#include "CoreMinimal.h"
#include "Components/SAttributeComponent.h"
#include "Tests/AutomationEditorCommon.h"

class FTestsHelpers
{
public:
	static inline USAttributeComponent* CreateTestAttributeComponent(UWorld*& OutWorld, AActor*& OutActor)
	{
		OutWorld = FAutomationEditorCommonUtils::CreateNewMap();						// Создаём временный мир
		OutActor = OutWorld->SpawnActor<AActor>();										// Создаём временный актор
		USAttributeComponent* Component = NewObject<USAttributeComponent>(OutActor);	// Создаём компонент
		Component->RegisterComponent();													// Регистрируем компонент в мире

		if (!OutWorld || !OutActor || !Component) return nullptr;
		return Component;
	}
};
