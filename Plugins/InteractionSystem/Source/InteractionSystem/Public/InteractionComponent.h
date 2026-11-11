// Copyright Dredd 2026. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionSystemTypes.h"
#include "InteractionComponent.generated.h"

/**
 * Attach to any pawn/character. Handles raycast look-at detection,
 * highlight management, and interaction execution.
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInteractionComponent();

    /* ── Configuration ── */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionRange = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionRadius = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TEnumAsByte<ECollisionChannel> InteractionChannel = ECC_Visibility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bDrawDebug = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bRequiresLineOfSight = true;

    /** If true, re-check look target every tick. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bContinuousLookCheck = false;

    /* ── Actions ── */
    /** Try to interact with the current look target. */
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void TryInteract();

    /** Force a re-scan for the nearest interactable. */
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ScanForInteractables();

    /* ── Queries ── */
    UFUNCTION(BlueprintPure, Category = "Interaction")
    AActor* GetCurrentLookTarget() const { return CurrentLookTarget.Get(); }

    UFUNCTION(BlueprintPure, Category = "Interaction")
    bool HasValidTarget() const { return CurrentLookTarget.IsValid(); }

    UFUNCTION(BlueprintPure, Category = "Interaction")
    FText GetTargetPrompt() const;

    UFUNCTION(BlueprintPure, Category = "Interaction")
    float GetHoldProgress() const { return HoldProgress; }

    /* ── Events ── */
    UPROPERTY(BlueprintAssignable, Category = "Interaction|Events")
    FOnLookTargetChanged OnLookTargetChanged;

    UPROPERTY(BlueprintAssignable, Category = "Interaction|Events")
    FOnInteractionStarted OnInteractionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Interaction|Events")
    FOnInteractionCompleted OnInteractionCompleted;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    void UpdateLookTarget();
    void ClearLookTarget();
    bool TraceForInteractable(AActor*& OutActor);

    UPROPERTY()
    TWeakObjectPtr<AActor> CurrentLookTarget;

    UPROPERTY()
    TWeakObjectPtr<AActor> CurrentHighlighted;

    bool bIsInteracting = false;
    float HoldProgress = 0.0f;
    float HoldTargetDuration = 0.0f;
};
