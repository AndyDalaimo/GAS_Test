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


// Set the Specified Ability to Equipped
void UInventory::SetAbilityToEquipped(int AbilityIndex)
{
	if (AbilityIndex < InventoryAbilitySet->AbilitySetItems.Num())
	{
		InventoryAbilitySet->AbilitySetItems[AbilityIndex].GameplayAbility.GetDefaultObject()->AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.State.Equipped"));
	}
}

// Add New weapon into Inventory
// Will grant the ability in this Actor Component's WeaponSet
void UInventory::AddWeaponToInventory(FAbilitySetItem NewItem)
{
	// Check if Item is already in Inventory, If true, Do not add
	bool addItem = true;

	for (FAbilitySetItem item : InventoryAbilitySet->AbilitySetItems)
	{
		if (item.GameplayAbility.Get()->GetName().Compare(NewItem.GameplayAbility.Get()->GetName()) == 0)
		{
			addItem = false;
		}
	}

	// Add new Ability to Set
	if (addItem)
	{
		FGameplayAbilitySpec NewAbilitySpec = FGameplayAbilitySpec(NewItem.GameplayAbility, 0, static_cast<uint32>(NewItem.InputKey));
		InventoryAbilitySet->AbilitySetItems.Push(NewItem);
		
	}

}

// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

