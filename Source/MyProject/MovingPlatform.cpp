// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	
	StartingPos = Platform->GetComponentLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//move the platform


	FVector CurrentPos = Platform->GetComponentLocation();

	CurrentPos += Velocity * DeltaTime;

	SetActorLocation(CurrentPos);



	float MovedDistance = FVector::Dist(StartingPos, CurrentPos);

	if (MovedDistance > MaxMoveDist)
	{
		Velocity = -Velocity;
		StartingPos = CurrentPos;
	}



}

