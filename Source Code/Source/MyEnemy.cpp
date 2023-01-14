#include "MyEnemy.h"
#include "MyPlayer.h"
#include "HealthBar.h"
#include "MyAIController.h"


AMyEnemy::AMyEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	IsAlive = true;
	HasHit = false;

	Life = 3;
	MaxLife = Life;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
	Capsule->InitCapsuleSize(38.f, 88.f);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Mesh, "WeaponPoint");

	Shield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	Shield->SetupAttachment(Mesh, "DualWeaponPoint");

	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHitBox"));
	WeaponHitBox->SetupAttachment(Weapon);

	WidgetHealth = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealth"));
	WidgetHealth->SetupAttachment(Capsule);

	FloatingPawn = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Camera Overlap Enemies
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);

	//Event Begin Overlap with the WeaponHitBox
	WeaponHitBox->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemy::OnHit);

	//Bind UI Life
	auto HealthBar = Cast<UHealthBar>(WidgetHealth->GetUserWidgetObject());
	if (HealthBar)
		HealthBar->BindToEnemy(this);
	WidgetHealth->SetVisibility(false);
	OnLifeChanged.Broadcast((float)Life, (float)MaxLife);
}

void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlive)
	{   //Look At The Player
		FRotator rotation = (GetWorld()->GetFirstPlayerController()->GetPawn()->GetTransform().GetLocation() - GetActorLocation()).Rotation();
		SetActorRotation(rotation);
	}
}

void AMyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyEnemy::GetHit()
{
	if (IsAlive)
	{
		Life--;
		OnLifeChanged.Broadcast((float)Life, (float)MaxLife);

		if (Life <= 0)
			Death();

		ChangeEmissiveColor(FLinearColor(1, 0, 0));
		float Duration = 0.3f;
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AMyEnemy::ChangeMaterial, Duration, false);
	}
}

void AMyEnemy::ChangeMaterial()
{
	ChangeEmissiveColor(FLinearColor(0, 0, 0));
}

void AMyEnemy::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	AMyPlayer* Player = Cast<AMyPlayer>(OtherActor);

	if (Player != nullptr && IsAttacking && !HasHit && IsAlive)
	{
		HasHit = true;
		FVector EnemyPos = GetActorLocation();
		FVector PlayerPos = Player->GetTransform().GetLocation();

		FVector ForwardPlayer = Player->GetActorForwardVector();
		ForwardPlayer = FVector(ForwardPlayer.X, ForwardPlayer.Y, 0);

		FVector PlayerToEnemyDir = (EnemyPos - PlayerPos).GetSafeNormal();
		PlayerToEnemyDir = FVector(PlayerToEnemyDir.X, PlayerToEnemyDir.Y, 0);

		//Check if The Enemy is in front of the player and if the player has his shield up
		float Angle = FMath::RadiansToDegrees(acos(FVector::DotProduct(ForwardPlayer, PlayerToEnemyDir))); //Angle between ForwardPlayer and PlayerToEnemyDir

		if (Player->GetIsShieldUp() && FMath::Abs(Angle) < 80)
			Player->HitShield();
		else
			Player->GetHit();
	}
}


void AMyEnemy::Attack()
{
	if (!Mesh->AnimScriptInstance->Montage_IsPlaying(AttackMontage) && !IsAttacking && IsAlive)
	{
		IsAttacking = true;
		float Duration = Mesh->AnimScriptInstance->Montage_Play(AttackMontage);
		FTimerHandle _;
		GetWorldTimerManager().SetTimer(_, this, &AMyEnemy::EndAttack, Duration, false);
	}
}

void AMyEnemy::EndAttack()
{
	IsAttacking = false;
	HasHit = false;
}

void AMyEnemy::Death()
{
	Capsule->SetCollisionProfileName(FName("NoCollision"));

	if (!Mesh->AnimScriptInstance->Montage_IsPlaying(DeathMontage) && IsAlive)
	{
		IsAlive = false;
		WidgetHealth->SetVisibility(false);
		Mesh->AnimScriptInstance->Montage_Play(DeathMontage);

		AMyAIController* AIController = Cast<AMyAIController>(GetController());
		if (AIController)
			AIController->UnPossess();
	}
}

void AMyEnemy::ChangeEmissiveColor(FLinearColor const& Color)
{
	auto mat = Mesh->GetMaterial(0);
	if (mat)
	{
		auto realMat = Cast<UMaterialInstanceDynamic>(mat->GetMaterial());
		if (!realMat)
			realMat = Mesh->CreateAndSetMaterialInstanceDynamic(0);
		realMat->SetVectorParameterValue("EmissivePara", Color);
	}
}

void AMyEnemy::Lock()
{
	WidgetHealth->SetVisibility(true);
}

void AMyEnemy::Unlock()
{
	WidgetHealth->SetVisibility(false);
}

void AMyEnemy::SetTargetLoc(FVector const& Location)
{
	TargetLoc = Location;
}