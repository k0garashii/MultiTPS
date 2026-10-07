#include "PlayerWidget.h"

void UPlayerWidget::UpdateHealthBar(int CurrentHealth, int MaxHealth) const
{
	float HealthPercent = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
	HealthBar->SetPercent(HealthPercent);
}

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
