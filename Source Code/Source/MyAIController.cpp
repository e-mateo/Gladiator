#include "MyAIController.h"
#include "MyEnemy.h"


AMyAIController::AMyAIController(FObjectInitializer const& object)
{
	BehaviorTreeComp = object.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTree Component"));
	BlackboardComp = object.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("Blackboard Component"));
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AMyEnemy* Enemy = Cast<AMyEnemy>(InPawn);

	if (Enemy != nullptr && Enemy->GetTree() != nullptr)
	{
		BlackboardComp->InitializeBlackboard(*Enemy->GetTree()->BlackboardAsset);
		RunBehaviorTree(Enemy->GetTree());
		BehaviorTreeComp->StartTree(*Enemy->GetTree());
	}
}