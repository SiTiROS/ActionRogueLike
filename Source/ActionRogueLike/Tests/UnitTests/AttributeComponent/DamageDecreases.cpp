#if WITH_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "UnitTests/TestsHelpers.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDamageDecreasesHealthTest,
                                 "Project.Attribute.Damage.Decreases",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)


bool FDamageDecreasesHealthTest::RunTest(const FString& Parameters)
{
	UWorld* World;
	AActor* DummyActor;
	USAttributeComponent* AttributeComponent = FTestsHelpers::CreateTestAttributeComponent(World, DummyActor);
	
	TestNotNull(TEXT("World should be valid"), World);
	TestNotNull(TEXT("Dummy actor should be valid"), DummyActor);
	TestNotNull(TEXT("Attribute component should be valid"), AttributeComponent);
	if (!World || !DummyActor || !AttributeComponent) return false;

	
	AttributeComponent->InitHealth(100.0f); 
	const float InitialHealth = AttributeComponent->GetHealth();
	AttributeComponent->ApplyHealthChange(DummyActor, -50.0f);

	// Check new health
	const float NewHealth = AttributeComponent->GetHealth();
	TestTrue(TEXT("Health should decreased withs 100 from 50"), NewHealth == 50);

	return true;
}

#endif // WITH_AUTOMATION_TESTS
