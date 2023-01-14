#pragma once
#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "MyEnemy.h"
#include "MyPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifePlayerChangedSignature, float, CurrentLife, float, MaxLife);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUltimateChargeChangedSignature, float, CurrentCharge, float, MaxCharge);

UCLASS()
class BRAWLER_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Life, meta = (AllowPrivateAccess = "true"))
		int32 Life;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* UltMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Shield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collider, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* WeaponHitBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collider, meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetHealth;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	bool HasHit;
	bool LookAtAnEnemy;
	bool IsAlive;
	bool IsUlting;
	bool IsShieldUp;
	bool IsAttacking;
	int UltCharge;
	int32 MaxLife;
	AMyEnemy* EnemyToLookAt;

protected:
	virtual void BeginPlay() override;

	void Attack();
	void EndAttack();

	void StartShield();
	void EndShield();

	void Ultimate();
	void EndUltimate();

	void Death();

	void LockEnemy();
	void StartLockEnemy(AMyEnemy* Enemy);
	void EndLockEnemy();
	void UpdateLock();
	void ChangeLock(float Value);
	void ChangeLockController();

	void TurnAtRate(const float Rate);
	void MoveRight(const float Value);
	void MoveForward(const float Value);
	void LookUpAtRate(const float Rate);

	void ChangeMaterial();
	void ChangeEmissiveColor(FLinearColor const& Color);

public:	
	AMyPlayer();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Animation")
		void ActivateNiagara();

	void GetHit();
	void HitShield();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Getter
	bool GetIsAlive() const { return IsAlive; }
	bool GetIsAttacking() const { return IsAttacking; }
	bool GetIsShieldUp() const { return IsShieldUp; }
	int32 GetLife() const { return Life; }
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(BlueprintAssignable)
		FOnLifePlayerChangedSignature OnLifeChanged;

	UPROPERTY(BlueprintAssignable)
		FOnUltimateChargeChangedSignature OnUltimateChargeChange;

};