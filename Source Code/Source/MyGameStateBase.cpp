#include "MyGameStateBase.h"
#include "MyUI.h"
#include "BrawlerGameModeBase.h"
#include "Kismet/GameplayStatics.h"


AMyGameStateBase::AMyGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
	Win = false;
	Lose = false;
}

void AMyGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<AMyEnemy> ClassToFind = AMyEnemy::StaticClass();
	TArray<AActor*> EnemiesActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, EnemiesActors);

	for (int i = 0; i < EnemiesActors.Num(); i++)
		Enemies.Add(Cast<AMyEnemy>(EnemiesActors[i]));

	Player = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UI = Cast<AMyUI>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
}

void AMyGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player->GetIsAlive())
	{
		Lose = true;
		if (IsValid(UI))
			UI->LaunchLose();
	}

	float NumberEnemiesDead = 0;

	for (int i = 0; i < Enemies.Num(); i++)
	{
		if (!Enemies[i]->GetIsAlive())
			NumberEnemiesDead++;
	}

	if (NumberEnemiesDead == 5)
	{
		Win = true;
		if (IsValid(UI))
			UI->LaunchWin();
	}
}