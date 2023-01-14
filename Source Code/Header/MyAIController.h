#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyAIController.generated.h"


UCLASS()
class BRAWLER_API AMyAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComp;

public:
	AMyAIController(FObjectInitializer const& object = FObjectInitializer::Get());

	virtual void OnPossess(APawn* InPawn) override;
	UBlackboardComponent* GetBlackBoardComp() { return BlackboardComp; };
	UBehaviorTreeComponent* GetBehaviorTreeComp() { return BehaviorTreeComp; };
};