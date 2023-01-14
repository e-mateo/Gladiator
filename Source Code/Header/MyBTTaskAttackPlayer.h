#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "MyBTTaskAttackPlayer.generated.h"


UCLASS(Blueprintable)
class BRAWLER_API UMyBTTaskAttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMyBTTaskAttackPlayer(FObjectInitializer const& initializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};