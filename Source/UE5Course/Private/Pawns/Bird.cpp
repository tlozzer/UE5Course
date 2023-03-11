// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Bird Collision Capsule"));
	SetRootComponent(CapsuleCollision);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bird Skeletal Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	BirdMovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement Component"));
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (!BirdMappingContext.IsNull())
			{
				Subsystem->AddMappingContext(BirdMappingContext.LoadSynchronous(), 0);
			}
		}
	}
}

void ABird::Move(const FInputActionValue& Value)
{
	const float CurrentValue = Value.Get<float>();
	if ((GetController() != nullptr) && (CurrentValue != 0.f))
	{
		AddMovementInput(GetActorForwardVector(), CurrentValue);
	}
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (!MoveAction.IsNull())
		{
			PlayerEnhancedInputComponent->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ABird::Move);
		}
	}
}

