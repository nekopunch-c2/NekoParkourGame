// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WaypointActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class MYPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentWaypoint = 0;

	UPROPERTY(EditAnywhere)
	TArray<AWaypointActor*> Waypoints;

	FVector StartLocation = GetActorLocation();  // The starting location of the sphere
	UPROPERTY(EditAnywhere)
	float SphereRadius = 100.0f;  // Radius of the sphere
	UPROPERTY(EditAnywhere)
	FVector EndLocVec = FVector(0, 0, 200);
	FVector EndLocation = StartLocation + EndLocVec;  // The endpoint of the sphere trace

	// Setup the collision parameters
	FCollisionQueryParams CollParams;

	void ManageDeath();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanDamage = true;
};
