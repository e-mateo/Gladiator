#include "AnimPlayer.h"
#include "MyPlayer.h"
#include "MyEnemy.h"


UAnimPlayer::UAnimPlayer()
{
	SpeedAnim = 0.f;
	DeathAnim = false;
}

void UAnimPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Pawn = TryGetPawnOwner();

	AMyPlayer* Player = Cast<AMyPlayer>(Pawn);
	AMyEnemy* Enemy = Cast<AMyEnemy>(Pawn);

	if (IsValid(Player))
	{
		if (Player->GetIsAlive())
		{
			if (Player->GetIsAttacking() == true)
				SpeedAnim = 0.f;
			else
				SpeedAnim = (float)Pawn->GetVelocity().Size() / 6.f;
		}
		else
		{
			SpeedAnim = 0.f;
			DeathAnim = true;
		}
	}
	else if (IsValid(Enemy))
	{
		if (Enemy->GetIsAlive())
		{
			SpeedAnim = (float)Pawn->GetVelocity().Size() / 6.f;
		}
		else
		{
			SpeedAnim = 0.f;
			DeathAnim = true;
		}
	}
}