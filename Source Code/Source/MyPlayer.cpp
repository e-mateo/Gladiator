#include "MyPlayer.h"
#include "MyEnemy.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrawlerGameModeBase.h"
#include "MyUI.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "HealthBar.h"
#include "MyGameStateBase.h"


AMyPlayer::AMyPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	IsAlive = true;
	HasHit = false;
	LookAtAnEnemy = false;

	Life = 5;
	UltCharge = 0;
	MaxLife = Life;
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	GetCapsuleComponent()->InitCapsuleSize(38.f, 88.f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 2000.f;
	GetCharacterMovement()->GravityScale = 6.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 700.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), "WeaponPoint");

	Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(GetMesh(), "DualWeaponPoint");

	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));
	WeaponHitBox->SetupAttachment(Weapon);

	WidgetHealth = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealth"));
	WidgetHealth->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	WeaponHitBox->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnHit);

	//Bind UI Life and UI Ultimate
	auto HealthBar = Cast<UHealthBar>(WidgetHealth->GetUserWidgetObject());
	if (HealthBar)
		HealthBar->BindToPlayer(this);

	AMyUI* UI = Cast<AMyUI>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (UI)
		UI->GetUIUltimate()->Bind(this);

	OnLifeChanged.Broadcast((float)Life, (float)MaxLife);
	OnUltimateChargeChange.Broadcast(UltCharge, 6);
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateLock();
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Lock", IE_Pressed, this, &AMyPlayer::LockEnemy);
	PlayerInputComponent->BindAxis("ChangeLock", this, &AMyPlayer::ChangeLock);
	PlayerInputComponent->BindAction("ChangeLockController", IE_Pressed, this, &AMyPlayer::ChangeLockController);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyPlayer::Attack);

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &AMyPlayer::Ultimate);

	PlayerInputComponent->BindAction("Shield", IE_Pressed, this, &AMyPlayer::StartShield);
	PlayerInputComponent->BindAction("Shield", IE_Released, this, &AMyPlayer::EndShield);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMyPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyPlayer::LookUpAtRate);
}

void AMyPlayer::LockEnemy()
{
	if (!LookAtAnEnemy)
	{
		float MinAngle = 400.f;
		AMyEnemy* TargetEnemy = nullptr;
		AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
		TArray<AMyEnemy*> Enemies = GameState->GetEnemies();

		FVector PlayerPos = GetTransform().GetLocation();
		FVector CamForward = GetControlRotation().Vector();
		CamForward = FVector(CamForward.X, CamForward.Y, 0);

		//Find the Enemy we are looking at
		for (int i = 0; i < Enemies.Num(); i++)
		{
			FVector EnemyPos = Enemies[i]->GetTransform().GetLocation();

			FVector PlayerToEnemyDir = (EnemyPos - PlayerPos).GetSafeNormal();
			PlayerToEnemyDir = FVector(PlayerToEnemyDir.X, PlayerToEnemyDir.Y, 0);

			float Angle = FMath::RadiansToDegrees(acos(CamForward.CosineAngle2D(PlayerToEnemyDir)));

			if (Angle < MinAngle && Enemies[i]->GetIsAlive())
			{
				MinAngle = Angle;
				TargetEnemy = Enemies[i];
			}
		}

		if (TargetEnemy != nullptr)
			StartLockEnemy(TargetEnemy);
	}
	else
	{
		EndLockEnemy();
	}

}

void AMyPlayer::StartLockEnemy(AMyEnemy* Enemy)
{
	EnemyToLookAt = Enemy;
	EnemyToLookAt->Lock();
	LookAtAnEnemy = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AMyPlayer::EndLockEnemy()
{
	if (LookAtAnEnemy && EnemyToLookAt != nullptr)
	{
		EnemyToLookAt->Unlock();
		LookAtAnEnemy = false;
		EnemyToLookAt = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AMyPlayer::UpdateLock()
{
	if (LookAtAnEnemy && EnemyToLookAt != nullptr && EnemyToLookAt->GetIsAlive())
	{
		FVector PlayerToEnemy = EnemyToLookAt->GetTransform().GetLocation() - GetTransform().GetLocation();
		PlayerToEnemy = FVector(PlayerToEnemy.X, PlayerToEnemy.Y, 0);

		FRotator LookAtEnemy = (PlayerToEnemy).Rotation();
		SetActorRotation(LookAtEnemy);

		FVector CamDir = PlayerToEnemy.RotateAngleAxis(25, GetActorRightVector());
		FRotator CamLookAtEnemy = CamDir.Rotation();
		GetController()->SetControlRotation(CamLookAtEnemy);
	}
	else if (LookAtAnEnemy && EnemyToLookAt != nullptr && !EnemyToLookAt->GetIsAlive()) //If the Enemy dies
	{
		EndLockEnemy();
	}
}

void AMyPlayer::ChangeLock(const float Value)
{
	if (LookAtAnEnemy && EnemyToLookAt != nullptr && Value != 0)
	{
		AMyEnemy* NewTargetEnemy = nullptr;

		AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
		TArray<AMyEnemy*> Enemies = GameState->GetEnemies();

		for (int i = 0; i < Enemies.Num(); i++)
		{
			if (EnemyToLookAt == Enemies[i] && Value > 0)
			{
				if (i < Enemies.Num() - 1)
					NewTargetEnemy = Enemies[i + 1];
				else
					NewTargetEnemy = Enemies[0];
			}
			else if (EnemyToLookAt == Enemies[i] && Value < 0)
			{
				if (i > 0)
					NewTargetEnemy = Enemies[i - 1];
				else
					NewTargetEnemy = Enemies[4];
			}
		}

		if (NewTargetEnemy != nullptr)
		{
			EndLockEnemy();
			StartLockEnemy(NewTargetEnemy);
		}
	}
}

void AMyPlayer::ChangeLockController()
{
	if (LookAtAnEnemy && EnemyToLookAt != nullptr)
	{
		AMyEnemy* NewTargetEnemy = nullptr;

		AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
		TArray<AMyEnemy*> Enemies = GameState->GetEnemies();

		for (int i = 0; i < Enemies.Num(); i++)
		{
			if (EnemyToLookAt == Enemies[i])
			{
				if (i < Enemies.Num() - 1)
					NewTargetEnemy = Enemies[i + 1];
				else
					NewTargetEnemy = Enemies[0];
			}
		}

		if (NewTargetEnemy != nullptr)
		{
			EndLockEnemy();
			StartLockEnemy(NewTargetEnemy);
		}
	}
}

void AMyPlayer::Attack()
{
	if (!GetMesh()->AnimScriptInstance->Montage_IsPlaying(AttackMontage) && !IsShieldUp && IsAlive)
	{
		IsAttacking = true;
		float Duration = GetMesh()->AnimScriptInstance->Montage_Play(AttackMontage);
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AMyPlayer::EndAttack, Duration, false);
	}
}

void AMyPlayer::EndAttack()
{
	IsAttacking = false;
	HasHit = false;
}

void AMyPlayer::Ultimate()
{
	if (UltCharge >= 6 && !IsAttacking && !IsShieldUp && IsAlive)
	{
		UltCharge = 0;
		OnUltimateChargeChange.Broadcast(UltCharge, 6);
		ChangeEmissiveColor(FLinearColor(0, 0, 1));
		GetCharacterMovement()->StopMovementImmediately();
		Jump();
		IsUlting = true;
		float Duration = GetMesh()->AnimScriptInstance->Montage_Play(UltMontage);
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AMyPlayer::EndUltimate, Duration * 2, false);
	}
}

void AMyPlayer::EndUltimate()
{
	ActivateNiagara();

	ChangeEmissiveColor(FLinearColor(0, 0, 0));
	IsUlting = false;
	AMyGameStateBase* GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
	TArray<AMyEnemy*> Enemies = GameState->GetEnemies();

	for (int i = 0; i < Enemies.Num(); i++)
		Enemies[i]->GetHit();
}

void AMyPlayer::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AMyEnemy* Enemy = Cast<AMyEnemy>(OtherActor);

	if (Enemy != nullptr && IsAttacking && !HasHit && IsAlive)
	{
		UltCharge++;
		OnUltimateChargeChange.Broadcast(UltCharge, 6);
		HasHit = true;
		Enemy->GetHit();
	}
}

void AMyPlayer::GetHit()
{
	if (IsAlive && !IsUlting)
	{
		Life--;
		UltCharge++;
		ChangeEmissiveColor(FLinearColor(1, 0, 0));
		float Duration = 0.3f;
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AMyPlayer::ChangeMaterial, Duration, false);
	}

	if (Life <= 0 && IsAlive)
		Death();

	OnLifeChanged.Broadcast((float)Life, (float)MaxLife);
	OnUltimateChargeChange.Broadcast(UltCharge, 6);
}

void AMyPlayer::HitShield()
{
	if (IsAlive)
	{
		ChangeEmissiveColor(FLinearColor(0, 0, 1));
		float Duration = 0.15f;
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AMyPlayer::ChangeMaterial, Duration, false);
	}
}

void AMyPlayer::ChangeMaterial()
{
	ChangeEmissiveColor(FLinearColor(0, 0, 0));
}

void AMyPlayer::Death()
{
	GetCapsuleComponent()->SetCollisionProfileName(FName("NoCollision"));
	if (!GetMesh()->AnimScriptInstance->Montage_IsPlaying(DeathMontage) && IsAlive)
	{
		IsAlive = false;
		WidgetHealth->SetVisibility(false);
		GetMesh()->AnimScriptInstance->Montage_Play(DeathMontage);
	}
}

void AMyPlayer::MoveForward(const float Value)
{
	if (Controller != nullptr && Value != 0.0f && !IsUlting && !IsAttacking && !IsShieldUp && IsAlive)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyPlayer::MoveRight(const float Value)
{
	if (Controller != nullptr && Value != 0.0f && !IsUlting && !IsAttacking && !IsShieldUp && IsAlive)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMyPlayer::TurnAtRate(const float Rate)
{
	if (IsAlive && !LookAtAnEnemy)
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyPlayer::LookUpAtRate(const float Rate)
{
	if (IsAlive && !LookAtAnEnemy)
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyPlayer::StartShield()
{
	if (IsAlive)
	{
		IsShieldUp = true;
		Shield->SetRelativeScale3D(FVector(1.40, 1.40, 1.40));
		Shield->AddLocalOffset(FVector(0, 5, 20));
	}
}

void AMyPlayer::EndShield()
{
	if (IsAlive)
	{
		IsShieldUp = false;
		Shield->AddLocalOffset(FVector(0, -5, -20));
		Shield->SetRelativeScale3D(FVector(1, 1, 1));
	}
}

void AMyPlayer::ChangeEmissiveColor(FLinearColor const& Color)
{
	auto mat = GetMesh()->GetMaterial(0);
	if (mat) 
	{
		auto realMat = Cast<UMaterialInstanceDynamic>(mat->GetMaterial());
		if (!realMat)
			realMat = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
		realMat->SetVectorParameterValue("EmissivePara", Color);
	}
}