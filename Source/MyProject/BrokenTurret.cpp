// Fill out your copyright notice in the Description page of Project Settings.


#include "BrokenTurret.h"

// Sets default values
ABrokenTurret::ABrokenTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("CapsuleCollider"));
	RootComponent = RootComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

}

// Called when the game starts or when spawned
void ABrokenTurret::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ABrokenTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

