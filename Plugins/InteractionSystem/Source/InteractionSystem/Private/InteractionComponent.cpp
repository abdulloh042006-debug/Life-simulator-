// Copyright Dredd 2026. All rights reserved.

#include "InteractionComponent.h"
#include "InteractableInterface.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bContinuousLookCheck)
    {
        UpdateLookTarget();
    }

    // Hold-to-interact progress
    if (bIsInteracting && CurrentLookTarget.IsValid())
    {
        HoldProgress += DeltaTime;
        if (HoldProgress >= HoldTargetDuration)
        {
            bIsInteracting = false;
            HoldProgress = 0.0f;

            AActor* Target = CurrentLookTarget.Get();
            AActor* Owner = GetOwner();
            if (Target && Target->Implements<UInteractableInterface>())
            {
                // Use Execute_Interact so Blueprint-only interface implementations work too.
                IInteractableInterface::Execute_Interact(Target, Owner);
                OnInteractionCompleted.Broadcast(Owner, Target);
            }
        }
    }
}

bool UInteractionComponent::TraceForInteractable(AActor*& OutActor)
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return false;
    }

    APawn* Pawn = Cast<APawn>(Owner);
    APlayerController* PC = nullptr;
    
    if (Pawn)
    {
        PC = Cast<APlayerController>(Pawn->GetController());
    }

    FVector EyeLocation;
    FRotator EyeRotation;
    
    if (PC)
    {
        PC->GetPlayerViewPoint(EyeLocation, EyeRotation);
    }
    else
    {
        EyeLocation = Owner->GetActorLocation();
        EyeRotation = Owner->GetActorRotation();
    }

    const FVector Start = EyeLocation;
    const FVector End = Start + EyeRotation.Vector() * InteractionRange;

    FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractionRadius);
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Owner);
    if (bRequiresLineOfSight)
    {
        QueryParams.bTraceComplex = true;
    }

    FHitResult Hit;
    bool bHit = GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity,
        InteractionChannel, Sphere, QueryParams);

    if (bDrawDebug)
    {
        DrawDebugSphere(GetWorld(), End, InteractionRadius, 8, bHit ? FColor::Green : FColor::Red, false, 0.02f);
        DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 0.02f);
    }

    if (!bHit || !Hit.GetActor())
    {
        return false;
    }

    // Check if actor implements the interface
    if (!Hit.GetActor()->Implements<UInteractableInterface>())
    {
        return false;
    }

    if (!IInteractableInterface::Execute_CanInteract(Hit.GetActor(), Owner))
    {
        return false;
    }

    // bTraceComplex only changes collision precision; it does not provide line of sight.
    // Explicitly reject an interactable hidden behind another blocking actor.
    if (bRequiresLineOfSight)
    {
        FCollisionQueryParams LOSParams;
        LOSParams.AddIgnoredActor(Owner);
        FHitResult LOSHit;
        const bool bBlocked = GetWorld()->LineTraceSingleByChannel(
            LOSHit, Start, Hit.ImpactPoint, InteractionChannel, LOSParams);
        if (bBlocked && LOSHit.GetActor() != Hit.GetActor())
        {
            return false;
        }
    }

    OutActor = Hit.GetActor();
    return true;
}

void UInteractionComponent::UpdateLookTarget()
{
    AActor* NewTarget = nullptr;
    const bool bFound = TraceForInteractable(NewTarget);

    AActor* OldTarget = CurrentLookTarget.Get();

    if (bFound && NewTarget != OldTarget)
    {
        // Remove highlight from old
        if (OldTarget && OldTarget->Implements<UInteractableInterface>())
        {
            IInteractableInterface::Execute_OnLookEnd(OldTarget, GetOwner());
        }
        CurrentHighlighted = nullptr;

        // Add highlight to new
        if (NewTarget)
        {
            IInteractableInterface::Execute_OnLookStart(NewTarget, GetOwner());
            CurrentHighlighted = NewTarget;
        }

        CurrentLookTarget = NewTarget;
        OnLookTargetChanged.Broadcast(NewTarget);

        // Cancel hold if target changed
        if (bIsInteracting)
        {
            bIsInteracting = false;
            HoldProgress = 0.0f;
        }
    }
    else if (!bFound && OldTarget)
    {
        ClearLookTarget();
    }
}

void UInteractionComponent::ClearLookTarget()
{
    AActor* OldTarget = CurrentLookTarget.Get();
    if (OldTarget && OldTarget->Implements<UInteractableInterface>())
    {
        IInteractableInterface::Execute_OnLookEnd(OldTarget, GetOwner());
        CurrentHighlighted = nullptr;
    }

    CurrentLookTarget = nullptr;
    OnLookTargetChanged.Broadcast(nullptr);
    bIsInteracting = false;
    HoldProgress = 0.0f;
}

void UInteractionComponent::ScanForInteractables()
{
    UpdateLookTarget();
}

FText UInteractionComponent::GetTargetPrompt() const
{
    if (CurrentLookTarget.IsValid() && CurrentLookTarget->Implements<UInteractableInterface>())
    {
        return IInteractableInterface::Execute_GetInteractionPrompt(CurrentLookTarget.Get(), GetOwner());
    }
    return FText::GetEmpty();
}

void UInteractionComponent::TryInteract()
{
    AActor* Target = CurrentLookTarget.Get();
    if (!Target || !Target->Implements<UInteractableInterface>())
    {
        return;
    }

    AActor* Owner = GetOwner();

    // Check hold vs press
    const EInteractionTrigger Trigger = IInteractableInterface::Execute_GetInteractionTrigger(Target);

    if (Trigger == EInteractionTrigger::Hold)
    {
        if (!bIsInteracting)
        {
            bIsInteracting = true;
            HoldProgress = 0.0f;
            HoldTargetDuration = FMath::Max(
                IInteractableInterface::Execute_GetHoldDuration(Target), 0.01f);
            OnInteractionStarted.Broadcast(Owner, Target);
        }
    }
    else if (Trigger == EInteractionTrigger::Press)
    {
        OnInteractionStarted.Broadcast(Owner, Target);
        IInteractableInterface::Execute_Interact(Target, Owner);
        OnInteractionCompleted.Broadcast(Owner, Target);
    }
    else if (Trigger == EInteractionTrigger::Toggle)
    {
        OnInteractionStarted.Broadcast(Owner, Target);
        IInteractableInterface::Execute_Interact(Target, Owner);
        OnInteractionCompleted.Broadcast(Owner, Target);
    }
}
