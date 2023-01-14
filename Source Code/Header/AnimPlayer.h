#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimPlayer.generated.h"


UCLASS()
class BRAWLER_API UAnimPlayer : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float SpeedAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool DeathAnim;

public:
	UAnimPlayer();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};