// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MyProjectCharacter.h"


AMyAIController::AMyAIController()
{
	SetupPerceptionSystem();
}
void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		Player = UGameplayStatics::GetPlayerPawn(this, 0);
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLoc"), Player->GetActorLocation());
		//GetBlackboardComponent()->SetValueAsVector(TEXT("StartPacLoc"), GetPawn()->GetActorLocation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::HandleTargetPerceptionUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No AI Behavior"));
	}
}
void AMyAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	}
}

void AMyAIController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (auto* const ch = Cast<AMyProjectCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hasn't detected player"))
	}
}
void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
