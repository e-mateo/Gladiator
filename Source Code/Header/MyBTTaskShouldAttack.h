#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "MyBTTaskShouldAttack.generated.h"


UCLASS(Blueprintable)
class BRAWLER_API UMyBTTaskShouldAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UMyBTTaskShouldAttack(FObjectInitializer const& initializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};