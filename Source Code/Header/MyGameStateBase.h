// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyEnemy.h"
#include "MyPlayer.h"
#include "MyUI.h"
#include "MyGameStateBase.generated.h"

UCLASS()
class BRAWLER_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

		UPROPERTY(meta = (AllowPrivateAccess = "true"))
		bool Win;

	UPROPERTY(meta = (AllowPrivateAccess = "true"))
		bool Lose;

protected:
	virtual void BeginPlay() override;

	AMyPlayer* Player;
	TArray<AMyEnemy*> Enemies;
	AMyUI* UI;

public:
	AMyGameStateBase();

	bool GetWin() { return Win; }
	bool GetLose() { return Lose; }
	TArray<AMyEnemy*> GetEnemies() { return Enemies; }

	virtual void Tick(float DeltaTime) override;
};