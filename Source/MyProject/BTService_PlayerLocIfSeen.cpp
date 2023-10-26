// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocIfSeen.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "MyProjectCharacter.h"


void UBTService_PlayerLocIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AICont = OwnerComp.GetAIOwner();

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	AEnemy* Enemy = Cast<AEnemy>(ControlledPawn);

	if (Enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Enemy"));
		return;
	}


	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AICont == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("No AICont"));
		return;
	}
	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerPawn"));
		return;
	}
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("CanSeePlayer"))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}


}
