// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "Math/Rotator.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "MyProjectProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMyProject3Character

AMyProjectCharacter::AMyProjectCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


}

void AMyProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	// Add Input Mapping Context
	//World = GetWorld();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	PlayerControllerGlobal = GetWorld()->GetFirstPlayerController();

	CurrentTimeHook = TimerStartHook;
}



void AMyProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("The float value is: %f"), JumpHeightUpper);
	DeltaTimeWorld = DeltaTime;

	HookTimer();
	if (GetCharacterMovement()->IsFalling() && ShouldWallrun())
	{
		WallRun();
	}
	else if ((GetCharacterMovement()->IsFalling() || !GetCharacterMovement()->IsFalling()) || !ShouldWallrun())
	{
		GetCharacterMovement()->GravityScale = NormalGravity;
		WallRunGravityIncreaser = 0;
		WallRunFOVIncreaser = 0;
		bIsWallRunning = false;
	}

	if (!bIsWallRunning)
	{

		WallRunTiltModifier = FMath::FInterpTo(WallRunTiltModifier, 0, DeltaTime, WallRunTiltToZeroInterpSpeed);
	}
	if (PlayerControllerGlobal)
	{
		FRotator rot = PlayerControllerGlobal->GetControlRotation();

		FRotator CameraRotationDelta(rot.Pitch, rot.Yaw, (WallRunTilt + WallRunTiltModifier));

		PlayerControllerGlobal->SetControlRotation(CameraRotationDelta);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("No PlayerController found."));
	}

	CharacterMovementComponent->JumpZVelocity = (JumpHeight + JumpHeightUpper * DeltaTime);
	CharacterMovementComponent->MaxWalkSpeed = (WalkingSpeed + SpeedUpper * DeltaTime);

}

//////////////////////////////////////////////////////////////////////////// Input

void AMyProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);


		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyProjectCharacter::JumpFromWall);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyProjectCharacter::StopJumpFromWall);

		//crouching

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMyProjectCharacter::StopCrouch);

		//firing hook

		EnhancedInputComponent->BindAction(FireHookAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::FireHook);
		EnhancedInputComponent->BindAction(FireHookAction, ETriggerEvent::Completed, this, &AMyProjectCharacter::StopFiringHook);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyProjectCharacter::StopMove);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyProjectCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyProjectCharacter::IsDead()
{
	UGameplayStatics::OpenLevel(this, LoadLevelName, false);
}

void AMyProjectCharacter::StartCrouch()
{
	//if was in the air and is grounded and is crouching
	if (GetCharacterMovement()->IsFalling())
	{
		if (!bIsWallRunning)
			ShouldSlide = true;
		else
			ShouldSlide = false;
		WasAir = true;
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Crouching."));
		if (SpeedUpper >= 0)
			SpeedUpper -= SpeedDecreaseValue * 100 * DeltaTimeWorld;
		if (JumpHeightUpper >= 0)
			JumpHeightUpper -= JumpHeightDecreaseValue * 100 * DeltaTimeWorld;

		UE_LOG(LogTemplateCharacter, Warning, TEXT("SpeedUpper: %f"), SpeedUpper);
		ShouldSlide = false;
	}


}
void AMyProjectCharacter::StopCrouch()
{
	ShouldSlide = false;

}


void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		if (ShouldSlide)
		{
			Slide();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Controller not found!"));
	}
}



void AMyProjectCharacter::StopMove()
{
	SpeedUpper = 0;
	JumpHeightUpper = 0;
	SpeedFOVIncreaser = 0;

	if (bIsWallRunning)
		WallRunGravityIncreaser *= 7 * DeltaTimeWorld;
}

void AMyProjectCharacter::FireHook()
{
	if (Controller == nullptr)
		return;
	if (!FirstPersonCameraComponent)
		return;

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FHitResult HookHitResult;
	FCollisionQueryParams CollisionParams;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	CollisionParams.AddIgnoredActors(ActorsToIgnore);
	FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * HookLength;

	if (GetWorld()->LineTraceSingleByChannel(HookHitResult, Start, End, ECC_Visibility, CollisionParams) && ShouldHook())
	{
		HookHit = HookHitResult.ImpactPoint;
		bIsHooking = true;
		bHasFiredHook = true;
	}
}

void AMyProjectCharacter::StopFiringHook()
{
	CharacterMovementComponent->SetMovementMode(MOVE_Falling);
	bIsHooking = false;
	bHasFiredHook = false;
	bShouldBeAbleToShoot = false;
}

void AMyProjectCharacter::HookTimer()
{
	if (!bHasFiredHook)
	{
		CurrentTimeHook -= 1 * DeltaTimeWorld;
		if (CurrentTimeHook <= 0)
		{
			bShouldBeAbleToShoot = true;
			CurrentTimeHook = TimerStartHook;
		}
	}
}

bool AMyProjectCharacter::ShouldWallrun()
{
	FVector Start = GetRootComponent()->GetComponentLocation();

	FHitResult RightHitResult;
	FCollisionQueryParams CollisionParams;
	FVector RightEnd = Start + GetRootComponent()->GetRightVector() * WallRunCheckDistance;
	if (GetWorld()->LineTraceSingleByChannel(RightHitResult, Start, RightEnd, ECC_Visibility, CollisionParams))
	{
		WallOnTheRight = true;
		UE_LOG(LogTemp, Warning, TEXT("WallOnTheRight"));
		HitNormalRight = RightHitResult.ImpactNormal;
		return true;
	}
	else
	{
		WallOnTheRight = false;
	}

	FVector LeftEnd = Start + GetRootComponent()->GetRightVector() * -1 * WallRunCheckDistance;
	FHitResult LeftHitResult;

	if (GetWorld()->LineTraceSingleByChannel(LeftHitResult, Start, LeftEnd, ECC_Visibility, CollisionParams))
	{
		WallOnTheLeft = true;
		UE_LOG(LogTemp, Warning, TEXT("WallOnTheLeft"));
		HitNormalLeft = LeftHitResult.ImpactNormal;
		return true;
	}
	else
	{
		WallOnTheLeft = false;
	}
	return false;
}

void AMyProjectCharacter::WallRun()
{
	bIsWallRunning = true;

	FVector Velocity = GetCharacterMovement()->Velocity;
	Velocity.Z = 0;

	WallRunFOVIncreaser += FOVIncreaser;

	GetCharacterMovement()->GravityScale = (WallRunGravityStart + WallRunGravityIncreaser * DeltaTimeWorld);


	if (PlayerControllerGlobal)
	{
		FRotator rot = PlayerControllerGlobal->GetControlRotation();
		if (WallOnTheLeft)
		{
			//tilt camera left
			WallRunTiltModifier = FMath::FInterpTo(WallRunTiltModifier, MaxWallRunTilt, DeltaTimeWorld, WallRunTiltToZeroInterpSpeed);

		}
		else if (WallOnTheRight)
		{
			//tilt camera right
			WallRunTiltModifier = FMath::FInterpTo(WallRunTiltModifier, MaxWallRunTilt * -1, DeltaTimeWorld, WallRunTiltToZeroInterpSpeed);
		}
	}
}

void AMyProjectCharacter::JumpFromWall()
{
	UE_LOG(LogTemp, Warning, TEXT("JumpFromWallMethod"));
	IsJumpingFromWall = true;
	if (IsJumpingFromWall && ShouldWallrun() && GetCharacterMovement()->IsFalling())
	{
		UE_LOG(LogTemp, Warning, TEXT("If IsJumpingFromWall"));
		if (WallOnTheLeft)
		{
			UE_LOG(LogTemp, Warning, TEXT("If WallOnTheLeft"));
			FVector JumpLeftDirection = HitNormalLeft;
			FVector JumpForce = JumpLeftDirection * JumpOffWallForce + FVector(0.0f, 0.0f, JumpHeightWall);
			LaunchCharacter(JumpForce, false, false);


		}
		else if (WallOnTheRight)
		{
			FVector JumpRightDirection = HitNormalRight;
			FVector JumpForce = JumpRightDirection * JumpOffWallForce + FVector(0.0f, 0.0f, JumpHeightWall);
			LaunchCharacter(JumpForce, false, false);
		}
	}
}

void AMyProjectCharacter::StopJumpFromWall()
{
	IsJumpingFromWall = false;
}

bool AMyProjectCharacter::ShouldHook()
{
	if (!bHasFiredHook && bShouldBeAbleToShoot)
	{
		return true;
	}
	return false;
}

void AMyProjectCharacter::Slide()
{
	SpeedUpper += SpeedIncreaseValue * 100 * DeltaTimeWorld;
	JumpHeightUpper += JumpHeightIncreaseValue;

	SpeedFOVIncreaser += FOVIncreaser;
	UE_LOG(LogTemp, Warning, TEXT("IsSliding"));

	WasAir = false;
}

void AMyProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyProjectCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AMyProjectCharacter::GetHasRifle()
{
	return bHasRifle;
}