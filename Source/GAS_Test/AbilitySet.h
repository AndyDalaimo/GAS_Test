// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySet.h"
#include "MyAbilitySystemComponent.h"
#include "AbilitySet.generated.h"

UENUM(BlueprintType)
enum class EAbilityInput
{
	None = 0 UMETA(Hidden),
	Jump = 1,
	PrimaryAttack = 2,
	SecondaryAttack = 3,
};


USTRUCT()
struct FAbilitySetItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	EAbilityInput InputKey;
};


/**
 * 
 */
UCLASS()
class GAS_TEST_API UAbilitySet : public UGameplayAbilitySet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<FAbilitySetItem> AbilitySetItems;

	TArray<FGameplayAbilitySpecHandle> GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbiltySystemComponent) const;

	
};
