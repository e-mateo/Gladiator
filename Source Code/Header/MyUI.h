#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MyTextWidget.h"
#include "MyProgressBarWidget.h"
#include "MyUI.generated.h"


UCLASS()
class BRAWLER_API AMyUI : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "ClassType")
		TSubclassOf<UUserWidget> WidgetWin;

	UPROPERTY(EditAnywhere, Category = "ClassType")
		TSubclassOf<UUserWidget> WidgetLose;

	UPROPERTY(EditAnywhere, Category = "ClassType")
		TSubclassOf<UUserWidget> WidgetUltimate;

	UPROPERTY()
		UMyTextWidget* WinScreen;

	UPROPERTY()
		UMyTextWidget* LoseScreen;

	UPROPERTY(EditAnywhere)
		UMyProgressBarWidget* UltimateBar;

public:
	AMyUI() {}

	void Restart();
	void LaunchWin();
	void LaunchLose();

	virtual void BeginPlay() override;

	UMyProgressBarWidget* GetUIUltimate() { return UltimateBar; }
};