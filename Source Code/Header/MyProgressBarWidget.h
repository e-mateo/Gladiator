#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CircularThrobber.h"
#include "MyPlayer.h"
#include "MyProgressBarWidget.generated.h"


UCLASS()
class BRAWLER_API UMyProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* MyProgressBar;

	UPROPERTY(meta = (BindWidget))
		UCircularThrobber* CircularThrobber_0;

	UPROPERTY(meta = (BindWidget))
		UCircularThrobber* CircularThrobber_1;

public:

	void Bind(AMyPlayer* Player);

	UFUNCTION()
	void UpdateBar(float Value, float MaxValue);
};