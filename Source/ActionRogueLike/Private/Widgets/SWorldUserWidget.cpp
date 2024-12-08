#include "Widgets/SWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor)) 
	{
		RemoveFromParent();
		return;
	}

	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition)) // получаем позицию над головой персонажа
	{
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this); // коэффициент разрешения экрана

		ScreenPosition /= Scale; // вычисляем положение размещения виджета относительно нашего разрешения экрана

		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition); // бинженый бокс выставляем в полученную позицию
		}
	}
}
