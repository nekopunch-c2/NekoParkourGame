// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementCurrentWP.h"
#include "AIController.h"
#include "Enemy.h"

EBTNodeResult::Type UBTTask_IncrementCurrentWP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();

    if (AIController)
    {
        AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());

        if (Enemy)
        {
            if (Enemy->Waypoints.Num() > 0)
            {
                if ((Enemy->CurrentWaypoint + 1) >= (Enemy->Waypoints.Num()))
                {
                    Enemy->CurrentWaypoint = 0;
                }
                else
                {
                    Enemy->CurrentWaypoint += 1;
                }
            }
        }
    }
    return EBTNodeResult::Succeeded;
}
