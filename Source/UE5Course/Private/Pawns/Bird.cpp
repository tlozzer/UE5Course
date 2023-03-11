// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Bird Collision Capsule"));
	SetRootComponent(CapsuleCollision);
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bird Skeletal Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
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
	if (CurrentValue != 0.f)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Red, FString::Printf(TEXT("Value: %f"), CurrentValue));
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

