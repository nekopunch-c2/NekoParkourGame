// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "MyProjectCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectProjectile.h"
#include "TimerManager.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMeshHolder = CreateDefaultSubobject<USceneComponent>(TEXT("TurretHolder"));
	TurretMeshHolder->SetupAttachment(BaseMesh);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(TurretMeshHolder);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	MainCharacter = Cast<AMyProjectCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurret::CheckFireCondition, FireRate, true);
}

void ATurret::Fire()
{
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

			const FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			World->SpawnActor<AMyProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("No World found."));
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("No ProjectileClass found."));
	}
}

void ATurret::CheckFireCondition()
{
	float Distance = FVector::Dist(GetActorLocation(), MainCharacter->GetActorLocation());
	if (IsCloseEnough(Distance))
	{
		Fire();
	}
}

bool ATurret::IsCloseEnough(float Distance)
{

	if (Distance <= DistanceShootingAllowed)
	{
		return true;
	}
	
	return false;
}

void ATurret::RotateTurret(FVector TargetToLookAt, float Distance, float DeltaTime)
{
	FVector ToTargetBase = TargetToLookAt - BaseMesh->GetComponentLocation();
	FVector ToTargetTurret = TargetToLookAt - TurretMeshHolder->GetComponentLocation();

	FRotator LookAtBase = FRotator(0, ToTargetBase.Rotation().Yaw, 0);
	FRotator LookAtTurret = FRotator(ToTargetTurret.Rotation().Pitch, ToTargetTurret.Rotation().Yaw, 0);

	TurretMeshHolder->SetWorldRotation(FMath::RInterpTo(TurretMeshHolder->GetComponentRotation(), LookAtTurret, DeltaTime, InterpSpeed));
	BaseMesh->SetWorldRotation(FMath::RInterpTo(BaseMesh->GetComponentRotation(), LookAtBase, DeltaTime, InterpSpeed));
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float Distance = FVector::Dist(GetActorLocation(), MainCharacter->GetActorLocation());

	if (IsCloseEnough(Distance))
	{
		RotateTurret(MainCharacter->GetActorLocation(), Distance, DeltaTime);
	}
}

