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
	if (AbilityIndex < InventoryAbilitySet->AbilitySetItems.Num() && IsValid(OwningASC))
	{
		FString AbilityTag = InventoryAbilitySet->AbilitySetItems[AbilityIndex].GameplayAbility.Get()->GetName().LeftChop(2).RightChop(3);
		OwningASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Inventory.Ability." + AbilityTag + ".Unequipped")));
		OwningASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Inventory.Ability." + AbilityTag + ".Equipped")));

		// Set the Currently equipped item
		CurrentEquippedItem = InventoryAbilitySet->AbilitySetItems[AbilityIndex];
		UE_LOG(LogTemp, Warning, TEXT("%s Ability Equipped"), *AbilityTag);
	}
}

// Set the Specified Ability to Unequipped
void UInventory::SetAbilityToUnequipped(int AbilityIndex)
{
	if (AbilityIndex < InventoryAbilitySet->AbilitySetItems.Num())
	{
		FString AbilityTag = InventoryAbilitySet->AbilitySetItems[AbilityIndex].GameplayAbility.Get()->GetName().LeftChop(2).RightChop(3);
		OwningASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Inventory.Ability." + AbilityTag + ".Equipped")));
		OwningASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Inventory.Ability." + AbilityTag + ".Unequipped")));
		UE_LOG(LogTemp, Warning, TEXT("%s Ability Unequipped"), *AbilityTag);
	}
}

// Cycle through Inventory, Equipping item player chooses
// Unequip previously equipped item
void UInventory::CycleAbility()
{
	for (int i = 0; i < InventoryAbilitySet->AbilitySetItems.Num(); i++)
	{
		if (CurrentEquippedItem.GameplayAbility == InventoryAbilitySet->AbilitySetItems[i].GameplayAbility)
		{
			// Cycle to front of array if cycle has reached the end
			if (i == InventoryAbilitySet->AbilitySetItems.Num() - 1)
			{
				this->SetAbilityToUnequipped(i);
				this->SetAbilityToEquipped(0);
				break;
			}
			else {
				this->SetAbilityToUnequipped(i);
				this->SetAbilityToEquipped(i + 1);
				break;
			}
		}
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
		// InventoryAbilitySet->AbilitySetItems.Push(NewItem);
		InventoryAbilitySet->AbilitySetItems.Insert(NewItem, 0);
		// Equip newly picked up item, unequip previous weapon
		this->SetAbilityToEquipped(0);
		if (InventoryAbilitySet->AbilitySetItems.Num() >= 1) this->SetAbilityToUnequipped(1);
		
	}

}

// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

