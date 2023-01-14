#include "MyBTTaskShouldAttack.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "MyEnemy.h"
#include "BehaviorTree/BlackBoard/BlackBoardKeyType.h"
#include "Kismet/GameplayStatics.h"


UMyBTTaskShouldAttack::UMyBTTaskShouldAttack(FObjectInitializer const& initializer)
{
	NodeName = TEXT("Should Attack ?");
}

EBTNodeResult::Type UMyBTTaskShouldAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and Blackboard
	auto* Controller = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	auto* BlackboardComp = Controller->GetBlackboardComponent();
	AMyEnemy* Enemy = Cast<AMyEnemy>(Controller->GetPawn());

	//Set Blackboard Value
	BlackboardComp->SetValueAsBool("ShouldAttack", Enemy->GetIsNearestToPlayer());

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}