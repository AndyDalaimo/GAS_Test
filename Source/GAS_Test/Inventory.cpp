// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	

	// ...
}



// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Add New weapon into Inventory
// Will grant the ability in this Actor Component's WeaponSet
void UInventory::AddWeaponToInventory(TSubclassOf<UGameplayAbility> Weapon, EAbilityInput InputKey)
{

	if (IsValid(WeaponSet) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("__FUNCTION__"));
		return;
	}

	// Build SetItem Struct to pass into WeaponSet
	FAbilitySetItem NewItem;
	NewItem.GameplayAbility = Weapon;
	NewItem.InputKey = InputKey;
	bool addItem = false;

	for (FAbilitySetItem item : WeaponSet->AbilitySetItems)
	{
		if (item.GameplayAbility != Weapon)
		{
			addItem = true;
		}
	}

	// Add Weapon into 
	if (addItem) WeaponSet->AbilitySetItems.Push(NewItem);
	


	// WeaponAbilitySpecHandles.Append(WeaponSet->GrantAbilitiesToAbilitySystem(OwningCharacter->GetAbilitySystemComponent()));
	
}

// Return Current State of Weapon Set
UAbilitySet* UInventory::GetWeaponSet()
{
	return WeaponSet;
}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

