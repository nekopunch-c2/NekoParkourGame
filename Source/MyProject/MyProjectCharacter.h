// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MyProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** FireHook Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireHookAction = nullptr;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;




	UPROPERTY(EditAnywhere)
	float RunningSpeed = 1000;

	UPROPERTY(EditAnywhere)
	float WalkingSpeed = 600;

	UPROPERTY(EditAnywhere)
	float JumpHeight = 600;

	UPROPERTY(EditAnywhere)
	float WallRunCheckDistance = 5;

	UPROPERTY(EditAnywhere)
	float WallRunGravityStart = 0.1;

	UPROPERTY(EditAnywhere)
	float NormalGravity = 1;

	UPROPERTY(EditAnywhere)
	float WallRunGravityIncreaser = 0.1;

	float WallRunFOVIncreaser = 0;

	float SpeedFOVIncreaser = 0;

	UPROPERTY(EditAnywhere)
	float FOVIncreaser = 2;

	UPROPERTY(EditAnywhere)
	float JumpOffWallForce = 100;

	bool IsJumpingFromWall = false;

	UPROPERTY(EditAnywhere)
	float JumpHeightWall = 10000;

	UPROPERTY(EditAnywhere)
	float HookLength = 200;

	float WallRunTilt = 0;

	float WallRunTiltModifier = 0;

	UPROPERTY(EditAnywhere)
	float WallRunTiltUpper = 2;

	UPROPERTY(EditAnywhere)
	float WallRunTiltToZeroInterpSpeed = 10;

	UPROPERTY(EditAnywhere)
	float MaxWallRunTilt = 30;

	bool WallOnTheLeft = false;
	bool WallOnTheRight = false;

	float SpeedUpper = 0;

	UPROPERTY(EditAnywhere)
	float SpeedIncreaseValue = 300;

	UPROPERTY(EditAnywhere)
	float SpeedDecreaseValue = 20;

	float JumpHeightUpper = 0;

	UPROPERTY(EditAnywhere)
	float JumpHeightIncreaseValue = 100;

	UPROPERTY(EditAnywhere)
	float JumpHeightDecreaseValue = 20;

	UPROPERTY(EditAnywhere)
	float HookLaunchForce = 100;

	UPROPERTY(EditAnywhere)
	float JumpHeightHookLaunch = 200;

	bool ShouldSlide = false;

	bool IsRunning = false;

	bool bIsWallRunning = false;

	bool WasAir = false;
	UPROPERTY(EditAnywhere)
	float TimerStartHook = 5;

	float CurrentTimeHook = 0;


	//UCableComponent* CableComponent = nullptr;


	FVector HitNormalRight;
	FVector HitNormalLeft;

	class APlayerController* PlayerControllerGlobal;

	UWorld* World = nullptr;
	float DeltaTimeWorld;



	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
public:
	AMyProjectCharacter();

protected:
	virtual void BeginPlay();

private:
	//AMainPlayerCameraManager* GetMainPlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasFiredHook = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector HookHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsHooking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldBeAbleToShoot = false;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();


protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void StopMove();

	void FireHook();
	void StopFiringHook();
	void HookTimer();

	bool ShouldWallrun();
	void WallRun();

	void JumpFromWall();
	void StopJumpFromWall();

	bool ShouldHook();

	void Slide();

	void Run();
	void StopRunning();

	void StartCrouch();
	void StopCrouch();


	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
	virtual void Tick(float DeltaTime) override;

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};