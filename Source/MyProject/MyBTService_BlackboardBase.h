// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_BlackboardBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTT_SetTargAsWayp : public UBTService_BlackboardBase
{
	GENERATED_BODY()
protected:
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
private:
	FBlackboardKeySelector BBKeyTarget;
};
