#include "MyProgressBarWidget.h"


void UMyProgressBarWidget::UpdateBar(float Value, float MaxValue)
{
	MyProgressBar->SetPercent((Value / MaxValue));

	if (Value / MaxValue >= 1.0f && !CircularThrobber_0->IsVisible())
	{
		CircularThrobber_0->SetVisibility(ESlateVisibility::Visible);
		CircularThrobber_1->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Value / MaxValue < 1.0f && CircularThrobber_0->IsVisible())
	{
		CircularThrobber_0->SetVisibility(ESlateVisibility::Hidden);
		CircularThrobber_1->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMyProgressBarWidget::Bind(AMyPlayer* Player)
{
	Player->OnUltimateChargeChange.AddUniqueDynamic(this, &UMyProgressBarWidget::UpdateBar);
}