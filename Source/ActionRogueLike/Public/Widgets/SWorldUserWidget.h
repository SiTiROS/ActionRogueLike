#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"


class USizeBox;

UCLASS()
class ACTIONROGUELIKE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(meta = (BindWidget)) // биндит в блюпринте в виджете у элемента SizeBox с именем ParentSizeBox
	USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	AActor* AttachedActor; // актор над которым отображать виджет
};