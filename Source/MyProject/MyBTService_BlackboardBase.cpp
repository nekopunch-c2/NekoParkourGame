// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTService_BlackboardBase.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UBTT_SetTargAsWayp::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();

    if (AIController)
    {
        AEnemy* Enemy = Cast<AEnemy>(AIController->GetPawn());

        if (Enemy)
        {
            AWaypointActor* CurrentWaypointActor = Enemy->Waypoints[Enemy->CurrentWaypoint];

            if (CurrentWaypointActor)
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("WPTarget"), CurrentWaypointActor->GetActorLocation()); // change ("WPTarget") to GetSelectedBlackboardKey()
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No CurrentWaypointActor"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to cast to AEnemy"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AIController not found"));
    }
}
