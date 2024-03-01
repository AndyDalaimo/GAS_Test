// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySet.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Inventory.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_TEST_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

	// Add Weapon to Inventory by passing GA Weapon and InputKey
	UFUNCTION(BlueprintCallable)
		bool AddWeaponToInventory(TSubclassOf<UGameplayAbility> Weapon, EAbilityInput InputKey);
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		UAbilitySystemComponent* OwningASC;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|Weapons")
		UAbilitySet* WeaponSet { nullptr };

	UPROPERTY(Transient)
		TArray<FGameplayAbilitySpecHandle> WeaponAbilitySpecHandles;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
