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
bool UInventory::AddWeaponToInventory(TSubclassOf<UGameplayAbility> Weapon, EAbilityInput InputKey)
{

	/*if (IsValid(OwningCharacter.GetAbilitySystemComponent()) == false || IsValid(WeaponSet) == false)
	{
		return false;
	}

	// Build SetItem Struct to pass into WeaponSet
	FAbilitySetItem NewItem;
	NewItem.GameplayAbility = Weapon;
	NewItem.InputKey = InputKey;

	// Add Weapon into 
	WeaponSet->AbilitySetItems.Push(NewItem);
		
	WeaponAbilitySpecHandles.Append(WeaponSet->GrantAbilitiesToAbilitySystem(OwningCharacter->GetAbilitySystemComponent()));*/

	return true;
	
}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

