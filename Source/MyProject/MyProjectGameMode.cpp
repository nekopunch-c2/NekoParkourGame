// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Turret.h"
#include "Enemy.h"

AMyProjectGameMode::AMyProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AMyProjectGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Character)
	{
		Character->IsDead();
	}
	else if (ATurret* DestroyedTurret = Cast<ATurret>(DeadActor))
	{
		DestroyedTurret->ManageDeath();
	}
	else if (AEnemy* EnemyAI = Cast<AEnemy>(DeadActor))
	{
		EnemyAI->ManageDeath();
	}

}

void AMyProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}
