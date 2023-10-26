// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	CollParams.AddIgnoredActors(ActorsToIgnore);
	CollParams.bReturnPhysicalMaterial = false;
	CollParams.bTraceComplex = true;
	bCanDamage = true;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::ManageDeath()
{
	USkeletalMeshComponent* SkelMesh = FindComponentByClass<USkeletalMeshComponent>();
	UCapsuleComponent* CapsuleComp = FindComponentByClass<UCapsuleComponent>();
	bCanDamage = false;
	if (SkelMesh)
	{
		SkelMesh->SetSimulatePhysics(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No SkelMesh found"));
	}
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No CapsuleComp found"));
	}
}



