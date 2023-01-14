#include "MyAIDirector.h"
#include "MyAIController.h"
#include "MyGameStateBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"



AMyAIDirector::AMyAIDirector()
{   
	PrimaryActorTick.bCanEverTick = true;
}

void AMyAIDirector::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle _;
	GetWorldTimerManager().SetTimer(_, this, &AMyAIDirector::Setup, 0.1f, false);
}

void AMyAIDirector::Setup()
{
	Player = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

	AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
	if (GameState)
		Enemies = GameState->GetEnemies();
}

void AMyAIDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player)
	{
		UpdateLocation();

		AMyEnemy* NearestEnemy = FindNearestEnemy();
		if (NearestEnemy != nullptr)
			NearestEnemy->SetIsNearestToPlayer(true);
	}
}

void AMyAIDirector::UpdateLocation()
{

	FVector PlayerPos = Player->GetTransform().GetLocation();

	for (int i = 0; i < Enemies.Num(); i++)
	{
		if (Enemies[i]->GetIsAlive() && Player->GetIsAlive())
		{

			AMyAIController* Controller = Cast<AMyAIController>(Enemies[i]->GetController());
			auto* Blackboard = Controller->GetBlackboardComponent();
			
			FVector EnemyPos = Enemies[i]->GetTransform().GetLocation();
			PlayerPos = FVector(PlayerPos.X, PlayerPos.Y, Enemies[i]->GetTransform().GetLocation().Z);
			Blackboard->SetValueAsVector("Player Pos", PlayerPos);

			FVector Dir = (EnemyPos - PlayerPos).GetSafeNormal();
			FVector TargetCenterPos = PlayerPos + Dir * 400;

			//Verif if the enemy can see the player
			FHitResult OutHit, OutHit2;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(Enemies[i]->GetOwner());

			FVector StartRayCast1 = EnemyPos + Enemies[i]->GetActorRightVector() * 40 + (-Dir) * 100;
			FVector StartRayCast2 = EnemyPos + Enemies[i]->GetActorRightVector() * -40 + (-Dir) * 100;
			FVector EndRayCast = PlayerPos;

			bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, StartRayCast1, EndRayCast, ECC_Pawn, Params);
			bool IsHit2 = GetWorld()->LineTraceSingleByChannel(OutHit2, StartRayCast2, EndRayCast, ECC_Pawn, Params);

			if (IsHit || IsHit2)
			{
				if (Cast<AMyEnemy>(OutHit.GetActor()) != nullptr || Cast<AMyEnemy>(OutHit2.GetActor()) != nullptr)
				{
					//If there is another Enemy in front of this enemy, make this enemy move to an another position
					bool Positive = FMath::RandBool();
					float Sign = 0;
					if (Positive)
						Sign = 1.0f;
					else
						Sign = -1.0f;

					Dir = Dir.RotateAngleAxis(FMath::FRandRange(20, 60) * Sign, FVector::ZAxisVector);
					TargetCenterPos = PlayerPos + Dir * FMath::FRandRange(300, 600);
				}
			}

			float radius = 100;
			FVector CircleRandom = FMath::VRand();
			CircleRandom = CircleRandom.GetSafeNormal() * radius;
			FVector TargetLoc = TargetCenterPos + CircleRandom;
			TargetLoc.Z = Enemies[i]->GetTransform().GetLocation().Z; //Lock the Z position of the Enemy
			Enemies[i]->SetTargetLoc(TargetLoc);
		}
	}
}


AMyEnemy* AMyAIDirector::FindNearestEnemy()
{
	float MinDistanceToPlayer = 1000000.f;
	AMyEnemy* NearestEnemy = nullptr;

	for (int i = 0; i < Enemies.Num(); i++)
	{
		Enemies[i]->SetIsNearestToPlayer(false);
		float DistanceToPlayer = (Player->GetTransform().GetLocation() - Enemies[i]->GetTransform().GetLocation()).Size();
		if (DistanceToPlayer < MinDistanceToPlayer && Player->GetIsAlive() && Enemies[i]->GetIsAlive())
		{
			NearestEnemy = Enemies[i];
			MinDistanceToPlayer = DistanceToPlayer;
		}
	}

	return NearestEnemy;
}
