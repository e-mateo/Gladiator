#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "MyEnemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifeEnemyChangedSignature, float, CurrentLife, float, MaxLife);

UCLASS()
class BRAWLER_API AMyEnemy : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Life, meta = (AllowPrivateAccess = "true"))
		int32 Life;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Shield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UFloatingPawnMovement* FloatingPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collider, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* WeaponHitBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetHealth;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UBehaviorTree* tree;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	bool HasHit;
	bool IsAlive;
	bool IsAttacking;
	bool IsNearestToPlayer;
	int32 MaxLife;
	FVector TargetLoc;

public:
	AMyEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Lock();
	void Unlock();
	void Death();
	void GetHit();
	void Attack();
	void EndAttack();
	void ChangeMaterial();
	void SetTargetLoc(FVector const& Location);
	void ChangeEmissiveColor(FLinearColor const& Color);

	//Getter
	bool GetIsAlive() { return IsAlive; }
	bool GetIsAttacking() { return IsAttacking; }
	bool GetIsNearestToPlayer() { return IsNearestToPlayer; }
	void SetIsNearestToPlayer(bool IsNearest) { IsNearestToPlayer = IsNearest; }

	UBehaviorTree* GetTree() { return tree; }
	FVector GetTargetLoc() { return TargetLoc; };

	UPROPERTY(BlueprintAssignable)
		FOnLifeEnemyChangedSignature OnLifeChanged;

protected:
	virtual void BeginPlay() override;
};