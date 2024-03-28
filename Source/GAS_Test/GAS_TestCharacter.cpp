// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_TestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AGAS_TestCharacter

AGAS_TestCharacter::AGAS_TestCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create AbilitySystemComponent and attach Attribute Set
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("AbiltySystemComp"));
	// Create AttributeSet
	AttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));
	// Create and attach Inventory Comp
	InventoryComponent = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));

	// Restricted Movement Tags
	RestrictedMovementTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));
	RestrictedMovementTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Melee.Block"));
	RestrictedMovementTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Weapon.Powerup"));


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGAS_TestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Instantiate the Attribute set onto the ASC
	AttributeSet = AbilitySystemComponent->GetSet<UMyAttributeSet>();

	InventoryComponent->OwningASC = GetAbilitySystemComponent();

	if (HasAuthority())
	{
		SetupInitialAbilitiesAndEffects();
	}

	// 
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGAS_TestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGAS_TestCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGAS_TestCharacter::Look);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &AGAS_TestCharacter::OpenInventory);

		// Set Bindings for AbilityInputs
		for (const FAbilityInputToInputActionBinding& binding : AbilityInputBindings.Bindings)
		{
			EnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Triggered, this, &AGAS_TestCharacter::AbilityInputBindingPressedHandler, binding.AbilityInput);
			//EnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Triggered, this, &ThisClass::AbilityInputBindingPressedHandler);
			EnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &AGAS_TestCharacter::AbilityInputBindingReleasedHandler, binding.AbilityInput);
			//EnhancedInputComponent->BindAction(binding.InputAction, ETriggerEvent::Completed, this, &ThisClass::AbilityInputBindingReleasedHandler);
		}

	}

}

// ---------------------------------------------------------------------------------------------------
// ------------------------------- Input Action Delegate Functions -----------------------------------
// ---------------------------------------------------------------------------------------------------

void AGAS_TestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !AbilitySystemComponent->HasAnyMatchingGameplayTags(RestrictedMovementTags))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
	

void AGAS_TestCharacter::Look(const FInputActionValue& Value)
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

// Open Inventory and Show current Items in CharacterInventory DataAsset
// TODO -- Create readable UI for this instead of printing each element
void AGAS_TestCharacter::OpenInventory(const FInputActionValue& Value)
{

	if (IsValid(InventoryComponent))
	{
		// Print All of the Gameplay Abilities in the Inventory Here
		for (FAbilitySetItem item : InventoryComponent->InventoryAbilitySet->AbilitySetItems)
		{
			UE_LOG(LogTemp, Display, TEXT("Item: %s"), *item.GameplayAbility.Get()->GetName().LeftChop(2).RightChop(3));
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Inventory not Valid"));
	}
	
}


// ---------------------------------------------------------------------------------------------------
// ------------------------------------ ASC Setup Functions ------------------------------------------
// ---------------------------------------------------------------------------------------------------

// Return the ASC
UAbilitySystemComponent* AGAS_TestCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGAS_TestCharacter::SetupInitialAbilitiesAndEffects()
{
	if (IsValid(AbilitySystemComponent) == false || IsValid(AttributeSet) == false)
	{
		return;
	}

	if (IsValid(InitialAbilitySet))
	{
		// InitiallyGrantedAbilitySpechandles
		InitiallyGrantedAbilitySpecHandles.Append(InitialAbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
	}

	if (IsValid(InitialGameplayEffect))
	{
		GEContexthandle = AbilitySystemComponent->MakeEffectContext();
		// AbilitySystemComponent->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(), 0.f, AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectToSelf(InitialGameplayEffect->GetDefaultObject<UGameplayEffect>(), 0.f, GEContexthandle);
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMyAttributeSet::GetHealthAttribute()).AddUObject(this, &AGAS_TestCharacter::OnHealthAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMyAttributeSet::GetMoveSpeedAttribute()).AddUObject(this, &AGAS_TestCharacter::OnMoveSpeedAttributeChanged);
}

// Bind Ability pressed actions onto ASC
void AGAS_TestCharacter::AbilityInputBindingPressedHandler(EAbilityInput ablityInput)
{
	AbilitySystemComponent->AbilityLocalInputPressed(static_cast<uint32>(ablityInput));
}

// Bind Ability released actions onto ASC
void AGAS_TestCharacter::AbilityInputBindingReleasedHandler(EAbilityInput ablityInput)
{
	AbilitySystemComponent->AbilityLocalInputReleased(static_cast<uint32>(ablityInput));
}

// ---------------------------------------------------------------------------------------------------
// ------------------------------- On Player's Attributes Changed ------------------------------------
// ---------------------------------------------------------------------------------------------------



void AGAS_TestCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& onAttributeChangeData) const
{
	if (FMath::IsNearlyEqual(onAttributeChangeData.NewValue, 0.0f) && onAttributeChangeData.OldValue > 0)
	{
		// When health reaches zero, initiate some death sequence 
		AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
	}
}

void AGAS_TestCharacter::OnMoveSpeedAttributeChanged(const FOnAttributeChangeData& onAttributeChangeData) const
{
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
}


// ---------------------------------------------------------------------------------------------------
// ------------------------------- Getters for Player's Attribute Set --------------------------------
// ---------------------------------------------------------------------------------------------------


float AGAS_TestCharacter::GetHealth() const
{
	if (IsValid(AttributeSet) == false)
	{
		return 0.f;
	}
	return AttributeSet->GetHealth();
}

float AGAS_TestCharacter::GetMoveSpeed() const
{
	if (IsValid(AttributeSet) == false)
	{
		return 0.f;
	}
	return AttributeSet->GetMoveSpeed();
}

UAbilitySet* AGAS_TestCharacter::GetAbilitySet()
{
	if (InitialAbilitySet != nullptr) return nullptr;
	else return InitialAbilitySet;
}

// ---------------------------------------------------------------------------------------------------
// --------------------------------- Inventory / Ability Control -------------------------------------
// ---------------------------------------------------------------------------------------------------

// Add new abilities from Pickup (Stored in Inventory) to ASC
void AGAS_TestCharacter::GrantNewAbility(UAbilitySet* NewAbilitySet)
{
	InitiallyGrantedAbilitySpecHandles.Append(NewAbilitySet->GrantAbilitiesToAbilitySystem(AbilitySystemComponent));
}

// Take Usable ability from Inventory and Bind ability to Input
void AGAS_TestCharacter::EquipAbility(int InventoryIndex)
{
	// AbilitySystemComponent->GiveA
}



