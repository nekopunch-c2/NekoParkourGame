// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProjectGameMode();
	UFUNCTION(BlueprintCallable)
	void ActorDied(AActor* DeadActor);

protected:

	virtual void BeginPlay() override;

private:
	class AMyProjectCharacter* Character;
};



