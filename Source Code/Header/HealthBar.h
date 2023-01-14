#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MyPlayer.h"
#include "MyEnemy.h"
#include "HealthBar.generated.h"


UCLASS()
class BRAWLER_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;

public:
	UFUNCTION()
	void UpdateBar(float CurrentLife, float MaxLife);

	void BindToPlayer(AMyPlayer* Player);
	void BindToEnemy(AMyEnemy* Enemy);
};