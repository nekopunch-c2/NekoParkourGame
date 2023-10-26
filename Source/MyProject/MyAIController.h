// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Perception/AIPerceptionComponent.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	AMyAIController();
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> Waypoints;

	APawn* Player;

	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

public:
	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

};
