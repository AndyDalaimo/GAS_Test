// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySet.h"

TArray<FGameplayAbilitySpecHandle> UAbilitySet::GrantAbilitiesToAbilitySystem(UAbilitySystemComponent* AbiltySystemComponent) const
{

	check(AbiltySystemComponent);

	TArray<FGameplayAbilitySpecHandle> Handles;
	Handles.Reserve(AbilitySetItems.Num());

	for (const auto abilitySetItem : AbilitySetItems)
	{
		Handles.AddUnique(AbiltySystemComponent->GiveAbility(
			FGameplayAbilitySpec(
				abilitySetItem.GameplayAbility,
				0,
				static_cast<uint32>(abilitySetItem.InputKey)
			)));
	}


	return Handles;
}
