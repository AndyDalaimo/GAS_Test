// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"




// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = true;
	bReplicates = true;
	bCanRespawn = true;

	// To Use later for respawning this pickup
	RespawnTime = 0.f;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(FName("CollisionComp"));
	CollisionComp->InitCapsuleSize(40.f, 50.f);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = CollisionComp;

	// TODO --------------------- TODO
	// Add restricted pickup tags
	// TODO --------------------- TODO
	RestrictedPickupTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));

}



// Called when the game starts or when spawned
void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
/*void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

// Get Replicated Properties 
void ABasePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePickup, bIsActive);
	DOREPLIFETIME(ABasePickup, InteractingCharRef);

}

bool ABasePickup::CanBePickedUp(AGAS_TestCharacter* TestCharacter) const
{
	return bIsActive && TestCharacter && IsValid(this) && 
		!TestCharacter->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(RestrictedPickupTags);
}

// Override function called when actor overlaps with Pawn
void ABasePickup::NotifyActorBeginOverlap(AActor* Other)
{
	if (GetLocalRole() == ROLE_Authority && Other && Other != this)
	{
		// Call PickupItem for functionality and ASC Calls to other actor
		PickupItem(Cast<AGAS_TestCharacter>(Other));
	}
}

// If the actor overlapping with this item can pick it up, 
// proceed with helper functions
void ABasePickup::PickupItem(AGAS_TestCharacter* Pawn)
{
	if (CanBePickedUp(Pawn))
	{
		GivePickupTo(Pawn);
		InteractingCharRef = Pawn;
		bIsActive = false;
		
		// TODO ----------- TODO
		// Call some functionality to play sound, etc. 
		K2_OnPickedUp();
		// TODO ----------- TODO

		if (bCanRespawn && RespawnTime > 0.0f)
		{
			// TODO ----------- TODO
			// Call a timer to reset respawn time and bCanRespawn
			// TODO ----------- TODO
		}
		else
		{
			Destroy();
		}
	}
}

// If the Pawn interacting with this Pickup has an AbilitySystemComp,
// Apply the correct gameplay effects and change GameplayTags
void ABasePickup::GivePickupTo(AGAS_TestCharacter* Pawn)
{
	// Grab the ASC from the Pawn along with it's InitialAbility Set
	UAbilitySystemComponent* ASC = Pawn->GetAbilitySystemComponent();

	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Pawn's ASC is null."), *FString(Pawn->GetName()));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("%s Pawn's ASC is Active."), *FString(Pawn->GetName()));

	// Add AbilitySetItem from Pickup into the Pawn's Inventory
	// Grant new Ability to Pawn if Pickup has ability attached to it
	if (AbilityToPickup.GameplayAbility)
	{

		/*for (FAbilitySetItem SetItem : AbilitySet->AbilitySetItems)
		{
			if (!SetItem.GameplayAbility)
			{
				continue;
			}

			Pawn->InventoryComponent->AddWeaponToInventory(SetItem);
		}*/
		
		Pawn->InventoryComponent->AddWeaponToInventory(AbilityToPickup);
		Pawn->GrantNewAbility(Pawn->InventoryComponent->InventoryAbilitySet);
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Ability Attached to Pickup is not valid"));
	}

	// Create Gameplay effect context to Add to pawn
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();

	EffectContext.AddSourceObject(this);

	// Add Gameplay Effect to Ability System Comp of interacting Pawn
	for (TSubclassOf<UGameplayEffect> EffectClass : EffectClasses)
	{
		if (!EffectClass)
		{
			continue;
		}

		// TODO -- Change when Characters have a Level associated with them
		FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(EffectClass, 0.f, EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		// Debug Print ---------------------------------------------- Debug Print
		// UE_LOG(LogTemp, Display, TEXT("%d SpeedBoost tags active."), ASC->GetGameplayTagCount(FGameplayTag::RequestGameplayTag("Effect.SpeedBoost")));
	}
}


void ABasePickup::OnRep_IsActive()
{
	if (bIsActive)
	{
		// TODO --------------------- TODO
		// Create respawned help function 
		// TODO --------------------- TODO
	}
	else
	{
		// TODO --------------------- TODO
		// Create PickUp help function 
		// TODO --------------------- TODO
		K2_OnPickedUp();
	}
}
