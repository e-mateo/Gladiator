#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/UObjectGlobals.h"
#include "MyAIController.h"
#include "MyBTTaskFindPlayer.generated.h"


UCLASS(Blueprintable)
class BRAWLER_API UMyBTTaskFindPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMyBTTaskFindPlayer(FObjectInitializer const& initializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};