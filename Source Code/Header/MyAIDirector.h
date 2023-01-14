#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEnemy.h"
#include "MyPlayer.h"
#include "MyAIDirector.generated.h"


UCLASS()
class BRAWLER_API AMyAIDirector : public AActor
{
	GENERATED_BODY()
	
private:

	TArray<AMyEnemy*> Enemies;
	AMyPlayer* Player;

public:	

	AMyAIDirector();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Setup();

	UFUNCTION()
	void UpdateLocation();

	UFUNCTION()
	AMyEnemy* FindNearestEnemy();
};