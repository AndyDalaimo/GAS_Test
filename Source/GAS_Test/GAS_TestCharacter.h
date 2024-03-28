// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySet.h"
#include "MyAttributeSet.h"
#include "Inventory.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GAS_TestCharacter.generated.h"

USTRUCT()
struct FAbilityInputToInputActionBinding
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	EAbilityInput AbilityInput;
};

USTRUCT()
struct FAbilityInputBindings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TArray<FAbilityInputToInputActionBinding> Bindings;

};

UCLASS(config=Game)
class AGAS_TestCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	// Inventory Input Action. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InventoryAction;
	
	// Inventory Input Action. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CycleInventoryAction;

public:
	AGAS_TestCharacter(const FObjectInitializer& ObjectInitializer);

	// Returns this Actor's AbilitySystemComponent
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Returns this Actor's AbilitySet
	UFUNCTION(BlueprintCallable)
		UAbilitySet* GetAbilitySet();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
		class UInventory* InventoryComponent;

	UFUNCTION()
		void GrantNewAbility(UAbilitySet* NewAbilitySet);

protected:

	// Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	class UMyAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input)
		FGameplayTagContainer RestrictedMovementTags;

	UPROPERTY()
	const UMyAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = AbilitySystem)
	UAbilitySet* InitialAbilitySet { nullptr };

	UPROPERTY(EditDefaultsOnly, Category = AbilitySystem)
	TSubclassOf<UGameplayEffect> InitialGameplayEffect;

	FGameplayEffectContextHandle GEContexthandle;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Binding")
	FAbilityInputBindings AbilityInputBindings;

	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> InitiallyGrantedAbilitySpecHandles;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called for Opening Inventory
	// TODO -- Tie this to UI event. Currently printing items in Character Inventory
	void OpenInventory(const FInputActionValue& Value);

	void CycleInventory(const FInputActionValue& Value);

	// Equip Ability into Bound Input
	void EquipAbility(int InventoryIndex);

	UFUNCTION()
	void SetupInitialAbilitiesAndEffects();

	UFUNCTION()
	void AbilityInputBindingPressedHandler(EAbilityInput ablityInput);

	UFUNCTION()
	void AbilityInputBindingReleasedHandler(EAbilityInput ablityInput);

	// On Changes to Player's Attributes
	void OnHealthAttributeChanged(const FOnAttributeChangeData& onAttributeChangeData) const;
	void OnMoveSpeedAttributeChanged(const FOnAttributeChangeData& onAttributeChangeData) const;

	UFUNCTION(BlueprintPure)
		float GetHealth() const;

	UFUNCTION(BlueprintPure)
		float GetMoveSpeed() const;

	UFUNCTION(BlueprintNativeEvent)
		void UpdateHealthText();
	void UpdateHealthText_Implementation() {  }
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

