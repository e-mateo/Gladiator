#include "MyBTTaskAttackPlayer.h"
#include "MyEnemy.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackBoard/BlackBoardKeyType.h"
#include "Kismet/GameplayStatics.h"


UMyBTTaskAttackPlayer::UMyBTTaskAttackPlayer(FObjectInitializer const& initializer)
{
	NodeName = TEXT("Attack Player");
}

EBTNodeResult::Type UMyBTTaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get Controller and Blackboard
	auto* Controller = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	auto* BlackboardComp = Controller->GetBlackboardComponent();
	AMyEnemy* Enemy = Cast<AMyEnemy>(Controller->GetPawn());

	//Set Blackboard Value
	if (Enemy->GetIsAlive())
	{
		BlackboardComp->SetValueAsBool("ShouldAttack", false);
		Enemy->Attack();
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}