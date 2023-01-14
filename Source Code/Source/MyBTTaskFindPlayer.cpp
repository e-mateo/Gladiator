#include "MyBTTaskFindPlayer.h"
#include "MyEnemy.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackBoard/BlackBoardKeyType.h"
#include "Kismet/GameplayStatics.h"


UMyBTTaskFindPlayer::UMyBTTaskFindPlayer(FObjectInitializer const& initializer)
{
	NodeName = TEXT("Find Target Location");
}

EBTNodeResult::Type UMyBTTaskFindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* Controller = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	auto* Blackboard = Controller->GetBlackboardComponent();

	//Set Blackboard Value
	AMyEnemy* Enemy = Cast<AMyEnemy>(Controller->GetPawn());
	APlayerController* Player = GetWorld()->GetFirstPlayerController();

	if (Enemy->GetIsAlive())
	{
		Blackboard->SetValueAsVector("TargetLocation", Enemy->GetTargetLoc());
		Blackboard->SetValueAsObject("Player", Player->GetPawn());
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}