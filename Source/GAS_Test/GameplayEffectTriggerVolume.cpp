// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffectTriggerVolume.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"



// Sets default values
AGameplayEffectTriggerVolume::AGameplayEffectTriggerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsActive = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(FName("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(10, 10, 10));
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = CollisionComp;

	IgnoredTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));

}

// Called when the game starts or when spawned
void AGameplayEffectTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Get Replicated Properties 
void AGameplayEffectTriggerVolume::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameplayEffectTriggerVolume, bIsActive);
}

bool AGameplayEffectTriggerVolume::CanEffectCharacter(AGAS_TestCharacter* TestCharacter) const
{
	return bIsActive && TestCharacter && IsValid(this) && 
		!TestCharacter->GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(IgnoredTags);
}

// ------------------------------------------------------------------------
// ----------------- Actor Notifies for Overlap Events --------------------
// ------------------------------------------------------------------------

void AGameplayEffectTriggerVolume::NotifyActorBeginOverlap(AActor* Other)
{
	if (GetLocalRole() == ROLE_Authority && Other && Other != this)
	{
		// Call Overlapped Volume for functionality and ASC Calls to Other Actor
		OverlappedVolume(Cast<AGAS_TestCharacter>(Other));
	}
}

void AGameplayEffectTriggerVolume::NotifyActorEndOverlap(AActor* Other)
{
	if (GetLocalRole() == ROLE_Authority && Other && Other != this)
	{
		// Call Overlapped Volume for functionality and ASC Calls to Other Actor
		ExitedVolume(Cast<AGAS_TestCharacter>(Other));
	}
}

void AGameplayEffectTriggerVolume::OverlappedVolume(AGAS_TestCharacter* Pawn)
{
	if (CanEffectCharacter(Pawn))
	{
		ApplyEffectTo(Pawn);
		bIsActive = false;

		// Trigger Gameplay Cues, etc. 
		K2_OnOverlapped();
	}
}

void AGameplayEffectTriggerVolume::ExitedVolume(AGAS_TestCharacter* Pawn)
{
	RemoveEffectFrom(Pawn);
	bIsActive = true;

	// Trigger Gameplay Cues, etc. 
	K2_OnExited();
}

// ------------------------------------------------------------------------
// ------- Event Functions to call and Apply/Remove Gameplay Effects ------
// ------------------------------------------------------------------------

void AGameplayEffectTriggerVolume::ApplyEffectTo(AGAS_TestCharacter* Pawn)
{
	// Grab the ASC from the Pawn 
	UAbilitySystemComponent* ASC = Pawn->GetAbilitySystemComponent();

	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Pawn's ASC is null."), *FString(Pawn->GetName()));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("%s Pawn's ASC is Active."), *FString(Pawn->GetName()));

	// Create Gameplay Effect context to add to pawn
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// Add GameplayEffecs to Ability System Comp of interacting Pawn
	for (TSubclassOf<UGameplayEffect> EffectClass : EffectClasses)
	{
		if (!EffectClass)
		{
			continue;
		}

		FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(EffectClass, 0.f, EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		
	}

}

void AGameplayEffectTriggerVolume::RemoveEffectFrom(AGAS_TestCharacter* Pawn)
{
	// Grab the ASC from the Pawn 
	UAbilitySystemComponent* ASC = Pawn->GetAbilitySystemComponent();
	
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Pawn's ASC is null."), *FString(Pawn->GetName()));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("%s Pawn's ASC is Active."), *FString(Pawn->GetName()));

	// Add GameplayEffecs to Ability System Comp of interacting Pawn
	for (TSubclassOf<UGameplayEffect> EffectClass : EffectClasses)
	{
		if (!EffectClass)
		{
			continue;
		}

		ASC->RemoveActiveGameplayEffectBySourceEffect(EffectClass, nullptr, -1);
	}
}




// Called every frame
/*void AGameplayEffectTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/


void AGameplayEffectTriggerVolume::OnRep_IsActive()
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
		K2_OnOverlapped();
	}
}
