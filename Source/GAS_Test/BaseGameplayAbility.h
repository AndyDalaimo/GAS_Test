// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

class UMyAbilitySystemComponent;
class AGAS_TestCharacter;


// ----------------------------------------------------
// Define Structs and Enums for Ability Interface
// ----------------------------------------------------

/**
*	EAbilityActivationPolicy
*	Define how an ability is meant to activate
*/

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered UMETA(DisplayName = "On Input Triggered"),

	// Continually try to activate the ability while the input is active.
	WhileInputActive UMETA(DisplayName = "While Input Active"),

	// Try to activate the ability when an avatar is assigned.
	OnSpawn UMETA(DisplayName = "On Spawn")
};

/**
*	EAbilityActivationGroup
*	How an ability is meant to activate in relation to others
*/
UENUM(BlueprintType)
enum class EAbilityActivationGroup : uint8
{
	// Ability runs independently of all other abilities.
	Independent UMETA(DisplayName = "Independent"),

	// Ability is canceled and replaced by other exclusive abilities.
	Exclusive_Replaceable UMETA(DisplayName = "Exclusive_Replaceable"),

	// Ability blocks all other exclusive abilities from activating.
	Exclusive_Blocking UMETA(DisplayName = "Exclusive_Blocking"),

	MAX	UMETA(Hidden)
};

/** Failure reason that can be used to play an animation montage when a failure occurs */
USTRUCT(BlueprintType)
struct FAbilityMontageFailureMessage
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		TObjectPtr<APlayerController> PlayerController = nullptr;

	// All the reasons why this ability has failed
	UPROPERTY(BlueprintReadWrite)
		FGameplayTagContainer FailureTags;

	UPROPERTY(BlueprintReadWrite)
		TObjectPtr<UAnimMontage> FailureMontage = nullptr;
};


/**
 *    This is the Base Gameplay Ability for project
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class GAS_TEST_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

		friend class UMyAbilitySystemComponent;

	UBaseGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintType, Category = "Game|Ability")
		UMyAbilitySystemComponent* GetMyAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Ability")
		APlayerController* GetPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Ability")
		AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Ability")
		AGAS_TestCharacter* GetCharacterFromActorInfo() const;

	EAbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }
	EAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:

	// Called when the ability fails to activate
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;

	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~End of UGameplayAbility interface


	virtual void OnPawnAvatarSet();

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
		void K2_OnAbilityAdded();

	/** Called when this ability is removed from the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
		void K2_OnAbilityRemoved();

	/** Called when the ability system is initialized with a pawn avatar. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
		void K2_OnPawnAvatarSet();

protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Ability Activation")
		EAbilityActivationPolicy ActivationPolicy;

	// Defines the relationship between this ability activating and other abilities activating.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game|Ability Activation")
		EAbilityActivationGroup ActivationGroup;


};
