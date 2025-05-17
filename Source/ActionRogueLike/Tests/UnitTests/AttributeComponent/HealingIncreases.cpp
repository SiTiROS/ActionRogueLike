#if WITH_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "UnitTests/TestsHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealIncreasesHealthTest,
                                 "Project.UnitTest.Attribute.ApplyHealthChange.Increases",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority)


bool FHealIncreasesHealthTest::RunTest(const FString& Parameters)
{
	UWorld* World;
	AActor* DummyActor;
	USAttributeComponent* AttributeComponent = FTestsHelpers::CreateTestAttributeComponent(World, DummyActor);
	
	TestNotNull(TEXT("World should be valid"), World);
	TestNotNull(TEXT("Dummy actor should be valid"), DummyActor);
	TestNotNull(TEXT("Attribute component should be valid"), AttributeComponent);
	if (!World || !DummyActor || !AttributeComponent) return false;

	
	AttributeComponent->InitHealth(60.0f); 
	AttributeComponent->ApplyHealthChange(DummyActor, +25.0f);

	// Check new health
	const float NewHealth = AttributeComponent->GetHealth();
	TestTrue(TEXT("Health should increased withs 60 from 85"), NewHealth == 85);

	return true;
}

#endif // WITH_AUTOMATION_TESTS
