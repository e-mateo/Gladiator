#include "MyUI.h"
#include "MyTextWidget.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


void AMyUI::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(WidgetWin))
		WinScreen = Cast<UMyTextWidget>(CreateWidget(GetWorld(), WidgetWin));

	if (IsValid(WidgetLose))
		LoseScreen = Cast<UMyTextWidget>(CreateWidget(GetWorld(), WidgetLose));

	if (IsValid(WidgetUltimate))
		UltimateBar = Cast<UMyProgressBarWidget>(CreateWidget(GetWorld(), WidgetUltimate));

	UltimateBar->AddToViewport();
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("Restart", IE_Pressed, this, &AMyUI::Restart);
}

void AMyUI::LaunchWin()
{
	if (IsValid(WinScreen) && !WinScreen->IsInViewport())
		WinScreen->AddToViewport();
}

void AMyUI::LaunchLose()
{
	if (LoseScreen != nullptr && !LoseScreen->IsInViewport())
		LoseScreen->AddToViewport();
}

void AMyUI::Restart()
{
	if (WinScreen->IsInViewport() || LoseScreen->IsInViewport())
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}