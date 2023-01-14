#include "HealthBar.h"


void UHealthBar::UpdateBar(float CurrentLife, float MaxLife)
{
	ProgressBar->SetPercent((CurrentLife / MaxLife));
}

void UHealthBar::BindToPlayer(AMyPlayer* Player)
{
	Player->OnLifeChanged.AddUniqueDynamic(this, &UHealthBar::UpdateBar);
}

void UHealthBar::BindToEnemy(AMyEnemy* Enemy)
{
	Enemy->OnLifeChanged.AddUniqueDynamic(this, &UHealthBar::UpdateBar);
}

