// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS_TestCharacter.h"
#include "AbilitySet.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"
#include "BasePickup.generated.h"

UCLASS()
class GAS_TEST_API ABasePickup : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComp;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	FGameplayTagContainer RestrictedPickupTags;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsActive, Category = "Pickup")
	bool bIsActive;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	bool bCanRespawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	float RespawnTime;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	UAbilitySet* AbilitySet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	FAbilitySetItem ItemToPickup;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pickup")
	TArray<TSubclassOf<class UGameplayEffect>> EffectClasses;

	// Reference to Character interacting with this Pickup
	UPROPERTY(BlueprintReadOnly, Replicated)
	AGAS_TestCharacter* InteractingCharRef;

	// -----------------------------------------------------
	// ------ Helper Functions for Pickup Interaction ------
	// -----------------------------------------------------

	virtual void GivePickupTo(AGAS_TestCharacter* Pawn);

	void PickupItem(AGAS_TestCharacter* Pawn);

	UFUNCTION()
		virtual void OnRep_IsActive();

	// Blueprint implementable effects
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnPickedUp"))
		void K2_OnPickedUp();


	
public:	
	// Sets default values for this actor's properties
	ABasePickup();

	// Replicated Objects
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Check if Character can pick up this item
	virtual bool CanBePickedUp(AGAS_TestCharacter* TestCharacter) const;

	// Pickup on Actor Collision
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
