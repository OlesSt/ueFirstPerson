// Copyright Epic Games, Inc. All Rights Reserved.

#include "ueFirstPersonCharacter.h"
#include "ueFirstPersonProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AkAudioEvent.h"
#include "AkGameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AueFirstPersonCharacter

AueFirstPersonCharacter::AueFirstPersonCharacter()
{
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
    
    PrimaryActorTick.bCanEverTick = true;

}

void AueFirstPersonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AueFirstPersonCharacter::PlayFootstepSound()
{
    if (FootstepWwiseEvent != nullptr)
    {
        FOnAkPostEventCallback NullCallback;
        UAkGameplayStatics::PostEvent(
            FootstepWwiseEvent,
            this,
            0,
            NullCallback
        );
    }
}

void AueFirstPersonCharacter::Jump()
{
    Super::Jump();

    if (JumpUpWwiseEvent != nullptr)
    {
        FOnAkPostEventCallback NullCallback;
        UAkGameplayStatics::PostEvent(
            JumpUpWwiseEvent,
            this,
            0,
            NullCallback
        );
    }
}

void AueFirstPersonCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);

    if (JumpDownWwiseEvent != nullptr)
    {
        FOnAkPostEventCallback NullCallback;
        UAkGameplayStatics::PostEvent(
            JumpDownWwiseEvent,
            this,
            0,
            NullCallback
        );
    }
}

void AueFirstPersonCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UCharacterMovementComponent* MovComp = GetCharacterMovement();
    bool bIsMoving = GetVelocity().SizeSquared() > 0.f;
    bool bIsGrounded = MovComp && !MovComp->IsFalling();

    if (bIsMoving && bIsGrounded)
    {
        FootstepTimer -= DeltaTime;
        if (FootstepTimer <= 0.f)
        {
            FootstepTimer = FootstepInterval;
            PlayFootstepSound();
        }
    }
    else
    {
        FootstepTimer = 0.f;
    }
}

//////////////////////////////////////////////////////////////////////////// Input

void AueFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AueFirstPersonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AueFirstPersonCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AueFirstPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AueFirstPersonCharacter::Look(const FInputActionValue& Value)
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
