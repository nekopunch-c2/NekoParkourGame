// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectGameMode.h"
#include "MyProjectCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	Character = Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	GameMode = Cast<AMyProjectGameMode>(UGameplayStatics::GetGameMode(this));
}


void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0) return;

	Health -= Damage;
	
	bIsTakingDamage = true;

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	if (Health <= 0 && GameMode)
	{
		GameMode->ActorDied(DamagedActor);
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if (bIsTakingDamage)
	//	bIsTakingDamage = false;
}

